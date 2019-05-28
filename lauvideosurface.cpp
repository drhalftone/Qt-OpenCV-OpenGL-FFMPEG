#include "lauvideosurface.h"
#include <stdio.h>

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
QList<QVideoFrame::PixelFormat> LAUVideoSurface::supportedPixelFormats(QAbstractVideoBuffer::HandleType type) const
{
    QList<QVideoFrame::PixelFormat> list;
    if (type == QAbstractVideoBuffer::NoHandle) {
        list << QVideoFrame::Format_RGB32;
        list << QVideoFrame::Format_ARGB32;
        list << QVideoFrame::Format_ARGB32_Premultiplied;
        list << QVideoFrame::Format_RGB555;
    }
    return (list);
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
QVideoSurfaceFormat LAUVideoSurface::nearestFormat(const QVideoSurfaceFormat &format) const
{
    return (format);
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
bool LAUVideoSurface::isFormatSupported(const QVideoSurfaceFormat &format) const
{
    if (QVideoFrame::imageFormatFromPixelFormat(format.pixelFormat()) == QImage::Format_Invalid) {
        return (false);
    } else if (format.frameSize().isEmpty()) {
        return (false);
    } else if (format.handleType() != QAbstractVideoBuffer::NoHandle) {
        return (false);
    }
    return (true);
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
bool LAUVideoSurface::present(const QVideoFrame &frame)
{
    // SEND THE IN-COMING VIDEO TO THE LABEL WIDGET, IF IT EXISTS
    if (labelWidget) {
        labelWidget->setFrame(frame);
    }
    return (true);
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
bool LAUVideoSurface::start(const QVideoSurfaceFormat &format)
{
    return (QAbstractVideoSurface::start(format));
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
void LAUVideoSurface::stop()
{
    QAbstractVideoSurface::stop();
}
