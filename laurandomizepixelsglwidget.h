#ifndef LAURANDOMIZEPIXELSGLWIDGET_H
#define LAURANDOMIZEPIXELSGLWIDGET_H

#include <QtCore>
#include <QObject>

#include "lauvideoglwidget.h"

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
class LAURandomizePixelsGLWidget : public LAUVideoGLWidget
{
    Q_OBJECT

public:
    explicit LAURandomizePixelsGLWidget(QWidget *parent = NULL) : LAUVideoGLWidget(parent), mapTexture(NULL), frameBufferObject(NULL)
    {
        ;
    }
    ~LAURandomizePixelsGLWidget();

    void initialize();
    void process();
    void paint();

    QImage grabImage();

private:
    QList<int> framesA, framesB;
    QOpenGLTexture *mapTexture;
    QOpenGLFramebufferObject *frameBufferObject;
    QOpenGLShaderProgram programA, programB;

    void buildMappingTexture(int cols, int rows);
};

#endif // LAURANDOMIZEPIXELSGLWIDGET_H
