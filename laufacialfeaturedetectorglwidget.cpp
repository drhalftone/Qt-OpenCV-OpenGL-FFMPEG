#include "laufacialfeaturedetectorglwidget.h"
#include "locale.h"

#include <QSettings>
#include <QStandardPaths>
#include <QFileDialog>
#include <QFileInfo>

using namespace std;
using namespace cv;
using namespace cv::face;

#define NUMBEROFFACIALFEATURES    68
#define NUMBEROFFACIALTRIANGLES  107
unsigned int triangles[3 * NUMBEROFFACIALTRIANGLES] = { 67, 58, 59, 31, 32, 49, 58, 57, 7, 52, 34, 35, 45, 44, 25, 40, 39, 29, 18, 37, 36, 42, 22, 43, 59, 48, 60, 1, 36, 41, 61, 50, 51, 52, 53, 63, 65, 56, 66, 67, 61, 62, 55, 10, 56, 57, 9, 8, 27, 28, 39, 33, 52, 51, 53, 55, 65, 65, 66, 62, 42, 28, 27, 35, 46, 14, 20, 37, 19, 19, 37, 18, 0, 36, 1, 17, 18, 36, 20, 38, 37, 20, 21, 38, 38, 21, 39, 43, 23, 44, 34, 30, 35, 39, 21, 27, 29, 28, 42, 29, 39, 28, 33, 30, 34, 29, 31, 40, 30, 29, 35, 30, 32, 31, 17, 36, 0, 49, 48, 31, 30, 31, 29, 41, 2, 1, 48, 3, 2, 31, 48, 2, 59, 58, 6, 48, 4, 3, 48, 5, 4, 48, 59, 5, 5, 59, 6, 67, 66, 58, 58, 7, 6, 60, 49, 59, 33, 32, 30, 59, 49, 67, 60, 48, 49, 41, 31, 2, 41, 40, 31, 49, 61, 67, 49, 32, 50, 33, 50, 32, 61, 49, 50, 63, 62, 51, 62, 61, 51, 52, 63, 51, 66, 67, 62, 35, 53, 52, 65, 62, 63, 66, 57, 58, 56, 9, 57, 57, 8, 7, 66, 56, 57, 65, 63, 53, 65, 55, 56, 53, 64, 55, 56, 10, 9, 55, 11, 10, 54, 64, 53, 64, 54, 55, 35, 54, 53, 54, 11, 55, 14, 54, 35, 13, 12, 54, 54, 12, 11, 47, 29, 42, 54, 14, 13, 46, 15, 14, 52, 33, 34, 51, 50, 33, 22, 42, 27, 47, 35, 29, 22, 23, 43, 46, 35, 47, 21, 22, 27, 24, 44, 23, 25, 44, 24, 16, 45, 26, 15, 46, 45, 45, 25, 26, 45, 16, 15 };

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
LAUFacialFeatureDetectorGLWidget::LAUFacialFeatureDetectorGLWidget(QWidget *parent) : LAUVideoGLWidget(parent), contextMenu(nullptr), frameBufferObject(nullptr), faceDetector(nullptr), subDivide(nullptr)
{
    QSettings settings;
    QString directory = settings.value("LAUFacialFeatureDetectorGLWidget::faceDetectorModel", QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)).toString();
    QString string = QFileDialog::getOpenFileName(nullptr, QString("Load classifier..."), directory, QString("*.xml"));
    if (string.isEmpty() == false) {
        settings.setValue("LAUFacialFeatureDetectorGLWidget::faceDetectorModel", QFileInfo(string).absolutePath());

        faceDetector = new CascadeClassifier();
        if (faceDetector->load(string.toStdString())) {
            QString directory = settings.value("LAUFacialFeatureDetectorGLWidget::faceMarkModel", QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)).toString();
            QString string = QFileDialog::getOpenFileName(nullptr, QString("Load classifier..."), directory, QString("*.yaml"));
            if (string.isEmpty() == false) {
                settings.setValue("LAUFacialFeatureDetectorGLWidget::faceMarkModel", QFileInfo(string).absolutePath());

                facemark = FacemarkLBF::create();
                facemark->loadModel(string.toStdString());

                // CREATE SUBDIVIDE OBJECT FOR EXTRACTING VORONOI DIAGRAM
                subDivide = new Subdiv2D();
            }
        }
    }

    contextMenu = new QMenu(QString("Load Template Face"));
    QAction *action = contextMenu->addAction(QString("Load Template Face"));
    connect(action, SIGNAL(triggered()), this, SLOT(onLoadFaceImageFromDisk()));
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
LAUFacialFeatureDetectorGLWidget::~LAUFacialFeatureDetectorGLWidget()
{
    if (wasInitialized()) {
        makeCurrent();
        if (frameBufferObject) {
            delete frameBufferObject;
        }
        if (faceDetector) {
            faceDetector.release();
            delete faceDetector;
        }
        if (facemark) {
            facemark.release();
            delete facemark;
        }
        if (subDivide) {
            subDivide.release();
            delete subDivide;
        }
    }

    if (contextMenu) {
        delete contextMenu;
    }

    qDebug() << "LAUFacialFeatureDetectorGLWidget::~LAUFacialFeatureDetectorGLWidget()";
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
void LAUFacialFeatureDetectorGLWidget::process()
{
    // SEE IF WE NEED NEW FBOS
    if (videoTexture) {
        // INITIALIZE THE FRAME BUFFER OBJECT BASED ON THE INCOMING TEXTURE SIZE
        if (frameBufferObject == nullptr) {
            // CREATE A FORMAT OBJECT FOR CREATING THE FRAME BUFFER
            QOpenGLFramebufferObjectFormat frameBufferObjectFormat;
            frameBufferObjectFormat.setInternalTextureFormat(GL_RGBA32F);

            // CREATE A NEW FRAME BUFFER OBJECT
            frameBufferObject = new QOpenGLFramebufferObject(videoTexture->width(), videoTexture->height(), frameBufferObjectFormat);
            frameBufferObject->release();

            // CREATE A OPENCV MATRIX FOR HOLDING THE GRAYSCALE FRAME ON THE CPU
            videoFrame = Mat(videoTexture->height(), videoTexture->width(), CV_8UC3);
            grayFrame = Mat(videoTexture->height(), videoTexture->width(), CV_8U);
        } else if (frameBufferObject->width() != videoTexture->width() || frameBufferObject->height() != videoTexture->height()) {
            // DELETE THE OLD FRAMEBUFFEROBJECT BECAUSE IT IS NO LONGER THE CORRECT SIZE
            delete frameBufferObject;

            // CREATE A FORMAT OBJECT FOR CREATING THE FRAME BUFFER
            QOpenGLFramebufferObjectFormat frameBufferObjectFormat;
            frameBufferObjectFormat.setInternalTextureFormat(GL_RGBA32F);

            // CREATE A NEW FRAME BUFFER OBJECT
            frameBufferObject = new QOpenGLFramebufferObject(videoTexture->width(), videoTexture->height(), frameBufferObjectFormat);
            frameBufferObject->release();

            // CREATE A OPENCV MATRIX FOR HOLDING THE GRAYSCALE FRAME ON THE CPU
            videoFrame = Mat(videoTexture->height(), videoTexture->width(), CV_8UC3);
            grayFrame = Mat(videoTexture->height(), videoTexture->width(), CV_8U);
        }

        // SET CLEAR COLOR AS NOT A NUMBERS
        glClearColor(NAN, NAN, NAN, NAN);

        // CONVERT THE RGB TEXTURE INTO GRAYSCALE
        if (frameBufferObject->bind()) {
            if (programA.bind()) {
                // CLEAR THE FRAME BUFFER OBJECT
                glViewport(0, 0, frameBufferObject->width(), frameBufferObject->height());
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                // BIND VBOS FOR DRAWING TRIANGLES ON SCREEN
                if (quadVertexBuffer.bind()) {
                    if (quadIndexBuffer.bind()) {
                        // BIND THE TEXTURE FROM THE ORIGINAL SCAN
                        glActiveTexture(GL_TEXTURE0);
                        videoTexture->bind();
                        programA.setUniformValue("qt_texture", 0);

                        // TELL OPENGL PROGRAMMABLE PIPELINE HOW TO LOCATE VERTEX POSITION DATA
                        glVertexAttribPointer(programA.attributeLocation("qt_vertex"), 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
                        programA.enableAttributeArray("qt_vertex");
                        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

                        // RELEASE THE FRAME BUFFER OBJECT AND ITS ASSOCIATED GLSL PROGRAMS
                        quadIndexBuffer.release();
                    }
                    quadVertexBuffer.release();
                }
                programA.release();
            }
            frameBufferObject->release();

            // COPY FRAME BUFFER TEXTURE FROM GPU TO LOCAL CPU BUFFER
            glBindTexture(GL_TEXTURE_2D, frameBufferObject->texture());
            glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_UNSIGNED_BYTE, grayFrame.data);

            //videoTexture->bind();
            //glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, videoFrame.data);

            // CREATE A VECTOR OF RECTANGLES TO HOLD ONE RECTANGLE PER FACE
            vector<Rect> faces;
            faceDetector->detectMultiScale(grayFrame, faces);

            // LETS KEEP TRACK OF HOW MANY FACE TRIANGLES WE NEED TO DRAW LATER
            int numLandmarks = 0;

            // NEW SEE IF FOUND AT LEAST ONE FACE
            if (faces.size() > 0) {
                // CREATE A VECTOR TO HOLD THE LANDMARKS FOR EACH DETECTED FACE
                vector< vector<Point2f> > landmarks;
                bool success = facemark->fit(grayFrame, faces, landmarks);
                if (success) {
                    videoTexture->setData(QOpenGLTexture::RGB, QOpenGLTexture::UInt8, (const void *)videoFrame.data);

                    // GET A COPY OF THE JUST DETECTED FACIAL FEATURE COORDINATES
                    vector<Point2f> features = landmarks.at(0);

                    // GET THE NUMBER OF LANDMARKS BETWEEN THIS AND TEMPLATE
                    numLandmarks = qMin(features.size(), templateList.size());

                    // COPY FACE TRIANGLE VERTICES TO THE GPU FOR DRAWING
                    if (numLandmarks == NUMBEROFFACIALFEATURES && faceVertexBuffer.bind()) {
                        float *buffer = (float *)faceVertexBuffer.mapRange(0, 4 * NUMBEROFFACIALFEATURES * sizeof(float), QOpenGLBuffer::RangeWrite);
                        if (buffer) {
                            for (int n = 0; n < numLandmarks; n++) {
                                // INSERT FIRST OF THREE VERTICES (INPUT AND OUTPUT POINTS)
                                buffer[4 * n + 0] = features.at(n).x;
                                buffer[4 * n + 1] = features.at(n).y;
                                buffer[4 * n + 2] = templateList.at(n).x;
                                buffer[4 * n + 3] = templateList.at(n).y;
                            }
                            faceVertexBuffer.unmap();
                        } else {
                            qDebug() << QString("faceVertexBuffer not mapped to CPU.") << glGetError();
                        }
                        faceVertexBuffer.release();
                    }
                }
            }

            // CHECK TO SEE IF WE HAVE ANY FACES TO DRAW
            if (numLandmarks == NUMBEROFFACIALFEATURES) {
                // CONVERT THE RGB TEXTURE INTO GRAYSCALE
                if (frameBufferObject->bind()) {
                    if (programB.bind()) {
                        // SET THE VIEWPOINT BUT DON'T CLEAR THE PREVIOUS CONTENTS OF THE BUFFER
                        glViewport(0, 0, frameBufferObject->width(), frameBufferObject->height());

                        // BIND VBOS FOR DRAWING TRIANGLES ON SCREEN
                        if (faceVertexBuffer.bind()) {
                            if (faceIndexBuffer.bind()) {
                                qDebug() << "Drawing face triangles" << numLandmarks;

                                // BIND THE INCOMING RGB VIDEO FRAME AS A TEXTURE
                                glActiveTexture(GL_TEXTURE0);
                                videoTexture->bind();
                                programB.setUniformValue("qt_texture", 0);

                                // TELL THE GLPROGRAM WHAT THE DIMENSIONS OF THE VIDEO FRAME
                                programB.setUniformValue("qt_size", QPointF(videoTexture->width(), videoTexture->height()));

                                // TELL OPENGL PROGRAMMABLE PIPELINE HOW TO LOCATE VERTEX POSITION DATA
                                glVertexAttribPointer(programB.attributeLocation("qt_vertex"), 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
                                programB.enableAttributeArray("qt_vertex");
                                glDrawElements(GL_TRIANGLES, 3 * NUMBEROFFACIALTRIANGLES, GL_UNSIGNED_INT, nullptr);

                                // RELEASE THE FRAME BUFFER OBJECT AND ITS ASSOCIATED GLSL PROGRAMS
                                faceIndexBuffer.release();
                            }
                            faceVertexBuffer.release();
                        }
                        programB.release();
                    }
                    frameBufferObject->release();
                }
            }
        }
    }
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
void LAUFacialFeatureDetectorGLWidget::initialize()
{
    // INITIALIZE THE UNDERLYING CLASS
    LAUVideoGLWidget::initialize();

    // CREATE VERTEX BUFFER TO HOLD FACIAL FEATURE TRIANGLES
    faceVertexBuffer = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    faceVertexBuffer.create();
    faceVertexBuffer.setUsagePattern(QOpenGLBuffer::DynamicDraw);
    if (faceVertexBuffer.bind()) {
        // ALLOCATE THE VERTEX BUFFER FOR HOLDING ENOUGH VEC4 FOR TWO FACES (SOURCE AND DESTINATION POINTS)
        faceVertexBuffer.allocate(NUMBEROFFACIALFEATURES * 4 * sizeof(float));
        faceVertexBuffer.release();
    }

    // CREATE INDEX BUFFER TO CONNECT VERTICES FOR FACIAL FEATURE TRIANGLES
    faceIndexBuffer = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    faceIndexBuffer.create();
    faceIndexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    if (faceIndexBuffer.bind()) {
        faceIndexBuffer.allocate(339 * sizeof(unsigned int));
        unsigned int *indices = (unsigned int *)faceIndexBuffer.map(QOpenGLBuffer::WriteOnly);
        if (indices) {
            memcpy(indices, triangles, 3 * NUMBEROFFACIALTRIANGLES * sizeof(unsigned int));
            faceIndexBuffer.unmap();
        } else {
            qDebug() << QString("indiceBufferA buffer mapped from GPU.");
        }
        faceIndexBuffer.release();
    }

    // NOW ADD OUR LIST OF HARRIS CORNER SHADER PROGRAMS
    setlocale(LC_NUMERIC, "C");
    programA.addShaderFromSourceFile(QOpenGLShader::Vertex,   ":/shaders/Shaders/filterRGBAtoGray.vert");
    programA.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/Shaders/filterRGBAtoGray.frag");
    programA.link();

    programB.addShaderFromSourceFile(QOpenGLShader::Vertex,   ":/shaders/Shaders/filterDrawFace.vert");
    programB.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/Shaders/filterDrawFace.frag");
    programB.link();
    setlocale(LC_ALL, "");
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
void LAUFacialFeatureDetectorGLWidget::paint()
{
    if (frameBufferObject == nullptr) {
        LAUVideoGLWidget::paint();
    } else {
        // SET THE VIEW PORT AND CLEAR THE SCREEN BUFFER
        glViewport(0, 0, localWidth, localHeight);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // DISPLAY THE LAST FBO IN OUR LIST
        if (videoTexture) {
            if (program.bind()) {
                if (quadVertexBuffer.bind()) {
                    if (quadIndexBuffer.bind()) {
                        // SET THE ACTIVE TEXTURE ON THE GPU
                        glActiveTexture(GL_TEXTURE0);
                        glBindTexture(GL_TEXTURE_2D, frameBufferObject->texture());
                        program.setUniformValue("qt_texture", 0);

                        // TELL OPENGL PROGRAMMABLE PIPELINE HOW TO LOCATE VERTEX POSITION DATA
                        program.setAttributeBuffer("qt_vertex", GL_FLOAT, 0, 4, 4 * sizeof(float));
                        program.enableAttributeArray("qt_vertex");

                        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

                        quadIndexBuffer.release();
                    }
                    quadVertexBuffer.release();
                }
                program.release();
            }
        }
    }
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
void LAUFacialFeatureDetectorGLWidget::onLoadFaceImageFromDisk()
{
    QSettings settings;
    QString directory = settings.value("LAUFacialFeatureDetectorGLWidget::lastUsedDirectory", QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)).toString();
    QString filename = QFileDialog::getOpenFileName(0, QString("Load scan from disk (*.tif)"), directory, QString("*.tif;*.tiff"));
    if (filename.isEmpty() == false) {
        settings.setValue("LAUFacialFeatureDetectorGLWidget::lastUsedDirectory", QFileInfo(filename).absolutePath());
    } else {
        return;
    }

    QImage image(filename);
    if (image.isNull() == false) {
        image = image.scaled(videoFrame.cols, videoFrame.rows);
        image = image.convertToFormat(QImage::Format_RGB888);
        for (int row = 0; row < videoFrame.rows; row++) {
            for (int col = 0; col < videoFrame.cols; col++) {
                memcpy(videoFrame.ptr(row), image.constScanLine(row), qMin(static_cast<int>(videoFrame.cols * videoFrame.elemSize()), static_cast<int>(image.bytesPerLine())));
            }
        }

        image = image.convertToFormat(QImage::Format_Grayscale8);
        for (int row = 0; row < grayFrame.rows; row++) {
            memcpy(grayFrame.ptr(row), image.constScanLine(row), qMin(static_cast<int>(grayFrame.cols * grayFrame.elemSize()), static_cast<int>(image.bytesPerLine())));
        }

        // CREATE A VECTOR OF RECTANGLES TO HOLD ONE RECTANGLE PER FACE
        vector<Rect> faces;
        faceDetector->detectMultiScale(grayFrame, faces);

        // NEW SEE IF FOUND EXACTLY ONE FACE
        if (faces.size() == 1) {
            // CREATE A VECTOR TO HOLD THE LANDMARKS FOR EACH DETECTED FACE
            vector< vector<Point2f> > landmarks;
            bool success = facemark->fit(grayFrame, faces, landmarks);
            if (success) {
                templateList = landmarks.at(0);
            }
        }
    }
}
