#ifndef LAUFFMPEGOBJECT_H
#define LAUFFMPEGOBJECT_H

#include <QObject>

extern "C"
{
#include "libavutil/opt.h"
#include "libavutil/imgutils.h"
#include "libavcodec/avcodec.h"
#include "libavutil/mathematics.h"
}

class LAUFFMpegObject : public QObject
{
    Q_OBJECT

public:
    explicit LAUFFMpegObject(QObject *parent = nullptr);

private:


signals:

};

#endif // LAUFFMPEGOBJECT_H
