#include "laurandomizepixelsglwidget.h"
#include "locale.h"

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
LAURandomizePixelsGLWidget::~LAURandomizePixelsGLWidget()
{
    if (wasInitialized()) {
        makeCurrent();
        if (mapTexture) {
            delete mapTexture;
        }
        if (frameBufferObject) {
            delete frameBufferObject;
        }
    }
    qDebug() << "LAURandomizePixelsGLWidget::~LAURandomizePixelsGLWidget()";
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
void LAURandomizePixelsGLWidget::buildMappingTexture(int cols, int rows)
{
    // CHECK TO SEE IF THERE IS AN EXISTING TEXTURE THAT WE NEED TO DELETE
    if (mapTexture) {
        delete mapTexture;
    }

    // CREATE A NEW 3-D TEXTURE
    mapTexture = new QOpenGLTexture(QOpenGLTexture::Target3D);
    mapTexture->setSize(cols, rows, 16);
    mapTexture->setFormat(QOpenGLTexture::RG32F);
    mapTexture->setWrapMode(QOpenGLTexture::ClampToEdge);
    mapTexture->setMinificationFilter(QOpenGLTexture::Linear);
    mapTexture->setMagnificationFilter(QOpenGLTexture::Linear);
    mapTexture->allocateStorage();

    // CREATE A LIST OF AVAILABLE PIXELS
    QList<QPoint> pointsA, pointsB;
    for (int c = 0; c < cols; c++) {
        for (int r = 0; r < rows; r++) {
            pointsA << QPoint(c, r);
        }
    }

    // NOW SORT THE PIXELS IN RANDOM ORDER FRAME BY FRAME
    int index = 0;
    unsigned short *buffer = (unsigned short *)malloc(mapTexture->width() * mapTexture->height() * mapTexture->depth() * sizeof(unsigned short) * 2);
    for (int s = 0; s < mapTexture->depth(); s++) {
        framesA << s;
        if (pointsA.count() > pointsB.count()) {
            while (pointsA.isEmpty() == false) {
                int index = qFloor((double)rand() / (double)RAND_MAX * (double)pointsA.count());
                pointsB << pointsA.takeAt(index);
            }
            for (int n = 0; n < pointsB.count(); n++) {
                buffer[index++] = (unsigned short)pointsB.at(n).x();
                buffer[index++] = (unsigned short)pointsB.at(n).y();
            }
        } else {
            while (pointsB.isEmpty() == false) {
                int index = qFloor((double)rand() / (double)RAND_MAX * (double)pointsB.count());
                pointsA << pointsB.takeAt(index);
            }
            for (int n = 0; n < pointsA.count(); n++) {
                buffer[index++] = (unsigned short)pointsA.at(n).x();
                buffer[index++] = (unsigned short)pointsA.at(n).y();
            }
        }
    }

    // UPLOAD THE RANDOM INDICES TO THE GPU TEXTURE
    QOpenGLPixelTransferOptions options;
    options.setAlignment(1);
    mapTexture->setData(QOpenGLTexture::RG, QOpenGLTexture::UInt16, (const void *)buffer, &options);

    // DELETE THE TEMPORARY BUFFER
    free(buffer);
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
QImage LAURandomizePixelsGLWidget::grabImage()
{
    if (frameBufferObject) {
        return (frameBufferObject->toImage());
    }
    return (QImage());
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
void LAURandomizePixelsGLWidget::initialize()
{
    LAUVideoGLWidget::initialize();

    // NOW ADD OUR LIST OF HARRIS CORNER SHADER PROGRAMS
    setlocale(LC_NUMERIC, "C");
    programA.addShaderFromSourceFile(QOpenGLShader::Vertex,   ":/shaders/Shaders/filterRandomMappingA.vert");
    programA.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/Shaders/filterRandomMappingA.frag");
    programA.link();

    programB.addShaderFromSourceFile(QOpenGLShader::Vertex,   ":/shaders/Shaders/filterRandomMappingB.vert");
    programB.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/Shaders/filterRandomMappingB.frag");
    programB.link();
    setlocale(LC_ALL, "");
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
void LAURandomizePixelsGLWidget::process()
{
    if (framesA.isEmpty()) {
        framesA = framesB;
        framesB.clear();
    }

    // SEE IF WE NEED NEW FBOS
    if (videoTexture) {
        if (frameBufferObject == NULL) {
            // CREATE A FORMAT OBJECT FOR CREATING THE FRAME BUFFER
            QOpenGLFramebufferObjectFormat frameBufferObjectFormat;
            frameBufferObjectFormat.setInternalTextureFormat(GL_RGBA32F);

            frameBufferObject = new QOpenGLFramebufferObject(videoTexture->width(), videoTexture->height(), frameBufferObjectFormat);
            frameBufferObject->release();
        } else if (frameBufferObject->width() != videoTexture->width() || frameBufferObject->height() != videoTexture->height()) {
            delete frameBufferObject;

            // CREATE A FORMAT OBJECT FOR CREATING THE FRAME BUFFER
            QOpenGLFramebufferObjectFormat frameBufferObjectFormat;
            frameBufferObjectFormat.setInternalTextureFormat(GL_RGBA32F);

            frameBufferObject = new QOpenGLFramebufferObject(videoTexture->width(), videoTexture->height(), frameBufferObjectFormat);
            frameBufferObject->release();
        }

        // CHECK TO SEE IF WE NEED TO INITIALIZE THE MAPPING TEXTURE
        if (mapTexture == NULL || mapTexture->width() != videoTexture->width() || mapTexture->height() != videoTexture->height()) {
            buildMappingTexture(videoTexture->width(), videoTexture->height());
        }

        // SET CLEAR COLOR AS NOT A NUMBERS
        glClearColor(NAN, NAN, NAN, NAN);

        // CALCULATE THE GRADIENT BUFFER
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

                        glActiveTexture(GL_TEXTURE1);
                        mapTexture->bind();
                        programA.setUniformValue("qt_map", 1);

                        // SET THE LAYER IN THE RANDOM MAPPING TEXTURE
                        int index = qFloor((double)rand() / (double)RAND_MAX * framesA.count());
                        programA.setUniformValue("qt_index", framesA.at(index));
                        framesB << framesA.takeAt(index);

                        // TELL OPENGL PROGRAMMABLE PIPELINE HOW TO LOCATE VERTEX POSITION DATA
                        glVertexAttribPointer(programA.attributeLocation("qt_vertex"), 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
                        programA.enableAttributeArray("qt_vertex");
                        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

                        // RELEASE THE FRAME BUFFER OBJECT AND ITS ASSOCIATED GLSL PROGRAMS
                        quadIndexBuffer.release();
                    }
                    quadVertexBuffer.release();
                }
                programA.release();
            }
            frameBufferObject->release();
        }
        update();
    }
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
void LAURandomizePixelsGLWidget::paint()
{
    if (frameBufferObject == NULL) {
        LAUVideoGLWidget::paint();
    } else {
        // SET THE VIEW PORT AND CLEAR THE SCREEN BUFFER
        glViewport(0, 0, localWidth, localHeight);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // DISPLAY THE LAST FBO IN OUR LIST
        if (videoTexture) {
            if (programB.bind()) {
                if (quadVertexBuffer.bind()) {
                    if (quadIndexBuffer.bind()) {
                        // SET THE ACTIVE TEXTURE ON THE GPU
                        glActiveTexture(GL_TEXTURE0);
                        videoTexture->bind();
                        programB.setUniformValue("qt_textureA", 0);

                        glActiveTexture(GL_TEXTURE1);
                        glBindTexture(GL_TEXTURE_2D, frameBufferObject->texture());
                        programB.setUniformValue("qt_textureB", 1);

                        // TELL OPENGL PROGRAMMABLE PIPELINE HOW TO LOCATE VERTEX POSITION DATA
                        programB.setAttributeBuffer("qt_vertex", GL_FLOAT, 0, 4, 4 * sizeof(float));
                        programB.enableAttributeArray("qt_vertex");

                        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

                        quadIndexBuffer.release();
                    }
                    quadVertexBuffer.release();
                }
                programB.release();
            }
        }
    }
}
