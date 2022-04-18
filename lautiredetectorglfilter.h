#ifndef LAUTIREDETECTORGLFILTER_H
#define LAUTIREDETECTORGLFILTER_H

#include <QtCore>
#include <QObject>

#include "lauvideoglwidget.h"

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
class LAUTireDetectorGLWidget : public LAUVideoGLWidget
{
    Q_OBJECT

public:
    explicit LAUTireDetectorGLWidget(QWidget *parent = NULL) : LAUVideoGLWidget(parent), frameBufferObject(NULL) { ; }
    ~LAUTireDetectorGLWidget();

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

#endif // LAUTIREDETECTORGLFILTER_H
