#ifndef LAUCALTAGGLWIDGET_H
#define LAUCALTAGGLWIDGET_H

#include <QtCore>
#include <QObject>

#include <QMenu>
#include <QLabel>
#include <QWidget>
#include <QSpinBox>
#include <QSettings>
#include <QGroupBox>
#include <QCheckBox>
#include <QFormLayout>
#include <QMouseEvent>

#include "opencv2/core/core.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "lauvideoglwidget.h"

namespace cv
{
    using std::vector;
}

#define CALTAGSCANGLFILTERITERATIONCOUNT                    1
#define CALTAGSCANGLFILTERQUANTIZATIONOFFSET                -0.01f
#define CALTAGSCANGLFILTERGAUSSIANFILTERRADIUS              10
#define CALTAGSCANGLFILTERMEDIANFILTERRADIUS                0
#define CALTAGSCANGLFILTERANGLETOLERANCE                    PI/9.0f
#define CALTAGSCANGLFILTERBESTFITTRANSFORMPOLYNOMIALORDER   2
#define CALTAGSCANGLFILTERDISTANCEFROMGRIDPOINTTHRESHOLD    0.10f

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
class LAUCalTagGLWidget : public LAUVideoGLWidget
{
    Q_OBJECT

public:
    explicit LAUCalTagGLWidget(QWidget *parent = NULL);
    ~LAUCalTagGLWidget();

    float offset() const
    {
        return (quantizationOffset);
    }

    unsigned int medianRadius() const
    {
        return (medianFilterRadius);
    }

    unsigned int gaussianRadius() const
    {
        return (gaussianFilterRadius);
    }

    unsigned int iterations() const
    {
        return (iterationCount);
    }

    void initialize();
    void process();
    void paint();

public slots:
    void onSetOffset(double val)
    {
        quantizationOffset = val;
    }

    void onSetMedianRadius(int val)
    {
        medianFilterRadius = val;
    }

    void onSetGaussianRadius(int val)
    {
        gaussianFilterRadius = val;
    }

    void onSetIterations(int val)
    {
        iterationCount = val;
    }

    void onSetMinRegionArea(int val)
    {
        minRegionArea = val;
    }

    void onSetMaxRegionArea(int val)
    {
        maxRegionArea = val;
    }

    void onSetMinBoxCount(int val)
    {
        minBoxCount = val;
    }

    void onSetFlipCalTagsFlag(bool val)
    {
        flipCalTagsFlag = val;
    }

    void onEnableTexture(bool state)
    {
        displayTextureFlag = state;
    }

private:
    typedef struct {
        QPoint cr;
        QPoint xy;
    } Pairing;

    QList<Pairing> gridPairings;            // KEEP TRACK OF THE LAST SET OF GRID POINT PAIRINGS

    float quantizationOffset;
    unsigned int medianFilterRadius;
    unsigned int gaussianFilterRadius;
    unsigned int iterationCount;

    int minRegionArea;     // MINIMUM REGION AREA
    int maxRegionArea;     // MAXIMUM REGION AREA
    int minBoxCount;       // MINIMUM BOX AREA
    bool flipCalTagsFlag;  // IS THE TARGET BACKLIT?
    bool displayTextureFlag;

    QByteArray memoryObject[2];
    QByteArray debugObject;

    QOpenGLShaderProgram programA, programB, programC;
    QOpenGLShaderProgram programD, programE, programF;
    QOpenGLShaderProgram programG, programH, programI;
    QOpenGLShaderProgram programJ, programK;

    QOpenGLTexture *textureLUT;

    QOpenGLFramebufferObject *frameBufferObjectA, *frameBufferObjectB;
    QOpenGLFramebufferObject *frameBufferObjectC, *frameBufferObjectD;

    void testFBO(QOpenGLFramebufferObject *fbo[]);

    void binarize(QOpenGLFramebufferObject *fboA, QOpenGLFramebufferObject *fboB, QOpenGLFramebufferObject *fboC);
    void sobel(QOpenGLFramebufferObject *fboA, QOpenGLFramebufferObject *fboB);
    void cleanUp(QOpenGLFramebufferObject *fboA, QOpenGLFramebufferObject *fboB);
    void dilationErosion(QOpenGLFramebufferObject *fboA, QOpenGLFramebufferObject *fboB);

    cv::Mat cleanStrays(cv::Mat inImage);
    cv::vector<cv::RotatedRect> regionArea(cv::Mat inImage);
    cv::vector<cv::Point2f> removeOutlierPoints(cv::vector<cv::Point2f> fmPoints, cv::vector<cv::Point2f> toPoints);
    cv::vector<cv::vector<cv::Point2f>> findSaddles(cv::vector<cv::RotatedRect> rotatedRects);
    cv::vector<cv::vector<cv::Point2f>> findPattern(cv::Mat image, cv::vector<cv::vector<cv::Point2f>> squares);

    cv::Mat detectCalTagGrid(bool *okay = NULL);
    cv::Mat findBestQuadraticMapping(cv::vector<cv::Point2f> fmPoints, cv::vector<cv::Point2f> toPoints, int order = 4);
    cv::Mat findBestLinearMapping(cv::vector<cv::Point2f> fmPoints, cv::vector<cv::Point2f> toPoints);

    static float length(cv::Point2f point)
    {
        return (sqrt(point.x * point.x + point.y * point.y));
    }

    static float angle(cv::Point2f point)
    {
        return (atan2(point.y, point.x));
    }

    static  void transform(cv::Mat inMat, cv::Mat outMat, cv::Mat transformMatrix);
    static  bool checkBitCode(int code, cv::Point2f *pt);
};

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
class LAUCalTagFilterWidget : public QWidget
{
    Q_OBJECT

public:
    LAUCalTagFilterWidget(LAUCalTagGLWidget *glwdgt, QWidget *parent = NULL);

public slots:

private:
    LAUCalTagGLWidget *glWidget;

    QSpinBox *iterSpinBox;
    QSpinBox *gausSpinBox;
    QSpinBox *mednSpinBox;
    QDoubleSpinBox *offsetSpinBox;

    QSpinBox *minRegionArea;     // MINIMUM REGION AREA
    QSpinBox *maxRegionArea;     // MAXIMUM REGION AREA
    QSpinBox *minBoxCount;       // MINIMUM BOX AREA
    QCheckBox *flipCalTagsFlag;  // IS THE TARGET BACKLIT?
};

#endif // LAUCALTAGGLWIDGET_H
