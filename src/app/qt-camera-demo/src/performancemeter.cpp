/**
 * Copyright (c) 2012-2014 Microsoft Mobile.
 */

#include "performancemeter.h"

#include <math.h>
#include <QtGui>


/*!
  \class PerformanceMeter
  \brief Measures performance between paints.
*/


/*!
  Construct and reset.
*/
PerformanceMeter::PerformanceMeter(QDeclarativeItem *parent)
    : QDeclarativeItem(parent)
{
    setFlag(QGraphicsItem::ItemHasNoContents, false);

    for (int f=0; f<PerformanceSamples; f++)
        m_sampleTable[f] = 0;

    m_frameCounter = 0;
    m_lastMeasurementTime = QTime::currentTime();
    m_maxSample = 256;
}


/*!
  Destructor.
*/
PerformanceMeter::~PerformanceMeter()
{
}


/*!
  Measures the FPS on current interval.
*/
bool PerformanceMeter::measure()
{
    m_frameCounter++;

    QTime ctime = QTime::currentTime();
    int msecsPassed = m_lastMeasurementTime.msecsTo(ctime);

    if (msecsPassed >= MeasureIntervalMSecs) {
        float secsPassed = (float)msecsPassed / 1000.0f;
        float fps = (float)m_frameCounter / secsPassed;

        qDebug("%d frames in %f secs: fps: %f", m_frameCounter,
               secsPassed, fps);

        // Scroll samples to right
        for (int f=PerformanceSamples-1; f>0; f--)
            m_sampleTable[f] = m_sampleTable[f-1];

        // Add the new sample
        m_sampleTable[0] = (int)(fps * 256.0);

        if (m_sampleTable[0] + 1024>m_maxSample)
            m_maxSample = m_sampleTable[0] + 1024;

        m_lastMeasurementTime = ctime;
        m_frameCounter = 0;

        return true;
    }

    return false;
}


/*!
  Draws the vertical line of the performance meter.
*/
void PerformanceMeter::line(unsigned int *vline, const int pitch, int y, int y2,
                            const unsigned int col)
{
    if (y>y2) {
        int temp = y2;
        y2 = y;
        y = temp;
    }

    do {
        vline[(y>>8)*pitch] |= col;
        y += 256;
    } while (y<y2);
}


/*!
  Draws the horizontal line of the performance meter.
*/
void PerformanceMeter::hline(unsigned int *d, int length, int col)
{
    while (length>0) {
        *d = col; d++; length--;
    }
}


/*!
 QDeclarativeItem's paint. This method render's the internal infoImage when
 required.
*/
void PerformanceMeter::paint(QPainter *painter,
                             const QStyleOptionGraphicsItem *option,
                             QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    int myWidth = boundingRect().width();
    int myHeight = boundingRect().height();

    if (myWidth<16 || myHeight<16)
        return;      // area too small to be rendered

    if (measure()) {  // new sample have been aquired
        if (m_displayPixmap.isNull() ||
            m_displayPixmap.width() != myWidth ||
            m_displayPixmap.height() != myHeight) {
            m_displayPixmap = QPixmap(myWidth, myHeight);
        }

        QImage timage = QImage(myWidth, myHeight, QImage::Format_ARGB32);

        int f,g;
        unsigned int *vline = ((unsigned int*)timage.bits());

        vline = (unsigned int*)timage.bits() + myWidth-1;
        int drawPitch = timage.bytesPerLine()/4;

        g = 0;
        int sinc = (m_maxSample) / myHeight;

        // scale
        for (f=0; f<myHeight; f++) {
            hline((unsigned int*)timage.bits() + (myHeight-1-f) * drawPitch,
                  myWidth, 0xaa000000 | ((((g>>8)/10)&1)*100+64));

            g += sinc;
        }

        int sample;
        int prevSample = ((myHeight-1)<<8)-(m_sampleTable[0])*(myHeight<<8) /
                m_maxSample;

        for (f=0; f<myWidth; f++) {
            sample = ((myHeight-1)<<8)-m_sampleTable[f]*(myHeight<<8) /
                    m_maxSample;

            line(vline, drawPitch,
                 prevSample,
                 sample,
                 0x00FFFFFF);

            prevSample = sample;
            vline--;
        }

        m_displayPixmap.convertFromImage(timage);
    }

    if (!m_displayPixmap.isNull())
        painter->drawPixmap(0, 0, m_displayPixmap);
}
