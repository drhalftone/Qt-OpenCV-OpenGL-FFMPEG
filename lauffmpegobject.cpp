#include "lauffmpegobject.h"

#include <QDebug>

LAUFFMpegObject::LAUFFMpegObject(QObject *parent) : QObject(parent)
{
    qDebug() << "AVCoded Version:" << avcodec_version();
}
