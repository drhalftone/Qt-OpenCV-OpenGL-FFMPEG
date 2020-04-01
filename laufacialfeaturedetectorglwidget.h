#ifndef LAUFACIALFEATUREDETECTORGLWIDGET_H
#define LAUFACIALFEATUREDETECTORGLWIDGET_H

#include <QtCore>
#include <QObject>
#include <QSettings>
#include <QFileDialog>

#ifdef USEVISAGE
#include "visageVision.h"
#include "VisageTracker.h"
#else
#include "opencv2/face.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#endif

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
        if (frameBufferObject) {
            return (frameBufferObject->toImage());
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
    VisageSDK::VisageFeaturesDetector *visageFeaturesDetector;
#else
    cv::Mat videoFrame, grayFrame;
    cv::Ptr<cv::Subdiv2D> subDivide;
    cv::Ptr<cv::CascadeClassifier> faceDetector;
    cv::Ptr<cv::face::Facemark> facemark;
#endif
};

#endif // LAUFACIALFEATUREDETECTORGLWIDGET_H
