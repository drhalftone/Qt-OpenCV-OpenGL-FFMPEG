#ifndef LAUVIDEOGLWIDGET_H
#define LAUVIDEOGLWIDGET_H

#include <QTime>
#include <QImage>
#include <QScreen>
#include <QObject>
#include <QVideoFrame>
#include <QApplication>
#include <QOpenGLWidget>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include <QDesktopWidget>
#include <QGuiApplication>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLFramebufferObject>
#include <QOpenGLPixelTransferOptions>

#ifdef Q_OS_WIN
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;
#endif

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
class LAUVideoGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit LAUVideoGLWidget(QWidget *parent = NULL) : QOpenGLWidget(parent), videoTexture(NULL), counter(0) { ; }
    ~LAUVideoGLWidget();

    virtual bool isValid() const
    {
        return (wasInitialized());
    }

    bool wasInitialized() const
    {
        return (vertexArrayObject.isCreated());
    }

#ifdef Q_OS_WIN
    void setVideoRecorder(cv::VideoWriter **handle)
    {
        recorder = handle;
    }
#endif

    void setFrame(const QVideoFrame &frame);
    void setFrame(QImage frame);

    virtual QImage grabImage()
    {
        if (videoTexture) {
            makeCurrent();
            QImage image(videoTexture->width(), videoTexture->height(), QImage::Format_RGBA8888);
            videoTexture->bind();
            glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.bits());
            videoTexture->release();
            return (image);
        }
        return (QImage());
    }

    virtual void process() { ; }
    virtual void initialize();
    virtual void resize(int w, int h);
    virtual void paint();

protected:
    void initializeGL()
    {
        initialize();
    }

    void resizeGL(int w, int h)
    {
        resize(w, h);
    }

    void paintGL()
    {
        paint();
    }

#ifdef Q_OS_WIN
    cv::VideoWriter **recorder;
#endif

    QOpenGLVertexArrayObject vertexArrayObject;
    QOpenGLBuffer quadVertexBuffer, quadIndexBuffer;
    QOpenGLShaderProgram program;
    QOpenGLTexture *videoTexture;

    int localWidth, localHeight;
    qreal devicePixelRatio;

private:
    int counter;
    QTime time;
};

#endif // LAUVIDEOGLWIDGET_H
