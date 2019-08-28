#ifndef LAUSOBELEDGEDETECTORGLWIDGET_H
#define LAUSOBELEDGEDETECTORGLWIDGET_H

#include <QtCore>
#include <QObject>

#include "lauvideoglwidget.h"

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
class LAUSobelEdgeDetectorGLWidget : public LAUVideoGLWidget
{
    Q_OBJECT

public:
    explicit LAUSobelEdgeDetectorGLWidget(QWidget *parent = NULL) : LAUVideoGLWidget(parent), frameBufferObject(NULL) { ; }
    ~LAUSobelEdgeDetectorGLWidget();

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
    QOpenGLFramebufferObject *frameBufferObject;
    QOpenGLShaderProgram programA;
};
#endif // LAUSOBELEDGEDETECTORGLWIDGET_H
