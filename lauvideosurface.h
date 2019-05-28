#ifndef LAUVIDEOSURFACE_H
#define LAUVIDEOSURFACE_H

#include <QList>
#include <QDebug>
#include <QLabel>
#include <QVideoFrame>
#include <QVideoSurfaceFormat>
#include <QAbstractVideoSurface>
#include <QCameraViewfinderSettings>

#include "lauvideoglwidget.h"

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
class LAUVideoSurface : public QAbstractVideoSurface
{
    Q_OBJECT

public:
    explicit LAUVideoSurface(QObject *parent = NULL) : QAbstractVideoSurface(parent), labelWidget(NULL) { ; }

    LAUVideoGLWidget *label() const
    {
        return (labelWidget);
    }

    void setLabel(LAUVideoGLWidget *lbl)
    {
        labelWidget = lbl;
    }

    QVideoSurfaceFormat nearestFormat(const QVideoSurfaceFormat &format) const;
    bool isFormatSupported(const QVideoSurfaceFormat &format) const;
    bool present(const QVideoFrame &frame);
    bool start(const QVideoSurfaceFormat &format);
    void stop();

    QList<QVideoFrame::PixelFormat> supportedPixelFormats(QAbstractVideoBuffer::HandleType type = QAbstractVideoBuffer::NoHandle) const;

private:
    LAUVideoGLWidget *labelWidget;
};

#endif // LAUVIDEOSURFACE_H
