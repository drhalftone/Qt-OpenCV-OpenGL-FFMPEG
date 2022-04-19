/*********************************************************************************
 *                                                                               *
 * Copyright (c) 2017, Dr. Daniel L. Lau                                         *
 * All rights reserved.                                                          *
 *                                                                               *
 * Redistribution and use in source and binary forms, with or without            *
 * modification, are permitted provided that the following conditions are met:   *
 * 1. Redistributions of source code must retain the above copyright             *
 *    notice, this list of conditions and the following disclaimer.              *
 * 2. Redistributions in binary form must reproduce the above copyright          *
 *    notice, this list of conditions and the following disclaimer in the        *
 *    documentation and/or other materials provided with the distribution.       *
 * 3. All advertising materials mentioning features or use of this software      *
 *    must display the following acknowledgement:                                *
 *    This product includes software developed by the <organization>.            *
 * 4. Neither the name of the <organization> nor the                             *
 *    names of its contributors may be used to endorse or promote products       *
 *    derived from this software without specific prior written permission.      *
 *                                                                               *
 * THIS SOFTWARE IS PROVIDED BY Dr. Daniel L. Lau ''AS IS'' AND ANY              *
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED     *
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE        *
 * DISCLAIMED. IN NO EVENT SHALL Dr. Daniel L. Lau BE LIABLE FOR ANY             *
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES    *
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;  *
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND   *
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT    *
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS *
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                  *
 *                                                                               *
 *********************************************************************************/

#include <stdio.h>
#include "laumemoryobject.h"

#ifdef Q_PROCESSOR_ARM
void *_mm_malloc(int size, int align)
{
    Q_UNUSED(align);
    return (malloc(size));
}

void _mm_free(void *pointer)
{
    free(pointer);
}
#endif

using namespace libtiff;

int LAUMemoryObjectData::instanceCounter = 0;

QString LAUMemoryObject::lastTiffWarningString;
QString LAUMemoryObject::lastTiffErrorString;

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
void myTIFFWarningHandler(const char *stringA, const char *stringB, va_list args)
{
    Q_UNUSED(stringA);

    char buffer[1024];
#ifdef Q_OS_WIN
    int length = vsprintf_s(buffer, (char *)stringB, args);
#else
    int length = vsprintf(buffer, (char *)stringB, args);
#endif
    LAUMemoryObject::lastTiffWarningString = QString(QByteArray(buffer, length));
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
void myTIFFErrorHandler(const char *stringA, const char *stringB, va_list args)
{
    Q_UNUSED(stringA);

    char buffer[1024];
#ifdef Q_OS_WIN
    int length = vsprintf_s(buffer, (char *)stringB, args);
#else
    int length = vsprintf(buffer, (char *)stringB, args);
#endif
    LAUMemoryObject::lastTiffErrorString = QString(QByteArray(buffer, length));
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
int LAU3DVideoParameters::colors(LAUVideoPlaybackColor clr)
{
    switch (clr) {
        case ColorUndefined:
            return (0);
        case ColorGray:
            return (1);
        case ColorRGB:
            return (3);
        case ColorRGBA:
            return (4);
        case ColorXYZ:
            return (3);
        case ColorXYZW:
            return (4);
        case ColorXYZG:
            return (4);
        case ColorXYZRGB:
            return (6);
        case ColorXYZWRGBA:
            return (8);
        default:
            return (0);
    }
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
bool LAU3DVideoParameters::isMachineVision(LAUVideoPlaybackDevice dvc)
{
    if (dvc == DeviceProsilicaGRY || dvc == DeviceProsilicaARG || dvc == DeviceProsilicaIOS || dvc == DeviceProsilicaLCG ||
        dvc == DeviceProsilicaAST || dvc == DeviceProsilicaPST || dvc == DeviceProsilicaDPR || dvc == DeviceProsilicaRGB) {
        return (true);
    }
    return (false);
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
LAUMemoryObjectData::LAUMemoryObjectData()
{
    numRows = 0;
    numCols = 0;
    numChns = 0;
    numByts = 0;
    numFrms = 0;

    stepBytes = 0;
    frameBytes = 0;

    buffer = nullptr;
    rfidString = nullptr;
    transformMatrix = nullptr;
    anchorPt = nullptr;
    elapsedTime = nullptr;

    numBytesTotal = 0;
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
LAUMemoryObjectData::LAUMemoryObjectData(unsigned int cols, unsigned int rows, unsigned int chns, unsigned int byts, unsigned int frms)
{
    numRows = rows;
    numCols = cols;
    numChns = chns;
    numByts = byts;
    numFrms = frms;

    buffer = nullptr;
    rfidString = nullptr;
    transformMatrix = nullptr;
    anchorPt = nullptr;
    elapsedTime = nullptr;

    stepBytes = 0;
    frameBytes = 0;
    numBytesTotal = 0;

    allocateBuffer();

    // FOR A BRAND NEW MEMORY OBJECT, INITIALIZE THE MEMORY TO ALL ZEROS
    if (buffer) {
        memset(buffer, 0, numBytesTotal);
    }
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
LAUMemoryObjectData::LAUMemoryObjectData(unsigned long long bytes)
{
    numRows = bytes;
    numCols = 1;
    numChns = 1;
    numByts = 1;
    numFrms = 1;

    buffer = nullptr;
    rfidString = nullptr;
    transformMatrix = nullptr;
    anchorPt = nullptr;
    elapsedTime = nullptr;

    stepBytes = 0;
    frameBytes = 0;
    numBytesTotal = 0;

    allocateBuffer();

    // FOR A BRAND NEW MEMORY OBJECT, INITIALIZE THE MEMORY TO ALL ZEROS
    if (buffer) {
        memset(buffer, 0, numBytesTotal);
    }
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
LAUMemoryObjectData::LAUMemoryObjectData(const LAUMemoryObjectData &other) : LAUMemoryObjectData()
{
    // COPY OVER THE SIZE PARAMETERS FROM THE SUPPLIED OBJECT
    numRows = other.numRows;
    numCols = other.numCols;
    numChns = other.numChns;
    numByts = other.numByts;
    numFrms = other.numFrms;

    // SET THESE VARIABLES TO ZERO AND LET THEM BE MODIFIED IN THE ALLOCATION METHOD
    buffer = nullptr;
    rfidString = nullptr;
    transformMatrix = nullptr;
    anchorPt = nullptr;
    elapsedTime = nullptr;

    stepBytes = 0;
    frameBytes = 0;
    numBytesTotal = 0;

    allocateBuffer();
    if (buffer) {
        memcpy(buffer, other.buffer, numBytesTotal);
        rfidString->clear();
        rfidString->append(*(other.rfidString));
        memcpy((void *)transformMatrix->constData(), (void *)other.transformMatrix->constData(), sizeof(QMatrix4x4));
        anchorPt->setX(other.anchorPt->x());
        anchorPt->setY(other.anchorPt->y());
        *elapsedTime = *other.elapsedTime;
    }
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
LAUMemoryObjectData::~LAUMemoryObjectData()
{
    if (buffer != NULL) {
        instanceCounter = instanceCounter - 1;
        qDebug() << QString("LAUMemoryObjectData::~LAUMemoryObjectData() %1").arg(instanceCounter) << numRows << numCols << numChns << numByts << numBytesTotal;
        _mm_free(buffer);

        delete rfidString;
        delete transformMatrix;
        delete anchorPt;
        delete elapsedTime;
    }
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
void LAUMemoryObjectData::allocateBuffer()
{
    // ALLOCATE SPACE FOR HOLDING PIXEL DATA BASED ON NUMBER OF CHANNELS AND BYTES PER PIXEL
    numBytesTotal  = (unsigned long long)numRows;
    numBytesTotal *= (unsigned long long)numCols;
    numBytesTotal *= (unsigned long long)numChns;
    numBytesTotal *= (unsigned long long)numByts;
    numBytesTotal *= (unsigned long long)numFrms;

    if (numBytesTotal > 0) {
        instanceCounter = instanceCounter + 1;
        qDebug() << QString("LAUMemoryObjectData::allocateBuffer() %1").arg(instanceCounter) << numRows << numCols << numChns << numByts << numFrms;

        stepBytes  = numCols * numChns * numByts;
        frameBytes = numCols * numChns * numByts * numRows;
        buffer     = _mm_malloc(numBytesTotal + 128, 16);
        if (buffer == nullptr) {
            qDebug() << QString("LAUVideoBufferData::allocateBuffer() MAJOR ERROR DID NOT ALLOCATE SPACE!!!");
            qDebug() << QString("LAUVideoBufferData::allocateBuffer() MAJOR ERROR DID NOT ALLOCATE SPACE!!!");
            qDebug() << QString("LAUVideoBufferData::allocateBuffer() MAJOR ERROR DID NOT ALLOCATE SPACE!!!");
        } else {
            rfidString = new QString();
            transformMatrix = new QMatrix4x4();
            anchorPt = new QPoint(-1, -1);
            elapsedTime = new unsigned int;
            *elapsedTime = 0;
        }
    }
    return;
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
LAUMemoryObject::LAUMemoryObject(QString filename, int index) : data(new LAUMemoryObjectData())
{
    // GET A FILE TO OPEN FROM THE USER IF NOT ALREADY PROVIDED ONE
    if (filename.isNull()) {
#ifndef HEADLESS
        QSettings settings;
        QString directory = settings.value("LAUMemoryObject::lastUsedDirectory", QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)).toString();
        if (QDir().exists(directory) == false) {
            directory = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
        }
        filename = QFileDialog::getOpenFileName(0, QString("Load scan from disk (*.tif)"), directory, QString("*.tif;*.tiff"));
        if (filename.isEmpty() == false) {
            settings.setValue("LAUMemoryObject::lastUsedDirectory", QFileInfo(filename).absolutePath());
        } else {
            return;
        }
#else
        return;
#endif
    }

    // IF WE HAVE A VALID TIFF FILE, LOAD FROM DISK
    // OTHERWISE TRY TO CONNECT TO SCANNER
    if (QFile::exists(filename)) {
        // OPEN INPUT TIFF FILE FROM DISK
        TIFF *inTiff = TIFFOpen(filename.toLatin1(), "r");
        if (!inTiff) {
            return;
        }
        load(inTiff, index);
        TIFFClose(inTiff);
    }
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
LAUMemoryObject::LAUMemoryObject(QImage image) : data(new LAUMemoryObjectData())
{
    if (image.isNull() == false) {
        data->numCols = (unsigned int)image.width();
        data->numRows = (unsigned int)image.height();
        data->numByts = sizeof(float);
        data->numChns = 4;
        data->numFrms = 1;
        data->allocateBuffer();

        for (unsigned int row = 0; row < height(); row++) {
            float *toBuffer = reinterpret_cast<float *>(constScanLine(row));
            for (unsigned int col = 0; col < width(); col++) {
                QRgb pixel = image.pixel(static_cast<int>(col), static_cast<int>(row));
                toBuffer[4 * col + 0] = (float)qRed(pixel) / 255.0f;
                toBuffer[4 * col + 1] = (float)qGreen(pixel) / 255.0f;
                toBuffer[4 * col + 2] = (float)qBlue(pixel) / 255.0f;
                toBuffer[4 * col + 3] = 1.0f;
            }
        }
    }
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
LAUMemoryObject::LAUMemoryObject(TIFF *inTiff, int index) : data(new LAUMemoryObjectData())
{
    load(inTiff, index);
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
bool LAUMemoryObject::save(QString filename) const
{
    // LET THE USER SELECT A FILE FROM THE FILE DIALOG
    if (filename.isNull()) {
#ifndef HEADLESS
        QSettings settings;
        QString directory = settings.value(QString("LAUMemoryObject::lastUsedDirectory"), QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)).toString();
        if (QDir().exists(directory) == false) {
            directory = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
        }
        int counter = 0;
        do {
            if (counter == 0) {
                filename = QString("%1/Untitled.tif").arg(directory);
            } else {
                filename = QString("%1/Untitled%2.tif").arg(directory).arg(counter);
            }
            counter++;
        } while (QFile::exists(filename));

        filename = QFileDialog::getSaveFileName(nullptr, QString("Save image to disk (*.tif)"), filename, QString("*.tif;*.tiff"));
        if (!filename.isNull()) {
            settings.setValue(QString("LAUMemoryObject::lastUsedDirectory"), QFileInfo(filename).absolutePath());
            if (!filename.toLower().endsWith(QString(".tiff"))) {
                if (!filename.toLower().endsWith(QString(".tif"))) {
                    filename = QString("%1.tif").arg(filename);
                }
            }
        } else {
            return (false);
        }
#else
        return (false);
#endif
    }

    // OPEN TIFF FILE FOR SAVING THE IMAGE USING BIGTIFF FOR LARGE FILES
    TIFF *outputTiff = (this->length() > 100000000) ? TIFFOpen(filename.toLatin1(), "w8") : TIFFOpen(filename.toLatin1(), "w");
    if (!outputTiff) {
        return (false);
    }

    // CREATE A RETURN FLAG
    bool flag = true;

    if (frames() == 1) {
        // WRITE IMAGE TO CURRENT DIRECTORY AND SAVE RESULT TO BOOL FLAG
        flag = save(outputTiff);
    } else {
        // CREATE A MEMORY OBJECT TO HOLD A SINGLE FRAME OF VIDEO
        //LAUMemoryObject object(width(), height(), colors(), depth(), 1);
        //for (unsigned int n = 0; n < frames(); n++) {
        //    // COPY THE CURRENT FRAME INTO THE TEMPORARY FRAME BUFFER OBJECT
        //    memcpy(object.constPointer(), frame(n), block());

        //    // SAVE THE CURRENT FRAME INTO ITS OWN DIRECTORY INSIDE THE NEW TIFF FILE
        //    if (object.save(outputTiff, n) == false) {
        //        flag = false;
        //        break;
        //    }
        //}
        LAUMemoryObject object(width(), height(), colors()*frames(), depth(), 1);

        // COPY THE CURRENT FRAME INTO THE TEMPORARY FRAME BUFFER OBJECT
        memcpy(object.constPointer(), constPointer(), length());

        // SAVE THE CURRENT FRAME INTO ITS OWN DIRECTORY INSIDE THE NEW TIFF FILE
        if (object.save(outputTiff, 0) == false) {
            flag = false;
        }
    }

    // CLOSE TIFF FILE
    TIFFClose(outputTiff);
    return (flag);
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
bool LAUMemoryObject::save(TIFF *otTiff, int index) const
{
    // WRITE FORMAT PARAMETERS TO CURRENT TIFF DIRECTORY
    TIFFSetField(otTiff, TIFFTAG_SUBFILETYPE, FILETYPE_PAGE);
    TIFFSetField(otTiff, TIFFTAG_IMAGEWIDTH, (unsigned long)width());
    TIFFSetField(otTiff, TIFFTAG_IMAGELENGTH, (unsigned long)(height()*frames()));
    TIFFSetField(otTiff, TIFFTAG_RESOLUTIONUNIT, RESUNIT_INCH);
    TIFFSetField(otTiff, TIFFTAG_XRESOLUTION, 72.0);
    TIFFSetField(otTiff, TIFFTAG_YRESOLUTION, 72.0);
    TIFFSetField(otTiff, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
    TIFFSetField(otTiff, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
    TIFFSetField(otTiff, TIFFTAG_SAMPLESPERPIXEL, (unsigned short)colors());
    TIFFSetField(otTiff, TIFFTAG_BITSPERSAMPLE, (unsigned short)(8 * depth()));
    if (colors() == 1) {
        TIFFSetField(otTiff, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
    } else if (colors() == 3) {
        TIFFSetField(otTiff, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
    } else if (colors() == 4) {
        TIFFSetField(otTiff, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_SEPARATED);
    } else {
        TIFFSetField(otTiff, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
        unsigned short *smples = new unsigned short [colors()];
        for (unsigned int n = 0; n < colors(); n++) {
            smples[n] = EXTRASAMPLE_UNSPECIFIED;
        }
        TIFFSetField(otTiff, TIFFTAG_EXTRASAMPLES, colors() - 1, smples);
        delete [] smples;
    }
    TIFFSetField(otTiff, TIFFTAG_COMPRESSION, COMPRESSION_LZW);
    TIFFSetField(otTiff, TIFFTAG_XPOSITION, qMax(0.0f, (float)anchor().x()));
    TIFFSetField(otTiff, TIFFTAG_YPOSITION, qMax(0.0f, (float)anchor().y()));
    TIFFSetField(otTiff, TIFFTAG_PREDICTOR, PREDICTOR_HORIZONTAL);
    TIFFSetField(otTiff, TIFFTAG_ROWSPERSTRIP, 1);

    // WRITE THE RFID TAG STRING TO THE PAGE NAME TAG
    if (rfid().isEmpty()) {
        TIFFSetField(otTiff, TIFFTAG_PAGENAME, QString("not defined").toLatin1().data());
    } else {
        TIFFSetField(otTiff, TIFFTAG_PAGENAME, rfid().toLatin1().data());
    }

    if (depth() == sizeof(float)) {
        // SEE IF WE HAVE TO TELL THE TIFF READER THAT WE ARE STORING
        // PIXELS IN 32-BIT FLOATING POINT FORMAT
        TIFFSetField(otTiff, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_IEEEFP);
    }

    // MAKE TEMPORARY BUFFER TO HOLD CURRENT ROW BECAUSE COMPRESSION DESTROYS
    // WHATS EVER INSIDE THE BUFFER
    unsigned int cnt = 0;
    unsigned char *tempBuffer = (unsigned char *)malloc(step());
    for (unsigned int frm = 0; frm < frames(); frm++) {
        for (unsigned int row = 0; row < height(); row++) {
            memcpy(tempBuffer, constScanLine(row, frm), step());
            TIFFWriteScanline(otTiff, tempBuffer, cnt++, 0);
        }
    }
    free(tempBuffer);

    // WRITE THE CURRENT DIRECTORY AND PREPARE FOR THE NEW ONE
    TIFFWriteDirectory(otTiff);

    // WRITE THE ELAPSED TIME STAMP TO AN EXIF TAG
    uint64_t dir_offset;
    TIFFCreateEXIFDirectory(otTiff);
    TIFFSetField(otTiff, EXIFTAG_SUBSECTIME, QString("%1").arg(elapsed()).toLatin1().data());
    TIFFWriteCustomDirectory(otTiff, &dir_offset);

    TIFFSetDirectory(otTiff, index);
    TIFFSetField(otTiff, TIFFTAG_EXIFIFD, dir_offset);
    TIFFRewriteDirectory(otTiff);

    return (true);
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
bool LAUMemoryObject::load(TIFF *inTiff, int index)
{
    // LOAD INPUT TIFF FILE PARAMETERS IMPORTANT TO RESAMPLING THE IMAGE
    unsigned long uLongVariable;
    unsigned short uShortVariable;

    // SEE IF USER WANTS TO LOAD A PARTICULAR DIRECTORY WITHIN A MULTIDIRECTORY TIFF FILE
    if (index > -1) {
        TIFFSetDirectory(inTiff, (unsigned short)index);
    }

    // NUMBER OF FRAMES IS ALWAYS EQUAL TO ONE
    data->numFrms = 1;

    // GET THE HEIGHT AND WIDTH OF INPUT IMAGE IN PIXELS
    TIFFGetField(inTiff, TIFFTAG_IMAGEWIDTH, &uLongVariable);
    data->numCols = uLongVariable;
    TIFFGetField(inTiff, TIFFTAG_IMAGELENGTH, &uLongVariable);
    data->numRows = uLongVariable;
    TIFFGetField(inTiff, TIFFTAG_SAMPLESPERPIXEL, &uShortVariable);
    data->numChns = uShortVariable;
    TIFFGetField(inTiff, TIFFTAG_BITSPERSAMPLE, &uShortVariable);
    data->numByts = uShortVariable / 8;

    // READ AND CHECK THE PHOTOMETRIC INTERPRETATION FIELD AND MAKE SURE ITS WHAT WE EXPECT
    TIFFGetField(inTiff, TIFFTAG_PHOTOMETRIC, &uShortVariable);
    //if (uShortVariable != PHOTOMETRIC_SEPARATED && uShortVariable != PHOTOMETRIC_MINISBLACK) {
    //    return (false);
    //}

    // ALLOCATE SPACE TO HOLD IMAGE DATA
    data->allocateBuffer();

    // LOAD THE ANCHOR POINT
    float xPos = -1.0f, yPos = -1.0f;
    TIFFGetField(inTiff, TIFFTAG_XPOSITION, &xPos);
    TIFFGetField(inTiff, TIFFTAG_YPOSITION, &yPos);
    setConstAnchor(QPoint(qRound(xPos), qRound(yPos)));

    // READ DATA AS EITHER CHUNKY OR PLANAR FORMAT
    if (data->buffer) {
        short shortVariable;
        TIFFGetField(inTiff, TIFFTAG_PLANARCONFIG, &shortVariable);
        if (shortVariable == PLANARCONFIG_SEPARATE) {
            unsigned char *tempBuffer = new unsigned char [step()];
            for (unsigned int chn = 0; chn < colors(); chn++) {
                for (unsigned int row = 0; row < height(); row++) {
                    unsigned char *pBuffer = scanLine(row);
                    TIFFReadScanline(inTiff, tempBuffer, (int)row, (int)chn);
                    for (unsigned int col = 0; col < width(); col++) {
                        ((float *)pBuffer)[col * colors() + chn] = ((float *)tempBuffer)[col];
                    }
                }
            }
            delete [] tempBuffer;
        } else if (shortVariable == PLANARCONFIG_CONTIG) {
            for (unsigned int row = 0; row < height(); row++) {
                TIFFReadScanline(inTiff, (unsigned char *)scanLine(row), static_cast<unsigned int>(row));
            }
        }
    }

    // LOAD A CUSTOM FILESTRING FROM THE DOCUMENT NAME TIFFTAG
    char *dataString;
    if (TIFFGetField(inTiff, TIFFTAG_PAGENAME, &dataString)) {
        setConstRFID(QString(QByteArray(dataString)));
    } else {
        setConstRFID(QString());
    }

    // GET THE ELAPSED TIME VALUE FROM THE EXIF TAG FOR SUBSECOND TIME
    uint64_t directoryOffset;
    if (TIFFGetField(inTiff, TIFFTAG_EXIFIFD, &directoryOffset)) {
        char *byteArray;
        TIFFReadEXIFDirectory(inTiff, directoryOffset);
        if (TIFFGetField(inTiff, EXIFTAG_SUBSECTIME, &byteArray)) {
            setConstElapsed(static_cast<unsigned int>(QString(QByteArray(byteArray)).toInt()));
        }
    }

    return (true);
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
bool LAUMemoryObject::loadInto(QString filename, int index)
{
    // CREATE A RETURN VARIABLE AND INITIALIZE AS FALSE
    bool flag = false;

    // IF WE HAVE A VALID TIFF FILE, LOAD FROM DISK
    // OTHERWISE TRY TO CONNECT TO SCANNER
    if (QFile::exists(filename)) {
        // OPEN INPUT TIFF FILE FROM DISK
        TIFF *inTiff = TIFFOpen(filename.toLatin1(), "r");
        if (!inTiff) {
            return (false);
        }
        flag = loadInto(inTiff, index);
        TIFFClose(inTiff);
    }
    return (flag);
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
bool LAUMemoryObject::loadInto(TIFF *inTiff, int index)
{
    // LOAD INPUT TIFF FILE PARAMETERS IMPORTANT TO RESAMPLING THE IMAGE
    unsigned short uShortVariable;

    // SEE IF USER WANTS TO LOAD A PARTICULAR DIRECTORY WITHIN A MULTIDIRECTORY TIFF FILE
    if (index > -1) {
        TIFFSetDirectory(inTiff, (unsigned short)index);
    }

    // GET THE HEIGHT AND WIDTH OF INPUT IMAGE IN PIXELS
    TIFFGetField(inTiff, TIFFTAG_IMAGEWIDTH, &uShortVariable);
    if (data->numCols != uShortVariable) {
        return (false);
    }

    TIFFGetField(inTiff, TIFFTAG_IMAGELENGTH, &uShortVariable);
    if ((data->numFrms * data->numRows) != uShortVariable) {
        return (false);
    }

    TIFFGetField(inTiff, TIFFTAG_SAMPLESPERPIXEL, &uShortVariable);
    if (data->numChns != uShortVariable) {
        return (false);
    }

    TIFFGetField(inTiff, TIFFTAG_BITSPERSAMPLE, &uShortVariable);
    if (data->numByts != uShortVariable / 8) {
        return (false);
    }

    // READ AND CHECK THE PHOTOMETRIC INTERPRETATION FIELD AND MAKE SURE ITS WHAT WE EXPECT
    TIFFGetField(inTiff, TIFFTAG_PHOTOMETRIC, &uShortVariable);
    if (uShortVariable != PHOTOMETRIC_SEPARATED && uShortVariable != PHOTOMETRIC_MINISBLACK) {
        return (false);
    }

    // LOAD THE ANCHOR POINT
    float xPos = -1.0f, yPos = -1.0f;
    TIFFGetField(inTiff, TIFFTAG_XPOSITION, &xPos);
    TIFFGetField(inTiff, TIFFTAG_YPOSITION, &yPos);
    setConstAnchor(QPoint(qRound(xPos), qRound(yPos)));

    // READ DATA AS EITHER CHUNKY OR PLANAR FORMAT
    if (data->buffer) {
        short shortVariable;
        TIFFGetField(inTiff, TIFFTAG_PLANARCONFIG, &shortVariable);
        if (shortVariable == PLANARCONFIG_SEPARATE) {
            unsigned char *tempBuffer = new unsigned char [step()];
            for (unsigned int chn = 0; chn < colors(); chn++) {
                for (unsigned int row = 0; row < height() * frames(); row++) {
                    unsigned char *pBuffer = scanLine(row);
                    TIFFReadScanline(inTiff, tempBuffer, (int)row, (int)chn);
                    for (unsigned int col = 0; col < width(); col++) {
                        ((float *)pBuffer)[col * colors() + chn] = ((float *)tempBuffer)[col];
                    }
                }
            }
            delete [] tempBuffer;
        } else if (shortVariable == PLANARCONFIG_CONTIG) {
            for (unsigned int row = 0; row < height() * frames(); row++) {
                TIFFReadScanline(inTiff, (unsigned char *)scanLine(row), static_cast<unsigned int>(row));
            }
        }
    }

    // LOAD A CUSTOM FILESTRING FROM THE DOCUMENT NAME TIFFTAG
    char *dataString;
    if (TIFFGetField(inTiff, TIFFTAG_PAGENAME, &dataString)) {
        setConstRFID(QString(QByteArray(dataString)));
    } else {
        setConstRFID(QString());
    }

    // GET THE ELAPSED TIME VALUE FROM THE EXIF TAG FOR SUBSECOND TIME
    uint64_t directoryOffset;
    if (TIFFGetField(inTiff, TIFFTAG_EXIFIFD, &directoryOffset)) {
        char *byteArray;
        TIFFReadEXIFDirectory(inTiff, directoryOffset);
        if (TIFFGetField(inTiff, EXIFTAG_SUBSECTIME, &byteArray)) {
            setConstElapsed(static_cast<unsigned int>(QString(QByteArray(byteArray)).toInt()));
        }
    }

    return (true);
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
unsigned int LAUMemoryObject::nonZeroPixelsCount() const
{
    // CREATE A REGISTER TO HOLD THE PIXEL COUNT
    unsigned int pixels = 0;

    // CREATE A VECTOR TO HOLD THE ACCUMULATED SUM OF PIXELS
    __m128i acSum = _mm_set1_epi32(0);

    if (depth() == sizeof(unsigned char)) {
        // CREATE A ZERO VECTOR FOR THE COMPARE OPERATION
        __m128i zeros = _mm_set1_epi8(0);

        // GRAB THE POINTER TO THE MEMORY OBJECT DATA
        unsigned char *buffer = (unsigned char *)constPointer();

        // ITERATE THROUGH THE BUFFER 16 BYTES AT A TIME
        for (unsigned int n = 0; n < length(); n += 16) {
            __m128i pixels = _mm_cmpeq_epi8(_mm_load_si128((const __m128i *)(buffer + n)), zeros);

            // HORIZONTAL SUM THE BYTES INTO 64-BIT INTEGERS AND
            __m128i vecL = _mm_sad_epu8(pixels, zeros);

            // ACCUMULATE THE SUM OF THE VECTORS TO FORM A SUM OF INTS
            acSum = _mm_add_epi32(acSum, vecL);
        }
        acSum = _mm_hadd_epi32(acSum, acSum);
        acSum = _mm_hadd_epi32(acSum, acSum);

        // EXTRACT THE INTEGER AND DIVIDE BY 255 TO GET UNITS OF PIXELS
        pixels = _mm_extract_epi32(acSum, 0) / 255;
    } else if (depth() == sizeof(unsigned short)) {
        // CREATE A ZERO VECTOR FOR THE COMPARE OPERATION
        __m128i zeros = _mm_set1_epi16(0);

        // GRAB THE POINTER TO THE MEMORY OBJECT DATA
        unsigned char *buffer = (unsigned char *)constPointer();

        // ITERATE THROUGH THE BUFFER 16 BYTES AT A TIME
        for (unsigned int n = 0; n < length(); n += 16) {
            __m128i pixels = _mm_cmpeq_epi16(_mm_load_si128((const __m128i *)(buffer + n)), zeros);

            // UNPACK FROM UNSIGNED SHORTS TO UNSIGNED INTS
            __m128i vecL = _mm_hadds_epi16(pixels, zeros);
            __m128i vecH = _mm_cvtepi16_epi32(vecL);

            // ACCUMULATE THE SUM OF THE VECTORS TO FORM A SUM OF INTS
            acSum = _mm_add_epi32(acSum, vecH);
        }
        acSum = _mm_hadd_epi32(acSum, acSum);
        acSum = _mm_hadd_epi32(acSum, acSum);

        // EXTRACT THE INTEGER AND DIVIDE BY 65535 TO GET UNITS OF PIXELS
        pixels = (unsigned int)(-1 * _mm_extract_epi32(acSum, 0));
    } else if (depth() == sizeof(float)) {
        // CREATE A ZERO VECTOR FOR THE COMPARE OPERATION
        __m128 zeros = _mm_set1_ps(0.0f);

        // GRAB THE POINTER TO THE MEMORY OBJECT DATA
        unsigned char *buffer = (unsigned char *)constPointer();

        // ITERATE THROUGH THE BUFFER 16 BYTES AT A TIME
        for (unsigned int n = 0; n < length(); n += 16) {
            __m128i pixels = _mm_castps_si128(_mm_cmpeq_ps(_mm_load_ps((const float *)(buffer + n)), zeros));

            // ACCUMULATE THE SUM OF THE VECTORS TO FORM A SUM OF INTS
            acSum = _mm_add_epi32(acSum, pixels);
        }
        acSum = _mm_hadd_epi32(acSum, acSum);
        acSum = _mm_hadd_epi32(acSum, acSum);

        // EXTRACT THE INTEGER AND DIVIDE BY 65535 TO GET UNITS OF PIXELS
        pixels = (unsigned int)(-1 * _mm_extract_epi32(acSum, 0));
    }

    // AT THIS POINT, THE SUM OF ZEROS RESULTS IN ADDING -1S TOGETHER
    // SO WE JUST NEED TO ADD THE NUMBER OF PIXELS TO GET THE NUMBER
    // OF NON-ZERO PIXELS IN THE BUFFER
    pixels = (unsigned int)((int)(width() * height() * colors()) - (int)pixels);

    // RETURN THE NUMBER OF NON-ZERO PIXELS
    return (pixels);
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
LAUMemoryObject LAUMemoryObject::toFloat()
{
    LAUMemoryObject object(width(), height(), colors(), sizeof(float), frames());

    unsigned char *fmBuffer = (unsigned char *)constPointer();
    unsigned char *toBuffer = (unsigned char *)object.constPointer();

    if (depth() == sizeof(unsigned char)) {
        __m128i vecA = _mm_set_epi8(-1, -1, -1, 3, -1, -1, -1, 2, -1, -1, -1, 1, -1, -1, -1, 0);
        __m128i vecB = _mm_set_epi8(-1, -1, -1, 7, -1, -1, -1, 6, -1, -1, -1, 5, -1, -1, -1, 4);
        __m128i vecC = _mm_set_epi8(-1, -1, -1, 11, -1, -1, -1, 10, -1, -1, -1, 9, -1, -1, -1, 8);
        __m128i vecD = _mm_set_epi8(-1, -1, -1, 15, -1, -1, -1, 14, -1, -1, -1, 13, -1, -1, -1, 12);
        __m128 vecE = _mm_set1_ps(1.0f / 255.0f);
        for (unsigned long long pix = 0; pix < length(); pix += 16) {
            __m128i inVec = _mm_loadu_si128((const __m128i *)&fmBuffer[pix]);
            _mm_store_ps((float *)&toBuffer[4 * pix + 0], _mm_mul_ps(_mm_cvtepi32_ps(_mm_shuffle_epi8(inVec, vecA)), vecE));
            _mm_store_ps((float *)&toBuffer[4 * pix + 16], _mm_mul_ps(_mm_cvtepi32_ps(_mm_shuffle_epi8(inVec, vecB)), vecE));
            _mm_store_ps((float *)&toBuffer[4 * pix + 32], _mm_mul_ps(_mm_cvtepi32_ps(_mm_shuffle_epi8(inVec, vecC)), vecE));
            _mm_store_ps((float *)&toBuffer[4 * pix + 48], _mm_mul_ps(_mm_cvtepi32_ps(_mm_shuffle_epi8(inVec, vecD)), vecE));
        }
    } else if (depth() == sizeof(unsigned short)) {
        __m128i vecA = _mm_set_epi8(-1, -1, 7, 6, -1, -1, 5, 4, -1, -1, 3, 2, -1, -1, 1, 0);
        __m128i vecB = _mm_set_epi8(-1, -1, 15, 14, -1, -1, 13, 12, -1, -1, 11, 10, -1, -1, 9, 8);
        __m128 vecC = _mm_set1_ps(1.0f / 65535.0f);
        for (unsigned long long pix = 0; pix < length(); pix += 16) {
            __m128i inVec = _mm_loadu_si128((const __m128i *)&fmBuffer[pix]);
            _mm_store_ps((float *)&toBuffer[2 * pix + 0], _mm_mul_ps(_mm_cvtepi32_ps(_mm_shuffle_epi8(inVec, vecA)), vecC));
            _mm_store_ps((float *)&toBuffer[2 * pix + 16], _mm_mul_ps(_mm_cvtepi32_ps(_mm_shuffle_epi8(inVec, vecB)), vecC));
        }
    } else if (depth() == sizeof(float)) {
        memcpy(toBuffer, fmBuffer, length());
    }

    return (object);
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
int LAUMemoryObject::howManyDirectoriesDoesThisTiffFileHave(QString filename)
{
    // GET A FILE TO OPEN FROM THE USER IF NOT ALREADY PROVIDED ONE
    if (filename.isNull()) {
        return (-1);
    }

    // IF WE HAVE A VALID TIFF FILE, LOAD FROM DISK
    // OTHERWISE TRY TO CONNECT TO SCANNER
    if (QFile::exists(filename)) {
        // OPEN INPUT TIFF FILE FROM DISK
        TIFF *inTiff = TIFFOpen(filename.toLatin1(), "r");
        if (!inTiff) {
            return (-2);
        }
        int numDirectories = TIFFNumberOfDirectories(inTiff);
        TIFFClose(inTiff);
        return (numDirectories);
    }
    return (-3);
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
int LAUMemoryObject::howManyChannelsDoesThisTiffFileHave(QString filename, int frame)
{
    // CREATE A VARIABLE TO HOLD THE RETURN VALUE
    int numChannels = -1;

    // MAKE SURE WE HAVE A VALID FILE TO OPEN
    if (filename.isNull() == false && QFile::exists(filename)) {
        // OPEN INPUT TIFF FILE FROM DISK
        TIFF *inTiff = TIFFOpen(filename.toLatin1(), "r");
        if (!inTiff) {
            numChannels = -2;
        } else {
            int numDirectories = TIFFNumberOfDirectories(inTiff);
            if (frame < 0) {
                numChannels = -3;
            } else if (frame >= numDirectories) {
                numChannels = -4;
            } else {
                // LOAD INPUT TIFF FILE PARAMETERS IMPORTANT TO RESAMPLING THE IMAGE
                unsigned short uShortVariable;

                // SET THE DIRECTORY TO THE USER SUPPLIED FRAME
                TIFFSetDirectory(inTiff, (unsigned short)frame);
                TIFFGetField(inTiff, TIFFTAG_SAMPLESPERPIXEL, &uShortVariable);

                // COPY THE NUMBER OF CHANNELS OVER TO THE RETURN VARIABLE
                numChannels = (int)uShortVariable;
            }
            TIFFClose(inTiff);
        }
    }
    return (numChannels);
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
LAUMemoryObjectManager::~LAUMemoryObjectManager()
{
    framesAvailable.clear();
    qDebug() << QString("LAUMemoryObjectManager::~LAUMemoryObjectManager()");
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
void LAUMemoryObjectManager::onGetFrame()
{
    // CREATE NEW BUFFER IF NONE AVAILABLE, OTHERWISE TAKE FIRST AVAILABLE
    if (framesAvailable.isEmpty()) {
        emit emitFrame(LAUMemoryObject(numCols, numRows, numChns, numByts));
    } else {
        emit emitFrame(framesAvailable.takeFirst());
    }

    // NOW CREATE TWO NEW BUFFERS TO EVERY BUFFER RELEASED UP UNTIL
    // WE HAVE A MINIMUM NUMBER OF BUFFERS AVAILABLE FOR SUBSEQUENT CALLS
    if (framesAvailable.count() < MINNUMBEROFFRAMESAVAILABLE) {
        framesAvailable << LAUMemoryObject(numCols, numRows, numChns, numByts);
        framesAvailable << LAUMemoryObject(numCols, numRows, numChns, numByts);
    }
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
void LAUMemoryObjectManager::onReleaseFrame(LAUMemoryObject frame)
{
    // WE CAN EITHER ADD THE BUFFER BACK TO THE AVAILABLE LIST, IF THERE IS SPACE
    // OR OTHERWISE FREE THE MEMORY
    if (framesAvailable.count() < MAXNUMBEROFFRAMESAVAILABLE) {
        framesAvailable << frame;
    }
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
LAUMemoryObjectWriter::LAUMemoryObjectWriter(QString filename, LAUMemoryObject obj, QObject *parent) : QThread(parent), tiff(nullptr), object(obj)
{
    if (object.isValid()) {
        // LET THE USER SELECT A FILE FROM THE FILE DIALOG
        if (filename.isNull()) {
#ifndef HEADLESS
            QSettings settings;
            QString directory = settings.value("LAUMemoryObject::lastUsedDirectory", QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)).toString();
            filename = QFileDialog::getSaveFileName(nullptr, QString("Save image to disk (*.tif)"), directory);
            if (filename.isEmpty() == false) {
                if (filename.toLower().endsWith(".tif") == false && filename.toLower().endsWith(".tiff") == false) {
                    filename.append(".tif");
                }
                settings.setValue("LAUMemoryObject::lastUsedDirectory", QFileInfo(filename).absolutePath());
            } else {
                return;
            }
#else
            return;
#endif
        }

        // OPEN TIFF FILE FOR SAVING THE IMAGE USING BIGTIFF FOR LARGE FILES
        tiff = (object.length() > 1000000000) ? TIFFOpen(filename.toLatin1(), "w8") : TIFFOpen(filename.toLatin1(), "w");
    }
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
LAUMemoryObjectWriter::~LAUMemoryObjectWriter()
{
    qDebug() << QString("LAUMemoryObjectWriter::~LAUMemoryObjectWriter()");
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
void LAUMemoryObjectWriter::run()
{
    // MAKE SURE WE HAVE A VALID FILE TO WRITE TO
    if (tiff) {
        for (unsigned int frm = 0; frm < object.frames(); frm++) {
            qDebug() << "LAUMemoryObjectWriter::run()" << frm << object.frames();

            // WRITE FORMAT PARAMETERS TO CURRENT TIFF DIRECTORY
            TIFFSetField(tiff, TIFFTAG_SUBFILETYPE, FILETYPE_PAGE);
            TIFFSetField(tiff, TIFFTAG_IMAGEWIDTH, (unsigned long)object.width());
            TIFFSetField(tiff, TIFFTAG_IMAGELENGTH, (unsigned long)object.height());
            TIFFSetField(tiff, TIFFTAG_RESOLUTIONUNIT, RESUNIT_INCH);
            TIFFSetField(tiff, TIFFTAG_XRESOLUTION, 72.0);
            TIFFSetField(tiff, TIFFTAG_YRESOLUTION, 72.0);
            TIFFSetField(tiff, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
            TIFFSetField(tiff, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
            TIFFSetField(tiff, TIFFTAG_SAMPLESPERPIXEL, (unsigned short)object.colors());
            TIFFSetField(tiff, TIFFTAG_BITSPERSAMPLE, (unsigned short)(8 * object.depth()));
            TIFFSetField(tiff, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
            TIFFSetField(tiff, TIFFTAG_COMPRESSION, COMPRESSION_LZW);
            TIFFSetField(tiff, TIFFTAG_PREDICTOR, PREDICTOR_HORIZONTAL);
            TIFFSetField(tiff, TIFFTAG_ROWSPERSTRIP, 1);

            if (object.depth() == sizeof(float)) {
                // SEE IF WE HAVE TO TELL THE TIFF READER THAT WE ARE STORING
                // PIXELS IN 32-BIT FLOATING POINT FORMAT
                TIFFSetField(tiff, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_IEEEFP);
            }

            // MAKE TEMPORARY BUFFER TO HOLD CURRENT ROW BECAUSE COMPRESSION DESTROYS
            // WHATS EVER INSIDE THE BUFFER
            unsigned char *tempBuffer = (unsigned char *)malloc(object.step());
            for (unsigned int row = 0; row < object.height(); row++) {
                memcpy(tempBuffer, object.constScanLine(row, frm), object.step());
                TIFFWriteScanline(tiff, tempBuffer, row, 0);
            }
            free(tempBuffer);

            // WRITE THE CURRENT DIRECTORY AND PREPARE FOR THE NEW ONE
            TIFFWriteDirectory(tiff);

            // WRITE THE ELAPSED TIME STAMP TO AN EXIF TAG
            uint64_t dir_offset;
            TIFFCreateEXIFDirectory(tiff);
            TIFFSetField(tiff, EXIFTAG_SUBSECTIME, QString("%1").arg(object.elapsed()).toLatin1().data());
            TIFFWriteCustomDirectory(tiff, &dir_offset);

            TIFFSetDirectory(tiff, (unsigned short)frm);
            TIFFSetField(tiff, TIFFTAG_EXIFIFD, dir_offset);
            TIFFRewriteDirectory(tiff);
        }
        TIFFClose(tiff);
    }
    emit emitSaveComplete();
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
LAUMemoryObject LAUMemoryObject::minAreaFilter(int rad) const
{
    LAUMemoryObject object(*this);
    if (depth() == sizeof(unsigned char)) {
        for (unsigned int frm = 0; frm < frames(); frm++) {
            for (int row = 0; row < (int)height(); row++) {
                unsigned char *toBuffer = object.scanLine(row, frm);
                for (int col = 0; col < (int)width(); col++) {
                    for (int dy = -rad; dy <= rad; dy++) {
                        int rwp = qMin((int)height() - 1, qMax(0, row + dy));
                        unsigned char *fmBuffer = constScanLine(rwp, frm);
                        for (int dx = -rad; dx <= rad; dx++) {
                            int clp = qMin((int)width() - 1, qMax(0, col + dx));
                            for (unsigned int chn = 0; chn < colors(); chn++) {
                                toBuffer[col * colors() + chn] = qMin(toBuffer[col * colors() + chn], fmBuffer[clp * colors() + chn]);
                            }
                        }
                    }
                }
            }
        }
    } else if (depth() == sizeof(unsigned short)) {
        for (unsigned int frm = 0; frm < frames(); frm++) {
            for (int row = 0; row < (int)height(); row++) {
                unsigned short *toBuffer = (unsigned short *)object.scanLine(row, frm);
                for (int col = 0; col < (int)width(); col++) {
                    for (int dy = -rad; dy <= rad; dy++) {
                        int rwp = qMin((int)height() - 1, qMax(0, row + dy));
                        unsigned short *fmBuffer = (unsigned short *)constScanLine(rwp, frm);
                        for (int dx = -rad; dx <= rad; dx++) {
                            int clp = qMin((int)width() - 1, qMax(0, col + dx));
                            for (unsigned int chn = 0; chn < colors(); chn++) {
                                toBuffer[col * colors() + chn] = qMin(toBuffer[col * colors() + chn], fmBuffer[clp * colors() + chn]);
                            }
                        }
                    }
                }
            }
        }
    } else if (depth() == sizeof(float)) {
        for (unsigned int frm = 0; frm < frames(); frm++) {
            for (int row = 0; row < (int)height(); row++) {
                float *toBuffer = (float *)object.scanLine(row, frm);
                for (int col = 0; col < (int)width(); col++) {
                    for (int dy = -rad; dy <= rad; dy++) {
                        int rwp = qMin((int)height() - 1, qMax(0, row + dy));
                        float *fmBuffer = (float *)constScanLine(rwp, frm);
                        for (int dx = -rad; dx <= rad; dx++) {
                            int clp = qMin((int)width() - 1, qMax(0, col + dx));
                            for (unsigned int chn = 0; chn < colors(); chn++) {
                                toBuffer[col * colors() + chn] = qMin(toBuffer[col * colors() + chn], fmBuffer[clp * colors() + chn]);
                            }
                        }
                    }
                }
            }
        }
    }
    return (object);
}
