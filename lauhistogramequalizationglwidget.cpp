#include "lauhistogramequalizationglwidget.h"
#include "laumemoryobject.h"
#include "locale.h"

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
LAUHistogramEqualizationGLWidget::~LAUHistogramEqualizationGLWidget()
{
    if (wasInitialized()) {
        makeCurrent();
        if (frameBufferObjectA) {
            delete frameBufferObjectA;
        }
        if (frameBufferObjectB) {
            delete frameBufferObjectB;
        }
        if (histogramTexture) {
            delete histogramTexture;
        }
    }
    qDebug() << "LAUHistogramEqualizationGLWidget::~LAUHistogramEqualizationGLWidget()";
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
void LAUHistogramEqualizationGLWidget::initialize()
{
    LAUVideoGLWidget::initialize();

    // CREATE GLSL PROGRAM FOR PROCESSING THE INCOMING VIDEO
    setlocale(LC_NUMERIC, "C");

    // CREATE A SHADER TO MAP INCOMING VIDEO TO AN RGBA HISTOGRAM
    programA.addShaderFromSourceFile(QOpenGLShader::Vertex,   ":/shaders/Shaders/rgbaToHistogramFilter.vert");
    programA.addShaderFromSourceFile(QOpenGLShader::Geometry, ":/shaders/Shaders/rgbaToHistogramFilter.geom");
    programA.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/Shaders/rgbaToHistogramFilter.frag");
    programA.link();

    // CREATE A SHADER FOR RESCALING PIXELS BASED ON HISTOGRAM
    programB.addShaderFromSourceFile(QOpenGLShader::Vertex,   ":/shaders/Shaders/equalizeHistogramRGBAFilter.vert");
    programB.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/Shaders/equalizeHistogramRGBAFilter.frag");
    programB.link();

    setlocale(LC_ALL, "");
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
void LAUHistogramEqualizationGLWidget::process()
{
    // SET THE BLENDING FLAGS
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);
    glBlendEquation(GL_FUNC_ADD);
    glPointSize(1.0f);

    // SEE IF WE NEED NEW FBOS
    if (videoTexture) {
        if (pixlVertexBuffer.isCreated() == false) {
            // CREATE A BUFFER TO HOLD THE ROW AND COLUMN COORDINATES OF IMAGE PIXELS FOR THE TEXEL FETCHES
            pixlVertexBuffer = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
            pixlVertexBuffer.create();
            pixlVertexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
            if (pixlVertexBuffer.bind()) {
                pixlVertexBuffer.allocate(videoTexture->height()*videoTexture->width() * 2 * sizeof(float));
                float *vertices = (float *)pixlVertexBuffer.map(QOpenGLBuffer::WriteOnly);
                if (vertices) {
                    for (int row = 0; row < videoTexture->height(); row++) {
                        for (int col = 0; col < videoTexture->width(); col++) {
                            vertices[2 * (col + row * width()) + 0] = (float)col;
                            vertices[2 * (col + row * width()) + 1] = (float)row;
                        }
                    }
                    pixlVertexBuffer.unmap();
                } else {
                    qDebug() << QString("Unable to map pixlVertexBuffer from GPU.");
                }
            }
        }

        if (pixlIndexBuffer.isCreated() == false) {
            // CREATE AN INDEX BUFFER FOR THE RESULTING POINT CLOUD DRAWN AS TRIANGLES
            pixlIndexBuffer = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
            pixlIndexBuffer.create();
            pixlIndexBuffer.setUsagePattern(QOpenGLBuffer::StaticDraw);
            if (pixlIndexBuffer.bind()) {
                pixlIndexBuffer.allocate((videoTexture->height()*videoTexture->width())*sizeof(unsigned int));
                unsigned int *indices = (unsigned int *)pixlIndexBuffer.map(QOpenGLBuffer::WriteOnly);
                if (indices) {
                    unsigned int ind = 0;
                    for (int row = 0; row < videoTexture->height(); row++) {
                        for (int col = 0; col < videoTexture->width(); col++) {
                            indices[ind] = ind;
                            ind++;
                        }
                    }
                    pixlIndexBuffer.unmap();
                } else {
                    qDebug() << QString("Unable to map indiceBuffer from GPU.");
                }
            }
        }

        // CALCULATE THE SIZE OF THE FRAME BUFFER OBJECT TO HOLD THE HISTOGRAMS
        int blocksPerRow = 10;
        int blocksPerCol = 10;
        int blockWidth = qCeil((float)videoTexture->width() / (float)blocksPerRow);
        int blockHeght = qCeil((float)videoTexture->height() / (float)blocksPerCol);

        int numberOfBlocksX = qCeil((float)videoTexture->width() / (float)blockWidth);
        int numberOfBlocksY = qCeil((float)videoTexture->height() / (float)blockHeght);
        int numberOfBlocks = numberOfBlocksX * numberOfBlocksY;

        if (frameBufferObjectA == nullptr) {
            // CREATE THE FRAME BUFFER OBJECT TO HOLD THE HISTOGRAMS OF THE INCOMING TEXTURE
            QOpenGLFramebufferObjectFormat frameBufferObjectFormat;
            frameBufferObjectFormat.setInternalTextureFormat(GL_RGBA32F);

            // CREATE THE FRAME BUFFER OBJECT TO HOLD THE HISTOGRAMS OF THE INCOMING TEXTURE
            frameBufferObjectA = new QOpenGLFramebufferObject(CALIBRATIONHISTOGRAMLENGTH, numberOfBlocks, frameBufferObjectFormat);
            frameBufferObjectA->release();

            // CREATE A LOCAL MEMORY OBJECT TO HOLD A COPY OF THE FRAME BUFFER OBJECT
            histogramObject = LAUMemoryObject(frameBufferObjectA->width(), frameBufferObjectA->height(), 4, sizeof(float));

            // CREATE TEXTURE FOR HOLDING THE MODIFIED HISTOGRAMS FOR EQUALIZING THE INPUT IMAGE
            if (histogramTexture == nullptr) {
                histogramTexture = new QOpenGLTexture(QOpenGLTexture::Target2D);
                histogramTexture->setSize(frameBufferObjectA->width(), frameBufferObjectA->height());
                histogramTexture->setFormat(QOpenGLTexture::RGBA32F);
                histogramTexture->setWrapMode(QOpenGLTexture::ClampToBorder);
                histogramTexture->setMinificationFilter(QOpenGLTexture::Nearest);
                histogramTexture->setMagnificationFilter(QOpenGLTexture::Nearest);
                histogramTexture->allocateStorage();
            }
        }

        if (frameBufferObjectB == nullptr) {
            // CREATE THE FRAME BUFFER OBJECT TO HOLD THE HISTOGRAM EQUALIZED RESULTS AS A TEXTURE
            QOpenGLFramebufferObjectFormat frameBufferObjectFormat;
            frameBufferObjectFormat.setInternalTextureFormat(GL_RGBA32F);

            // CREATE THE FRAME BUFFER OBJECT TO HOLD THE HISTOGRAM EQUALIZED RESULTS AS A TEXTURE
            frameBufferObjectB = new QOpenGLFramebufferObject(videoTexture->width(), videoTexture->height(), frameBufferObjectFormat);
            frameBufferObjectB->release();
        }

        // BIND THE FRAME BUFFER OBJECT FOR PROCESSING THE HISTOGRAM
        // ALONG WITH THE GLSL PROGRAMS THAT WILL DO THE PROCESSING
        if (frameBufferObjectA->bind()) {
            if (programA.bind()) {
                // CLEAR THE FRAME BUFFER OBJECT
                glViewport(0, 0, frameBufferObjectA->width(), frameBufferObjectA->height());
                glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
                glClear(GL_COLOR_BUFFER_BIT);

                // BIND VBOS FOR DRAWING PIXELS ON SCREEN
                if (pixlVertexBuffer.bind()) {
                    if (pixlIndexBuffer.bind()) {
                        // BIND THE TEXTURE FROM THE FRAME BUFFER OBJECT
                        glActiveTexture(GL_TEXTURE0);
                        videoTexture->bind();
                        programA.setUniformValue("qt_texture", 0);

                        programA.setUniformValue("qt_blockSizeX", blockWidth);
                        programA.setUniformValue("qt_blockSizeY", blockHeght);
                        programA.setUniformValue("qt_blocksPerRow", blocksPerRow);
                        programA.setUniformValue("qt_blocksPerCol", blocksPerCol);

                        float geometrySlope = 2.0f / (float)frameBufferObjectA->height();
                        float geometryOffst = 1.0f / (float)frameBufferObjectA->height() - 1.0f;
                        programA.setUniformValue("qt_geometryMappingSlope", geometrySlope);
                        programA.setUniformValue("qt_geometryMappingOffst", geometryOffst);

                        // TELL OPENGL PROGRAMMABLE PIPELINE HOW TO LOCATE VERTEX POSITION DATA
                        glVertexAttribPointer(programA.attributeLocation("qt_vertex"), 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
                        programA.enableAttributeArray("qt_vertex");
                        glDrawElements(GL_POINTS, videoTexture->width()*videoTexture->height(), GL_UNSIGNED_INT, 0);

                        // RELEASE THE FRAME BUFFER OBJECT AND ITS ASSOCIATED GLSL PROGRAMS
                        pixlIndexBuffer.release();
                    }
                    pixlVertexBuffer.release();
                }
                programA.release();
            }
            frameBufferObjectA->release();
        }

        // DOWNLOAD THE HISTOGRAM BUFFER OBJECT FROM THE GPU TO THE CPU
        glBindTexture(GL_TEXTURE_2D, frameBufferObjectA->texture());
        glPixelStorei(GL_PACK_ALIGNMENT, 1);
        glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, histogramObject.constPointer());

        // EQUALIZE THE HISTOGRAMS
        for (unsigned int row = 0; row < histogramObject.height(); row++) {
            // GET A POINTER TO THE CURRENT HISTOGRAM
            float *buffer = (float *)histogramObject.constScanLine(row);

            // GET THE NUMBER OF PIXELS THE CURRENT HISTOGRAM (ALPHA VALUE OF ELEMENT IN THE HISTOGRAM)
            __m128 numPixelsInSubblockVec = _mm_set1_ps(buffer[255 * 4 + 3]);

            // SET THE UPPER BOUND ON THE HISTOGRAM DISTRIBUTION
            __m128 vecMx = _mm_set1_ps(0.025f);

            // GENERATE A CLIPPED HISTOGRAM AND COLLECT THE TRIMMED PART
            __m128 cumSumA = _mm_set1_ps(0.0f);
            for (unsigned int col = 0; col < 256; col++) {
                __m128 pixA = _mm_div_ps(_mm_load_ps(&buffer[4 * col]), numPixelsInSubblockVec);
                __m128 pixB = _mm_min_ps(pixA, vecMx);
                _mm_store_ps(&buffer[4 * col], pixB);
                cumSumA = _mm_add_ps(cumSumA, _mm_sub_ps(pixA, pixB));
            }

            // DETERMINE HOW MUCH TO TRIMMED AREA TO DISTRIBUTE OVER ALL BINS
            cumSumA = _mm_div_ps(cumSumA, _mm_set1_ps(256.0));

            // ITERATION THROUGH HISTOGRAM CALCULATING CUMMULATIVE DISTRIBUTION FUNCTION
            __m128 cumSumB = _mm_set1_ps(0.0f);
            for (unsigned int col = 0; col < 256; col++) {
                __m128 pix = _mm_add_ps(_mm_load_ps(&buffer[4 * col]), cumSumA);
                cumSumB = _mm_add_ps(cumSumB, pix);
                _mm_store_ps(&buffer[4 * col], cumSumB);
            }
        }
        //histogramObject.save(QString("/Users/dllau/Documents/histogramObject.tif"));

        // UPLOAD THE MODIFIED HISTOGRAMS BACK TO THE GPU
        histogramTexture->setData(QOpenGLTexture::RGBA, QOpenGLTexture::Float32, (const void *)histogramObject.constPointer());

        // BIND THE FRAME BUFFER OBJECT FOR PROCESSING THE HISTOGRAM
        // ALONG WITH THE GLSL PROGRAMS THAT WILL DO THE PROCESSING
        if (frameBufferObjectB->bind()) {
            if (programB.bind()) {
                // CLEAR THE FRAME BUFFER OBJECT
                glViewport(0, 0, frameBufferObjectB->width(), frameBufferObjectB->height());
                glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
                glClear(GL_COLOR_BUFFER_BIT);

                // BIND VBOS FOR DRAWING PIXELS ON SCREEN
                if (quadVertexBuffer.bind()) {
                    if (quadIndexBuffer.bind()) {
                        // BIND THE TEXTURE OF THE INPUT IMAGE
                        glActiveTexture(GL_TEXTURE0);
                        videoTexture->bind();
                        programB.setUniformValue("qt_texture", 0);

                        // BIND THE TEXTURE OF THE HISTOGRAM TEXTURE
                        glActiveTexture(GL_TEXTURE1);
                        histogramTexture->bind();
                        programB.setUniformValue("qt_histogram", 1);

                        // SET THE SUBBLOCK COORDINATE MAPPING PARAMETERS
                        programB.setUniformValue("qt_blockSizeX", blockWidth);
                        programB.setUniformValue("qt_blockSizeY", blockHeght);
                        programB.setUniformValue("qt_blocksPerRow", blocksPerRow);
                        programB.setUniformValue("qt_blocksPerCol", blocksPerCol);

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
            //static LAUMemoryObject object(frameBufferObjectB->width(), frameBufferObjectB->height(), 4, sizeof(float));
            //glBindTexture(GL_TEXTURE_2D, frameBufferObjectB->texture());
            //glPixelStorei(GL_PACK_ALIGNMENT, 1);
            //glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, object.constPointer());
            //object.save(QString("/Users/dllau/Documents/histogram.tif"));
            frameBufferObjectB->release();
        }
    }
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
void LAUHistogramEqualizationGLWidget::paint()
{
    if (frameBufferObjectB == nullptr) {
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
                        glBindTexture(GL_TEXTURE_2D, frameBufferObjectB->texture());
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
