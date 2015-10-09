/**
 * Copyright (c) 2012 Nokia Corporation.
 */

#ifndef __PERFORMANCEMETER__
#define __PERFORMANCEMETER__


#include <QDeclarativeItem>
#include <QImage>
#include <QTime>


const int PerformanceSamples(256);
const int MeasureIntervalMSecs(750);


class PerformanceMeter : public QDeclarativeItem
{
    Q_OBJECT

public:
    PerformanceMeter(QDeclarativeItem *parent = 0);
    virtual ~PerformanceMeter();

    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget);

protected:
    void line(unsigned int *vline, const int pitch, int y, int y2,
              const unsigned int col);
    void hline(unsigned int *d, int length, int col);
    bool measure();

protected:
    QTime m_lastMeasurementTime;
    int m_frameCounter;

    QPixmap m_displayPixmap;

    unsigned int m_maxSample;
    unsigned int m_sampleTable[PerformanceSamples];
};

QML_DECLARE_TYPE(PerformanceMeter)

#endif
