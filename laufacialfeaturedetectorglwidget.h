#ifndef LAUFACIALFEATUREDETECTORGLWIDGET_H
#define LAUFACIALFEATUREDETECTORGLWIDGET_H

#include <QtCore>
#include <QObject>

#include "opencv2/face.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "lauvideoglwidget.h"

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
class LAUFacialFeatureDetectorGLWidget : public LAUVideoGLWidget
{

public:
    explicit LAUFacialFeatureDetectorGLWidget(QWidget *parent = nullptr);
    ~LAUFacialFeatureDetectorGLWidget();

    void initialize();
    void process();
    void paint();

private:
    QOpenGLBuffer faceVertexBuffer, faceIndexBuffer;
    QOpenGLFramebufferObject *frameBufferObject;
    QOpenGLShaderProgram programA, programB;

    cv::Mat videoFrame, grayFrame;
    cv::Ptr<cv::Subdiv2D> subDivide;
    cv::Ptr<cv::CascadeClassifier> faceDetector;
    cv::Ptr<cv::face::Facemark> facemark;
};

#endif // LAUFACIALFEATUREDETECTORGLWIDGET_H
