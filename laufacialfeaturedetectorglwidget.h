#ifndef LAUFACIALFEATUREDETECTORGLWIDGET_H
#define LAUFACIALFEATUREDETECTORGLWIDGET_H

#include <QMenu>
#include <QtCore>
#include <QAction>
#include <QObject>
#include <QSettings>
#include <QFileDialog>
#include <QMouseEvent>

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
    Q_OBJECT

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

public slots:
    void onLoadFaceImageFromDisk();

protected:
    void mousePressEvent(QMouseEvent *event)
    {
        if (event->button() == Qt::RightButton) {
            if (contextMenu) {
                contextMenu->popup(event->globalPos());
            }
        }
    }

private:
    QMenu *contextMenu;

    QOpenGLBuffer faceVertexBuffer, faceIndexBuffer;
    QOpenGLFramebufferObject *frameBufferObject;
    QOpenGLShaderProgram programA, programB;

    cv::Mat videoFrame, grayFrame;
    cv::Ptr<cv::Subdiv2D> subDivide;
    cv::Ptr<cv::CascadeClassifier> faceDetector;
    cv::Ptr<cv::face::Facemark> facemark;
    std::vector<cv::Point2f> templateList;
};

#endif // LAUFACIALFEATUREDETECTORGLWIDGET_H
