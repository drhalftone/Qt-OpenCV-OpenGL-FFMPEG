#include "lauharriscornerdetectorglwidget.h"
#include "locale.h"

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
LAUHarrisCornerDetectorGLWidget::~LAUHarrisCornerDetectorGLWidget()
{
    if (wasInitialized()) {
        makeCurrent();
        for (int n = 0; n < 5; n++) {
            if (frameBufferObjects[n]) {
                delete frameBufferObjects[n];
            }
        }
    }
    qDebug() << "LAUHarrisCornerDetectorGLWidget::~LAUHarrisCornerDetectorGLWidget()";
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
void LAUHarrisCornerDetectorGLWidget::process()
{
    // SEE IF WE NEED NEW FBOS
    if (videoTexture) {
        for (int n = 0; n < 5; n++) {
            if (frameBufferObjects[n] == NULL) {
                // CREATE A FORMAT OBJECT FOR CREATING THE FRAME BUFFER
                QOpenGLFramebufferObjectFormat frameBufferObjectFormat;
                frameBufferObjectFormat.setInternalTextureFormat(GL_RGBA32F);

                frameBufferObjects[n] = new QOpenGLFramebufferObject(videoTexture->width(), videoTexture->height(), frameBufferObjectFormat);
                frameBufferObjects[n]->release();
            } else if (frameBufferObjects[n]->width() != videoTexture->width() || frameBufferObjects[n]->height() != videoTexture->height()) {
                delete frameBufferObjects[n];

                // CREATE A FORMAT OBJECT FOR CREATING THE FRAME BUFFER
                QOpenGLFramebufferObjectFormat frameBufferObjectFormat;
                frameBufferObjectFormat.setInternalTextureFormat(GL_RGBA32F);

                frameBufferObjects[n] = new QOpenGLFramebufferObject(videoTexture->width(), videoTexture->height(), frameBufferObjectFormat);
                frameBufferObjects[n]->release();
            }
        }

        // SET CLEAR COLOR AS NOT A NUMBERS
        glClearColor(NAN, NAN, NAN, NAN);

        // CALCULATE THE GRADIENT BUFFER
        if (frameBufferObjects[0]->bind()) {
            if (programA.bind()) {
                // CLEAR THE FRAME BUFFER OBJECT
                glViewport(2, 2, frameBufferObjects[0]->width() - 4, frameBufferObjects[0]->height() - 4);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                // BIND VBOS FOR DRAWING TRIANGLES ON SCREEN
                if (quadVertexBuffer.bind()) {
                    if (quadIndexBuffer.bind()) {
                        // BIND THE TEXTURE FROM THE ORIGINAL SCAN
                        glActiveTexture(GL_TEXTURE0);
                        videoTexture->bind();
                        programA.setUniformValue("qt_texture", 0);

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
            frameBufferObjects[0]->release();
        }

        // SMOOTH THE GRADIENT BUFFER WITH A GAUSSIAN FILTER
        if (frameBufferObjects[1]->bind()) {
            if (programB.bind()) {
                // CLEAR THE FRAME BUFFER OBJECT
                glViewport(4, 4, frameBufferObjects[1]->width() - 8, frameBufferObjects[1]->height() - 8);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                // BIND VBOS FOR DRAWING TRIANGLES ON SCREEN
                if (quadVertexBuffer.bind()) {
                    if (quadIndexBuffer.bind()) {
                        // BIND THE TEXTURE FROM THE FRAME BUFFER OBJECT A
                        glActiveTexture(GL_TEXTURE1);
                        glBindTexture(GL_TEXTURE_2D, frameBufferObjects[0]->texture());
                        programB.setUniformValue("qt_texture", 1);

                        // TELL OPENGL PROGRAMMABLE PIPELINE HOW TO LOCATE VERTEX POSITION DATA
                        glVertexAttribPointer(programB.attributeLocation("qt_vertex"), 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
                        programB.enableAttributeArray("qt_vertex");
                        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

                        // RELEASE THE FRAME BUFFER OBJECT AND ITS ASSOCIATED GLSL PROGRAMS
                        quadIndexBuffer.release();
                    }
                    quadVertexBuffer.release();
                }
                programB.release();
            }
            frameBufferObjects[1]->release();
        }

        // FIND LOCAL MAXIMUMS
        if (frameBufferObjects[2]->bind()) {
            if (programC.bind()) {
                // CLEAR THE FRAME BUFFER OBJECT
                glViewport(5, 5, frameBufferObjects[2]->width() - 10, frameBufferObjects[2]->height() - 10);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                // BIND VBOS FOR DRAWING TRIANGLES ON SCREEN
                if (quadVertexBuffer.bind()) {
                    if (quadIndexBuffer.bind()) {
                        // BIND THE TEXTURE FROM THE FRAME BUFFER OBJECT B
                        glActiveTexture(GL_TEXTURE2);
                        glBindTexture(GL_TEXTURE_2D, frameBufferObjects[1]->texture());
                        programC.setUniformValue("qt_texture", 2);

                        // SET THE HARRIS CORNER MATRIX SCALE FACTOR K
                        programC.setUniformValue("qt_scaleFactor", qtScaleFactor);

                        // TELL OPENGL PROGRAMMABLE PIPELINE HOW TO LOCATE VERTEX POSITION DATA
                        glVertexAttribPointer(programC.attributeLocation("qt_vertex"), 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
                        programC.enableAttributeArray("qt_vertex");
                        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

                        // RELEASE THE FRAME BUFFER OBJECT AND ITS ASSOCIATED GLSL PROGRAMS
                        quadIndexBuffer.release();
                    }
                    quadVertexBuffer.release();
                }
                programC.release();
            }
            frameBufferObjects[2]->release();
        }

        // DISPLAY CORNERS ON THE INPUT SCAN
        if (frameBufferObjects[3]->bind()) {
            if (programD.bind()) {
                // CLEAR THE FRAME BUFFER OBJECT
                glViewport(0, 0, frameBufferObjects[3]->width(), frameBufferObjects[3]->height());
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                // BIND VBOS FOR DRAWING TRIANGLES ON SCREEN
                if (quadVertexBuffer.bind()) {
                    if (quadIndexBuffer.bind()) {
                        // BIND THE TEXTURE FROM THE FRAME BUFFER OBJECT C
                        glActiveTexture(GL_TEXTURE3);
                        glBindTexture(GL_TEXTURE_2D, frameBufferObjects[2]->texture());
                        programD.setUniformValue("qt_texture", 3);

                        // SET THE MINIMUM CORNER STRENGTH THRESHOLD
                        programD.setUniformValue("qt_threshold", qtCornerThreshold);
                        programD.setUniformValue("qt_radius", 10);

                        // TELL OPENGL PROGRAMMABLE PIPELINE HOW TO LOCATE VERTEX POSITION DATA
                        glVertexAttribPointer(programD.attributeLocation("qt_vertex"), 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
                        programD.enableAttributeArray("qt_vertex");
                        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

                        // RELEASE THE FRAME BUFFER OBJECT AND ITS ASSOCIATED GLSL PROGRAMS
                        quadIndexBuffer.release();
                    }
                    quadVertexBuffer.release();
                }
                programD.release();
            }
            frameBufferObjects[3]->release();
        }

        // DISPLAY CORNERS ON THE INPUT SCAN
        if (frameBufferObjects[4]->bind()) {
            if (programE.bind()) {
                // CLEAR THE FRAME BUFFER OBJECT
                glViewport(0, 0, frameBufferObjects[4]->width(), frameBufferObjects[4]->height());
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                // BIND VBOS FOR DRAWING TRIANGLES ON SCREEN
                if (quadVertexBuffer.bind()) {
                    if (quadIndexBuffer.bind()) {
                        // BIND THE ORIGINAL SCAN AS WE MERGE THE TWO
                        glActiveTexture(GL_TEXTURE0);
                        videoTexture->bind();
                        programE.setUniformValue("qt_textureA", 0);

                        // BIND THE TEXTURE FROM THE FRAME BUFFER OBJECT C
                        glActiveTexture(GL_TEXTURE4);
                        glBindTexture(GL_TEXTURE_2D, frameBufferObjects[3]->texture());
                        programE.setUniformValue("qt_textureB", 4);

                        // TELL OPENGL PROGRAMMABLE PIPELINE HOW TO LOCATE VERTEX POSITION DATA
                        glVertexAttribPointer(programE.attributeLocation("qt_vertex"), 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
                        programE.enableAttributeArray("qt_vertex");
                        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

                        // RELEASE THE FRAME BUFFER OBJECT AND ITS ASSOCIATED GLSL PROGRAMS
                        quadIndexBuffer.release();
                    }
                    quadVertexBuffer.release();
                }
                programE.release();
            }
            frameBufferObjects[4]->release();
        }
    }
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
void LAUHarrisCornerDetectorGLWidget::initialize()
{
    LAUVideoGLWidget::initialize();

    // NOW ADD OUR LIST OF HARRIS CORNER SHADER PROGRAMS
    setlocale(LC_NUMERIC, "C");
    programA.addShaderFromSourceFile(QOpenGLShader::Vertex,   ":/shaders/Shaders/filterHarrisCornersA.vert");
    programA.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/Shaders/filterHarrisCornersA.frag");
    programA.link();

    programB.addShaderFromSourceFile(QOpenGLShader::Vertex,   ":/shaders/Shaders/filterHarrisCornersB.vert");
    programB.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/Shaders/filterHarrisCornersB.frag");
    programB.link();

    programC.addShaderFromSourceFile(QOpenGLShader::Vertex,   ":/shaders/Shaders/filterHarrisCornersC.vert");
    programC.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/Shaders/filterHarrisCornersC.frag");
    programC.link();

    programD.addShaderFromSourceFile(QOpenGLShader::Vertex,   ":/shaders/Shaders/filterHarrisCornersD.vert");
    programD.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/Shaders/filterHarrisCornersD.frag");
    programD.link();

    programE.addShaderFromSourceFile(QOpenGLShader::Vertex,   ":/shaders/Shaders/filterHarrisCornersE.vert");
    programE.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/Shaders/filterHarrisCornersE.frag");
    programE.link();
    setlocale(LC_ALL, "");
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
void LAUHarrisCornerDetectorGLWidget::paint()
{
    if (frameBufferObjects[4] == NULL) {
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
                        glBindTexture(GL_TEXTURE_2D, frameBufferObjects[4]->texture());
                        program.setUniformValue("qt_texture", 0);
#ifdef Q_OS_WIN
                        program.setUniformValue("qt_flip", false);
#else
                        program.setUniformValue("qt_flip", true);
#endif
                        // TELL OPENGL PROGRAMMABLE PIPELINE HOW TO LOCATE VERTEX POSITION DATA
                        program.setAttributeBuffer("qt_vertex", GL_FLOAT, 0, 4, 4 * sizeof(float));
                        program.enableAttributeArray("qt_vertex");

                        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

                        quadIndexBuffer.release();
                    }
                    quadVertexBuffer.release();
                }
                program.release();
            }
        }
    }
}
