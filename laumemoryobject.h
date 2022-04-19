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

#ifndef LAUMEMORYOBJECT_H
#define LAUMEMORYOBJECT_H

#ifndef Q_PROCESSOR_ARM
#include "emmintrin.h"
#include "xmmintrin.h"
#include "tmmintrin.h"
#include "smmintrin.h"
#endif

#include <QRgb>
#include <QTime>
#include <QImage>
#include <QtCore>
#include <QDebug>
#include <QObject>
#include <QString>
#include <QMatrix4x4>
#include <QSharedData>
#include <QSharedDataPointer>

#ifndef HEADLESS
#include <QFileDialog>
#endif

namespace libtiff
{
#include "tiffio.h"
}

namespace LAU3DVideoParameters
{
    enum LAUVideoPlaybackState  { StateLiveVideo, StateVideoPlayback };
    enum LAUVideoPlaybackDevice { DeviceUndefined, DeviceKinect, DevicePrimeSense, DeviceProsilicaLCG, DeviceProsilicaDPR, DeviceProsilicaIOS, Device2DCamera, DeviceProsilicaARG, DeviceProsilicaPST, DeviceProsilicaAST, DeviceProsilicaGRY, DeviceProsilicaRGB, DeviceXimea, DeviceIDS, DeviceRealSense, DeviceLucid, DeviceDemo };
    enum LAUVideoPlaybackColor  { ColorUndefined, ColorGray, ColorRGB, ColorRGBA, ColorXYZ, ColorXYZW, ColorXYZG, ColorXYZRGB, ColorXYZWRGBA };
    enum LAUVideoPatternSequence { SequenceNone, SequenceCustom, SequenceUnitFrequency, SequenceTwoFrequency, SequenceThreeFrequency, SequenceDualFrequency, SequenceCalibration, SequenceMultipath, SequenceTiming };
    enum LAUVideoProjector { ProjectorLC4500, ProjectorLC3000, ProjectorTI2010, ProjectorML500, ProjectoML750ST, ProjectorUnknown };
    enum LAUVideoProjectorSynchronizationMode { ModeSlave, ModeMaster, ModeMono, ModeMasterHandshake, ModeHDMIFPGA, ModeFPGA };
    enum LAUVideoPatternSynchronizationScheme { SchemeFlashingSequence, SchemePatternBit, SchemeNone };

    int colors(LAUVideoPlaybackColor clr);
    bool isMachineVision(LAUVideoPlaybackDevice dvc);
}

using namespace LAU3DVideoParameters;

#define MINNUMBEROFFRAMESAVAILABLE      40
#define MAXNUMBEROFFRAMESAVAILABLE      100

void myTIFFWarningHandler(const char *stringA, const char *stringB, va_list args);
void myTIFFErrorHandler(const char *stringA, const char *stringB, va_list args);

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
class LAUMemoryObjectData : public QSharedData
{
public:
    LAUMemoryObjectData();
    LAUMemoryObjectData(const LAUMemoryObjectData &other);
    LAUMemoryObjectData(unsigned int cols, unsigned int rows, unsigned int chns = 1, unsigned int byts = 1, unsigned int frms = 1);
    LAUMemoryObjectData(unsigned long long bytes);

    ~LAUMemoryObjectData();

    static int instanceCounter;

    unsigned int numRows, numCols, numChns, numFrms, numByts;
    unsigned int stepBytes, frameBytes;
    unsigned long long numBytesTotal;
    void *buffer;

    QString *rfidString;
    QMatrix4x4 *transformMatrix;
    QPoint *anchorPt;
    unsigned int *elapsedTime;

    void allocateBuffer();
};

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
class LAUMemoryObject
{
public:
    LAUMemoryObject()
    {
        data = new LAUMemoryObjectData();
    }

    LAUMemoryObject(unsigned int cols, unsigned int rows, unsigned int chns = 1, unsigned int byts = 1, unsigned int frms = 1)
    {
        data = new LAUMemoryObjectData(cols, rows, chns, byts, frms);
    }

    LAUMemoryObject(unsigned long long bytes)
    {
        data = new LAUMemoryObjectData(bytes);
    }

    LAUMemoryObject(const LAUMemoryObject &other) : data(other.data) { ; }

    LAUMemoryObject &operator = (const LAUMemoryObject &other)
    {
        if (this != &other) {
            data = other.data;
        }
        return (*this);
    }

    LAUMemoryObject(QImage image);
    LAUMemoryObject(QString filename, int index = -1);
    LAUMemoryObject(libtiff::TIFF *inTiff, int index = -1);

    bool save(QString filename = QString()) const;
    bool save(libtiff::TIFF *otTiff, int index = 0) const;
    bool load(libtiff::TIFF *inTiff, int index = -1);

    // LOAD INTO READS A FILE INTO THE EXISTING BUFFER BUT ALL
    // SIZE PARAMETERS MUST BE SAME OTHERWISE RETURN FALSE
    bool loadInto(libtiff::TIFF *inTiff, int index = -1);
    bool loadInto(QString filename, int index = -1);

    unsigned int nonZeroPixelsCount() const;
    LAUMemoryObject toFloat();
    LAUMemoryObject minAreaFilter(int rad) const;

    // SEE IF THE POINTERS ARE LOOKING AT SAME MEMORY
    bool operator == (const LAUMemoryObject &other) const
    {
        if (this == &other) {
            return (true);
        }
        return (data->buffer == other.data->buffer);
    }

    bool operator  < (const LAUMemoryObject &other) const
    {
        if (this == &other) {
            return (false);
        }
        return (elapsed() < other.elapsed());
    }

    bool operator  > (const LAUMemoryObject &other) const
    {
        if (this == &other) {
            return (false);
        }
        return (elapsed() > other.elapsed());
    }

    bool operator <= (const LAUMemoryObject &other) const
    {
        if (this == &other) {
            return (true);
        }
        return (elapsed() <= other.elapsed());
    }

    bool operator >= (const LAUMemoryObject &other) const
    {
        if (this == &other) {
            return (true);
        }
        return (elapsed() >= other.elapsed());
    }

    ~LAUMemoryObject() { ; }

    inline bool isNull() const
    {
        return (data->buffer == nullptr);
    }

    inline bool isValid() const
    {
        return (data->buffer != nullptr);
    }

    inline unsigned long long length() const
    {
        return (data->numBytesTotal);
    }

    inline QSize size() const
    {
        return (QSize(width(), height()));
    }

    inline unsigned int nugget() const
    {
        return (data->numChns * data->numByts);
    }

    inline unsigned int width() const
    {
        return (data->numCols);
    }

    inline unsigned int height() const
    {
        return (data->numRows);
    }

    inline unsigned int depth() const
    {
        return (data->numByts);
    }

    inline unsigned int colors() const
    {
        return (data->numChns);
    }

    inline unsigned int frames()  const
    {
        return (data->numFrms);
    }

    inline unsigned int step()    const
    {
        return (data->stepBytes);
    }

    inline unsigned long long block()   const
    {
        return (data->frameBytes);
    }

    inline unsigned char *pointer()
    {
        return (scanLine(0));
    }

    inline unsigned char *constPointer() const
    {
        return (constScanLine(0));
    }

    inline unsigned char *scanLine(unsigned int row, unsigned int frame = 0)
    {
        return (&(((unsigned char *)(data->buffer))[frame * block() + row * step()]));
    }

    inline unsigned char *constScanLine(unsigned int row, unsigned int frame = 0) const
    {
        return (&(((unsigned char *)(data->buffer))[frame * block() + row * step()]));
    }

    inline unsigned char *frame(unsigned int frm = 0)
    {
        return (scanLine(0, frm));
    }

    inline unsigned char *constFrame(unsigned int frm = 0) const
    {
        return (constScanLine(0, frm));
    }

    inline QString rfid() const
    {
        if (data->rfidString) {
            return (*(data->rfidString));
        } else {
            return (QString("RFID String wasn't allocated!"));
        }
    }

    inline void setRFID(QString string)
    {
        if (data->rfidString) {
            data->rfidString->clear();
            data->rfidString->append(string);
        }
    }

    inline void setConstRFID(QString string) const
    {
        if (data->rfidString) {
            data->rfidString->clear();
            data->rfidString->append(string);
        }
    }

    inline QMatrix4x4 transform() const
    {
        if (data->transformMatrix) {
            return (*(data->transformMatrix));
        } else {
            return (QMatrix4x4());
        }
    }

    inline void setTransform(QMatrix4x4 mat)
    {
        if (data->transformMatrix) {
            memcpy((void *)(data->transformMatrix->data()), (void *)mat.constData(), sizeof(QMatrix4x4));
        }
    }

    inline void setConstTransform(QMatrix4x4 mat) const
    {
        if (data->transformMatrix) {
            memcpy((void *)(data->transformMatrix->data()), (void *)mat.constData(), sizeof(QMatrix4x4));
        }
    }

    inline unsigned int elapsed() const
    {
        if (data->elapsedTime) {
            return (*data->elapsedTime);
        } else {
            return (0);
        }
    }

    inline void setElapsed(unsigned int elps)
    {
        if (data->elapsedTime) {
            *data->elapsedTime = elps;
        }
    }

    inline void setConstElapsed(unsigned int elps) const
    {
        if (data->elapsedTime) {
            *data->elapsedTime = elps;
        }
    }

    inline QPoint anchor() const
    {
        if (data->anchorPt) {
            return (*data->anchorPt);
        } else {
            return (QPoint());
        }
    }

    inline void setAnchor(QPoint pt)
    {
        if (data->anchorPt) {
            data->anchorPt->setX(pt.x());
            data->anchorPt->setY(pt.y());
        }
    }

    inline void setConstAnchor(QPoint pt) const
    {
        if (data->anchorPt) {
            data->anchorPt->setX(pt.x());
            data->anchorPt->setY(pt.y());
        }
    }

    static int numberOfColors(LAUVideoPlaybackColor color)
    {
        switch (color) {
            case ColorGray:
                return (1);
            case ColorRGB:
            case ColorXYZ:
                return (3);
            case ColorRGBA:
            case ColorXYZG:
                return (4);
            case ColorXYZRGB:
                return (6);
            case ColorXYZWRGBA:
                return (8);
            default:
                return (-1);
        }
    }

    static QString lastTiffErrorString;
    static QString lastTiffWarningString;
    static int howManyDirectoriesDoesThisTiffFileHave(QString filename);
    static int howManyChannelsDoesThisTiffFileHave(QString filename, int frame);

protected:
    QSharedDataPointer<LAUMemoryObjectData> data;
};

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
class LAUMemoryObjectManager : public QObject
{
    Q_OBJECT

public:
    explicit LAUMemoryObjectManager(unsigned int cols = 0, unsigned int rows = 0, unsigned int chns = 0, unsigned int byts = 0, unsigned int frms = 0, QObject *parent = 0) : QObject(parent), numRows(rows), numCols(cols), numChns(chns), numByts(byts), numFrms(frms) { ; }
    ~LAUMemoryObjectManager();

public slots:
    void onGetFrame();
    void onReleaseFrame(LAUMemoryObject frame);

private:
    unsigned int numRows, numCols, numChns, numByts, numFrms;
    QList<LAUMemoryObject> framesAvailable;

signals:
    void emitFrame(LAUMemoryObject frame);
};

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
class LAUModalityObject
{
public:
    LAUModalityObject(LAUMemoryObject dpt = LAUMemoryObject(), LAUMemoryObject clr = LAUMemoryObject(), LAUMemoryObject map = LAUMemoryObject()) : depth(dpt), color(clr), mappi(map) { ; }
    LAUModalityObject(const LAUModalityObject &other) : depth(other.depth), color(other.color), mappi(other.mappi) { ; }
    LAUModalityObject &operator = (const LAUModalityObject &other)
    {
        if (this != &other) {
            depth = other.depth;
            color = other.color;
            mappi = other.mappi;
        }
        return (*this);
    }

    bool isAnyValid()
    {
        return (depth.isValid() || color.isValid() || mappi.isValid());
    }

    LAUMemoryObject depth;
    LAUMemoryObject color;
    LAUMemoryObject mappi;
};

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
class LAUMemoryObjectWriter : public QThread
{
    Q_OBJECT

public:
    explicit LAUMemoryObjectWriter(QString flnm, LAUMemoryObject obj, QObject *parent = nullptr);
    ~LAUMemoryObjectWriter();

    bool isNull() const
    {
        return (!isValid());
    }

    bool isValid() const
    {
        return (tiff != nullptr);
    }

protected:
    void run();

private:
    libtiff::TIFF *tiff;
    LAUMemoryObject object;

signals:
    void emitSaveComplete();
};

Q_DECLARE_METATYPE(LAUMemoryObject);

#endif // LAUMEMORYOBJECT_H
