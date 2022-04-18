#include "lautiredetectorglfilter.h"
#include "locale.h"

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
LAUTireDetectorGLWidget::~LAUTireDetectorGLWidget()
{
    if (wasInitialized()) {
        makeCurrent();
        delete frameBufferObject;
    }
    qDebug() << "LAUTireDetectorGLWidget::~LAUTireDetectorGLWidget()";
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
void LAUTireDetectorGLWidget::process()
{
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

        // SET CLEAR COLOR AS NOT A NUMBERS
        glClearColor(NAN, NAN, NAN, NAN);

        // CALCULATE THE GRADIENT BUFFER
        if (frameBufferObject->bind()) {
            if (programA.bind()) {
                // CLEAR THE FRAME BUFFER OBJECT
                glViewport(1, 1, frameBufferObject->width() - 2, frameBufferObject->height() - 2);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                // BIND VBOS FOR DRAWING TRIANGLES ON SCREEN
                if (quadVertexBuffer.bind()) {
                    if (quadIndexBuffer.bind()) {
                        // BIND THE TEXTURE FROM THE ORIGINAL SCAN
                        glActiveTexture(GL_TEXTURE0);
                        videoTexture->bind();
                        programA.setUniformValue("qt_texture", 0);

                        // SET THE RADIUS AND SKEW PARAMETER
                        programA.setUniformValue("qt_radius", 50);
                        programA.setUniformValue("qt_skew", 1.25f);

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
    }
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
void LAUTireDetectorGLWidget::initialize()
{
    LAUVideoGLWidget::initialize();

    // NOW ADD OUR LIST OF HARRIS CORNER SHADER PROGRAMS
    setlocale(LC_NUMERIC, "C");
    programA.addShaderFromSourceFile(QOpenGLShader::Vertex,   ":/shaders/Shaders/filterSkewBlur.vert");
    programA.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/Shaders/filterSkewBlur.frag");
    programA.link();
    setlocale(LC_ALL, "");
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
void LAUTireDetectorGLWidget::paint()
{
    if (frameBufferObject == NULL) {
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
