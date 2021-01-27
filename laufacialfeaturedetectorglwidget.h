#ifndef LAUFACIALFEATUREDETECTORGLWIDGET_H
#define LAUFACIALFEATUREDETECTORGLWIDGET_H

#include <QtCore>
#include <QObject>
#include <QSettings>
#include <QFileDialog>

#ifdef USEVISAGE
#include "visageVision.h"
#include "VisageTracker.h"
#endif

#include "opencv2/face.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "lauvideoglwidget.h"

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
class LAUFacialFeatureDetectorGLWidget : public LAUVideoGLWidget
{

public:
    explicit LAUFacialFeatureDetectorGLWidget(QWidget *parent = nullptr);
    ~LAUFacialFeatureDetectorGLWidget();

    QImage grabImage()
    {
        if (videoTexture) {
            makeCurrent();
            QImage image(videoTexture->width(), videoTexture->height(), QImage::Format_ARGB32);
            videoTexture->bind();
            glGetTexImage(GL_TEXTURE_2D, 0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)image.constBits());
            return(image);
        }
        return (QImage());
    }

    void initialize();
    void process();
    void paint();

private:
    QOpenGLBuffer faceVertexBuffer, faceIndexBuffer;
    QOpenGLFramebufferObject *frameBufferObject;
    QOpenGLShaderProgram programA, programB;

#ifdef USEVISAGE
    VsImage *inputImage;
    VisageSDK::VisageTracker *visageTracker;
    VisageSDK::FaceData faceData[16];
#else
    cv::Ptr<cv::Subdiv2D> subDivide;
    cv::Ptr<cv::CascadeClassifier> faceDetector;
    cv::Ptr<cv::face::Facemark> facemark;
#endif
    cv::Mat videoFrame, grayFrame;
};

#endif // LAUFACIALFEATUREDETECTORGLWIDGET_H
