#ifndef LAUHISTOGRAMEQUALIZATIONGLWIDGET_H
#define LAUHISTOGRAMEQUALIZATIONGLWIDGET_H
#include <QtCore>
#include <QObject>

#include "lauvideoglwidget.h"
#include "laumemoryobject.h"

#define CALIBRATIONHISTOGRAMLENGTH 256

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
class LAUHistogramEqualizationGLWidget : public LAUVideoGLWidget
{
    Q_OBJECT

public:
    explicit LAUHistogramEqualizationGLWidget(QWidget *parent = NULL) : LAUVideoGLWidget(parent), frameBufferObjectA(nullptr), frameBufferObjectB(nullptr), histogramTexture(nullptr) { ; }
    ~LAUHistogramEqualizationGLWidget();

    void initialize();
    void process();
    void paint();

    QImage grabImage()
    {
        if (frameBufferObjectB) {
            return (frameBufferObjectB->toImage());
        }
        return (QImage());
    }

private:
    LAUMemoryObject histogramObject;
    QOpenGLFramebufferObject *frameBufferObjectA, *frameBufferObjectB;
    QOpenGLTexture *histogramTexture;
    QOpenGLShaderProgram programA, programB;
    QOpenGLBuffer pixlVertexBuffer, pixlIndexBuffer;
};

#endif // LAUHISTOGRAMEQUALIZATIONGLWIDGET_H
