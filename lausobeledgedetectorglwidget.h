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

public:
    explicit LAUSobelEdgeDetectorGLWidget(QWidget *parent = NULL) : LAUVideoGLWidget(parent), frameBufferObjects(NULL) { ; }
    ~LAUSobelEdgeDetectorGLWidget();

    void initialize();
    void process();
    void paint();

private:
    QOpenGLFramebufferObject *frameBufferObjects;
    QOpenGLShaderProgram programA;
};
#endif // LAUSOBELEDGEDETECTORGLWIDGET_H
