#include "laufacialfeaturedetectorglwidget.h"
#include "locale.h"

using namespace std;
using namespace cv;
using namespace cv::face;

#ifdef USEVISAGE
using namespace VisageSDK;
#endif

int groupList[68] = {15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 4, 14, 4, 14, 4, 4, 14, 4, 14, 4, 14, 14, 14, 14, 9, 9, 9, 9, 9, 3, 12, 12, 3, 12, 12, 3, 12, 12, 3, 12, 12, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 2, 2, 2, 2, 2, 2, 2, 2};
int indexListA[68] = {2, 4, 6, 8, 10, 12, 14, 16, 17, 15, 13, 11, 9, 7, 5, 3, 1, 6, 4, 4, 2, 2, 1, 1, 3, 3, 5, 25, 24, 23, 22, 2, 4, 15, 5, 1, 12, 10, 6, 8, 8, 12, 11, 9, 5, 7, 7, 11, 4, 6, 9, 1, 10, 5, 3, 3, 2, 2, 2, 4, 5, 7, 2, 6, 4, 8, 3, 9};
int indexListB[68] = {2, 4, 6, 8, 10, 12, 14, 16, 17, 15, 13, 11, 9, 7, 5, 3, 1, 6, 4, 4, 2, 2, 1, 1, 3, 3, 5, 25, 24, 23, 22, 2, 4, 15, 5, 1, 12, 10, 6, 8, 8, 12, 11, 9, 5, 7, 7, 11, 4, 6, 9, 1, 10, 5, 3, 7, 7, 2, 8, 8, 5, 7, 2, 6, 4, 8, 3, 9};

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
LAUFacialFeatureDetectorGLWidget::LAUFacialFeatureDetectorGLWidget(QWidget *parent) : LAUVideoGLWidget(parent), frameBufferObject(nullptr), visageTracker(nullptr)
{
#ifdef USEVISAGE
    inputImage = nullptr;

    QSettings settings;
    QString directory = settings.value("LAUFacialFeatureDetectorGLWidget::licenseKey", QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)).toString();
    QString string = QFileDialog::getOpenFileName(nullptr, QString("Load license key..."), directory, QString("*.vlc"));
    if (string.isEmpty() == false) {
        settings.setValue("LAUFacialFeatureDetectorGLWidget::licenseKey", QFileInfo(string).absolutePath());

        string = QFileInfo(string).path();
        initializeLicenseManager(string.toLatin1());

        string.append("/data/Facial Features Tracker - Low.cfg");
        visageTracker = new VisageTracker(string.toUtf8());

        VisageConfiguration configuration = visageTracker->getTrackerConfiguration();
        //configuration.setMaxFaceScale(1.0f);
        //configuration.setMinFaceScale(0.1f);
        //configuration.setFaceDetectorSensitivity(0.1f);
        //configuration.enableVNN();
        visageTracker->setTrackerConfiguration(configuration);
    }
#else
    faceDetector = nullptr;
    subDivide = nullptr;

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
#endif
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
#ifdef USEVISAGE
        if (inputImage) {
            vsReleaseImageHeader(&inputImage);
        }
        if (visageTracker) {
            delete visageTracker;
        }
#else
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
#endif
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
        static int frameCounter = 0;

        // INITIALIZE THE FRAME BUFFER OBJECT BASED ON THE INCOMING TEXTURE SIZE
        if (frameBufferObject == nullptr) {
            // CREATE A FORMAT OBJECT FOR CREATING THE FRAME BUFFER
            QOpenGLFramebufferObjectFormat frameBufferObjectFormat;
            frameBufferObjectFormat.setInternalTextureFormat(GL_RGBA32F);

            // CREATE A NEW FRAME BUFFER OBJECT
            frameBufferObject = new QOpenGLFramebufferObject(videoTexture->width(), videoTexture->height(), frameBufferObjectFormat);
            frameBufferObject->release();

#ifdef USEVISAGE
            // CREATE A VISAGE IMAGE FOR HOLDING THE GRAYSCALE FRAME ON THE CPU
            inputImage = vsCreateImageHeader(vsSize(videoTexture->width(), videoTexture->height()), 8, 1);
#endif
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

#ifdef USEVISAGE
            // CREATE A VISAGE IMAGE FOR HOLDING THE GRAYSCALE FRAME ON THE CPU
            if (inputImage) {
                vsReleaseImageHeader(&inputImage);
            }
            inputImage = vsCreateImageHeader(vsSize(videoTexture->width(), videoTexture->height()), 8, 1);
#endif
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
                        programA.setUniformValue("qt_flip", true);

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

#ifdef USEVISAGE
            // COPY FRAME BUFFER TEXTURE FROM GPU TO LOCAL CPU BUFFER
            glBindTexture(GL_TEXTURE_2D, frameBufferObject->texture());
            glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_UNSIGNED_BYTE, grayFrame.data);

            videoTexture->bind();
            glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, videoFrame.data);

            if (visageTracker) {
                int *numFaces = visageTracker->track(grayFrame.cols, grayFrame.rows, (const char *)grayFrame.data, faceData, VISAGE_FRAMEGRABBER_FMT_LUMINANCE, VISAGE_FRAMEGRABBER_ORIGIN_TL, 0, -1, 1);
                if (frameCounter > 2 && numFaces[0] == TRACK_STAT_OK) {
                    // CREATE A VECTOR TO HOLD THE LANDMARKS FOR EACH DETECTED FACE
                    vector<Point2f> landmarks(68);

                    for (int n = 0; n < 68; n++) {
                        FeaturePoint fpA = faceData[0].featurePoints2D->getFP(groupList[n], indexListA[n]);
                        FeaturePoint fpB = faceData[0].featurePoints2D->getFP(groupList[n], indexListB[n]);
                        float col = (fpA.pos[0] + fpB.pos[0]) / 2.0f * (float)videoFrame.cols;
                        float row = (fpA.pos[1] + fpB.pos[1]) / 2.0f * (float)videoFrame.rows;
                        landmarks.at(n) = Point2f(col, row);
                    }

                    for (int n = 0; n < landmarks.size(); n++) {
                        int col = landmarks[n].x;
                        int row = landmarks[n].y;
                        circle(videoFrame, Point(col, row), 2, Scalar(0, 255, 0), -1);
                    }

                    //for (int group = 0; group < 14; group++){
                    //    // DRAW THE EYE FIDUCIALS ON THE VIDEO FRAME
                    //    int numFiducials = faceData[0].featurePoints2D->groupSize(group+1);
                    //    for (int n = 0; n < numFiducials; n++){
                    //        FeaturePoint fp = faceData[0].featurePoints2D->getFP(group+1,n+1);
                    //        int col = fp.pos[0] * videoFrame.cols;
                    //        int row = fp.pos[1] * videoFrame.rows;
                    //        circle(videoFrame, Point(col, row), 2, Scalar(0, 255, 0), -1);
                    //    }
                    //}
                    videoTexture->setData(QOpenGLTexture::RGB, QOpenGLTexture::UInt8, (const void *)videoFrame.data);
                }
            }
#else
            // COPY FRAME BUFFER TEXTURE FROM GPU TO LOCAL CPU BUFFER
            glBindTexture(GL_TEXTURE_2D, frameBufferObject->texture());
            glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_UNSIGNED_BYTE, grayFrame.data);

            videoTexture->bind();
            glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, videoFrame.data);

            // CREATE A VECTOR OF RECTANGLES TO HOLD ONE RECTANGLE PER FACE
            vector<Rect> faces;
            faceDetector->detectMultiScale(grayFrame, faces);

            // LETS KEEP TRACK OF HOW MANY FACE TRIANGLES WE NEED TO DRAW LATER
            int numFaceTriangles = 0;

            // NEW SEE IF FOUND AT LEAST ONE FACE
            if (faces.size() > 0) {
                // CREATE A VECTOR TO HOLD THE LANDMARKS FOR EACH DETECTED FACE
                vector< vector<Point2f> > landmarks;
                bool success = facemark->fit(grayFrame, faces, landmarks);
                if (success) {
                    for (int n = 0; n < landmarks.size(); n++) {
                        // Draws voronoi diagram
                        vector< vector<Point2f> > facetList;
                        vector<Point2f> facetCenters;

                        // PREPARE THE SUBDIVIDE OBJECT FOR THE CURRENT FRAME SIZE
                        subDivide->initDelaunay(Rect(-videoTexture->width() / 2, -videoTexture->height() / 2, 2 * videoTexture->width(), 2 * videoTexture->height()));
                        for (int m = 0; m < landmarks.at(n).size(); m++) {
                            subDivide->insert(landmarks.at(n).at(m));
                        }

                        // PUSH THE FOUR CORNERS OF THE VIDEO TEXTURE
                        //subDivide->insert(Point2f(1, 1));
                        //subDivide->insert(Point2f(1, videoTexture->height() - 1));
                        //subDivide->insert(Point2f(videoTexture->width() - 1, 1));
                        //subDivide->insert(Point2f(videoTexture->width() - 1, videoTexture->height() - 1));

                        // DRAW THE FACE TRIANGLES ON THE VIDEO FRAME
                        std::vector< Vec6f > triangleList;
                        subDivide->getTriangleList(triangleList);
                        for (n = 0; n < triangleList.size(); n++) {
                            line(videoFrame, Point2f(triangleList.at(n)[0], triangleList.at(n)[1]), Point2f(triangleList.at(n)[2], triangleList.at(n)[3]), Scalar(0, 0, 255), 1, 8, 0);
                            line(videoFrame, Point2f(triangleList.at(n)[2], triangleList.at(n)[3]), Point2f(triangleList.at(n)[4], triangleList.at(n)[5]), Scalar(0, 0, 255), 1, 8, 0);
                            line(videoFrame, Point2f(triangleList.at(n)[4], triangleList.at(n)[5]), Point2f(triangleList.at(n)[0], triangleList.at(n)[1]), Scalar(0, 0, 255), 1, 8, 0);
                        }
                        videoTexture->setData(QOpenGLTexture::RGB, QOpenGLTexture::UInt8, (const void *)videoFrame.data);
                        numFaceTriangles = triangleList.size();

                        // COPY FACE TRIANGLE VERTICES TO THE GPU FOR DRAWING
                        if (numFaceTriangles > 0 && faceVertexBuffer.bind()) {
                            float *buffer = (float *)faceVertexBuffer.mapRange(0, 12 * numFaceTriangles * sizeof(float), QOpenGLBuffer::RangeWrite);
                            if (buffer) {
                                for (n = 0; n < triangleList.size(); n++) {
                                    // INSERT FIRST OF THREE VERTICES (INPUT AND OUTPUT POINTS)
                                    buffer[12 * n + 0] = triangleList.at(n)[0];
                                    buffer[12 * n + 1] = triangleList.at(n)[1];
                                    buffer[12 * n + 2] = triangleList.at(n)[0];
                                    buffer[12 * n + 3] = triangleList.at(n)[1];

                                    // INSERT SECOND OF THREE VERTICES (INPUT AND OUTPUT POINTS)
                                    buffer[12 * n + 4] = triangleList.at(n)[2];
                                    buffer[12 * n + 5] = triangleList.at(n)[3];
                                    buffer[12 * n + 6] = triangleList.at(n)[2];
                                    buffer[12 * n + 7] = triangleList.at(n)[3];

                                    // INSERT THIRD OF THREE VERTICES (INPUT AND OUTPUT POINTS)
                                    buffer[12 * n + 8] = triangleList.at(n)[4];
                                    buffer[12 * n + 9] = triangleList.at(n)[5];
                                    buffer[12 * n + 10] = triangleList.at(n)[4];
                                    buffer[12 * n + 11] = triangleList.at(n)[5];
                                }
                                faceVertexBuffer.unmap();
                            } else {
                                qDebug() << QString("faceVertexBuffer not mapped to CPU.") << glGetError();
                            }
                            faceVertexBuffer.release();
                        }
                    }
                }
            }

            // CHECK TO SEE IF WE HAVE ANY FACES TO DRAW
            if (numFaceTriangles > 0) {
                // CONVERT THE RGB TEXTURE INTO GRAYSCALE
                if (frameBufferObject->bind()) {
                    if (programB.bind()) {
                        // SET THE VIEWPOINT BUT DON'T CLEAR THE PREVIOUS CONTENTS OF THE BUFFER
                        glViewport(0, 0, frameBufferObject->width(), frameBufferObject->height());

                        // BIND VBOS FOR DRAWING TRIANGLES ON SCREEN
                        if (faceVertexBuffer.bind()) {
                            if (faceIndexBuffer.bind()) {
                                qDebug() << "Drawing face triangles" << numFaceTriangles;

                                // BIND THE INCOMING RGB VIDEO FRAME AS A TEXTURE
                                glActiveTexture(GL_TEXTURE0);
                                videoTexture->bind();
                                programB.setUniformValue("qt_texture", 0);

                                // TELL THE GLPROGRAM WHAT THE DIMENSIONS OF THE VIDEO FRAME
                                programB.setUniformValue("qt_size", QPointF(videoTexture->width(), videoTexture->height()));

                                // TELL OPENGL PROGRAMMABLE PIPELINE HOW TO LOCATE VERTEX POSITION DATA
                                glVertexAttribPointer(programB.attributeLocation("qt_vertex"), 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
                                programB.enableAttributeArray("qt_vertex");
                                glDrawElements(GL_TRIANGLES, 3 * numFaceTriangles, GL_UNSIGNED_INT, nullptr);

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
#endif
        }
        frameCounter++;
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
        faceVertexBuffer.allocate(200 * 12 * sizeof(float));
        faceVertexBuffer.release();
    }

    // CREATE INDEX BUFFER TO CONNECT VERTICES FOR FACIAL FEATURE TRIANGLES
    faceIndexBuffer = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    faceIndexBuffer.create();
    faceIndexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
    if (faceIndexBuffer.bind()) {
        faceIndexBuffer.allocate(200 * 3 * sizeof(unsigned int));
        unsigned int *indices = (unsigned int *)faceIndexBuffer.map(QOpenGLBuffer::WriteOnly);
        if (indices) {
            for (int n = 0; n < 200; n++) {
                indices[3 * n + 0] = 3 * n + 0;
                indices[3 * n + 1] = 3 * n + 1;
                indices[3 * n + 2] = 3 * n + 2;
            }
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
                        //glBindTexture(GL_TEXTURE_2D, frameBufferObject->texture());
                        videoTexture->bind();
                        program.setUniformValue("qt_texture", 0);
#ifdef Q_OS_WIN
                        program.setUniformValue("qt_flip", false);
#else
                        program.setUniformValue("qt_flip", true);
#endif
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
