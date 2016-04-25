#ifndef Q_VIDEO_FRAME_SCOPE_MAP 
#define Q_VIDEO_FRAME_SCOPE_MAP 1

// TODO: move to QVideoFrameScope
struct QVideoFrameScopeMap
{
    QVideoFrameScopeMap(QVideoFrame *frame, QAbstractVideoBuffer::MapMode mode) : frame(frame)
    {
        if(frame)
        {
            status = frame->map(mode);
            if (!status)
            {
                qWarning("Can't map!");
            }
        }
    }
    ~QVideoFrameScopeMap()
    {
        if(frame)
        {
            frame->unmap();
        }
    }
    operator bool() const { return status; }
    QVideoFrame *frame = nullptr;
    bool status = false;
};

#endif // Q_VIDEO_FRAME_SCOPE_MAP 
