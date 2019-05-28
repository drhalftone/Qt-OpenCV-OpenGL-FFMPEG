#include "lauwebcamerawidget.h"

#include <QApplication>
#include <QSurfaceFormat>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName(QString("Lau Consulting Inc"));
    a.setOrganizationDomain(QString("drhalftone.com"));
    a.setApplicationName(QString("LAURealSense"));

    QSurfaceFormat format;
    format.setDepthBufferSize(10);
    format.setMajorVersion(4);
    format.setMinorVersion(1);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setRenderableType(QSurfaceFormat::OpenGL);
    QSurfaceFormat::setDefaultFormat(format);

    LAUWebCameraDialog w(QCamera::CaptureVideo);

    if (w.isValid()) {
        return w.exec();
    }
    return (0);
}
