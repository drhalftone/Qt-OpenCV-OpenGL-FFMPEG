#ifndef LAUHARRISCORNERDETECTORGLWIDGET_H
#define LAUHARRISCORNERDETECTORGLWIDGET_H

#include <QtCore>
#include <QObject>

#include "lauvideoglwidget.h"

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
class LAUHarrisCornerDetectorGLWidget : public LAUVideoGLWidget
{

public:
    explicit LAUHarrisCornerDetectorGLWidget(QWidget *parent = NULL) : LAUVideoGLWidget(parent)
    {
        qtScaleFactor = 0.05f;
        qtCornerThreshold = 0.01f;
        for (int n = 0; n < 5; n++) {
            frameBufferObjects[n] = NULL;
        }
    }
    ~LAUHarrisCornerDetectorGLWidget();

    QImage grabImage()
    {
        if (frameBufferObjects[4]) {
            return (frameBufferObjects[4]->toImage());
        }
        return (QImage());
    }

    void setScaleFactor(float val)
    {
        qtScaleFactor = val;
    }

    void setCornerThreshold(float val)
    {
        qtCornerThreshold = val;
    }

    void initialize();
    void process();
    void paint();

private:
    float qtScaleFactor, qtCornerThreshold;
    QOpenGLFramebufferObject *frameBufferObjects[5];
    QOpenGLShaderProgram programA, programB, programC, programD, programE;
};

#endif // LAUHARRISCORNERDETECTORGLWIDGET_H
