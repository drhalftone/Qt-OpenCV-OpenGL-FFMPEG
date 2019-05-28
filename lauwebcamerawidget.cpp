#include "lauwebcamerawidget.h"

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
LAUWebCameraWidget::LAUWebCameraWidget(QCamera::CaptureMode capture, QWidget *parent) : QWidget(parent), mode(capture), thread(NULL), camera(NULL), imageCapture(NULL), surface(NULL)
{
    this->setLayout(new QVBoxLayout());
    this->layout()->setContentsMargins(6, 6, 6, 6);

    // ASK THE USER WHAT FILTER THEY WANT TO IMPLEMENT
    QStringList items;
    items << QString("Facial Features");
    items << QString("Harris Corners");
    items << QString("Randomized Pixels");
    items << QString("Raw Video");
    items << QString("Sobel Edges");
    QString string = QInputDialog::getItem(nullptr, QString("Web Camera Widget"), QString("Select video filter"), items);

    if (string == QString("Facial Features")) {
        label = new LAUFacialFeatureDetectorGLWidget();
    } else if (string == QString("Harris Corners")) {
        label = new LAUHarrisCornerDetectorGLWidget();
    } else if (string == QString("Randomized Pixels")) {
        label = new LAURandomizePixelsGLWidget();
    } else if (string == QString("Raw Video")) {
        label = new LAUVideoGLWidget();
    } else if (string == QString("Sobel Edges")) {
        label = new LAUSobelEdgeDetectorGLWidget();
    }
    label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->layout()->addWidget(label);

    QStringList strings;
    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
    for (int n = 0; n < cameras.count(); n++) {
        strings << cameras.at(n).description();
    }

    if (strings.count() > 1) {
        bool okay = false;
        QString string = QInputDialog::getItem(this, QString("Select Camera"), QString("Select input device"), strings, 0, false, &okay);
        if (okay) {
            int n = strings.indexOf(string);
            camera = new QCamera(cameras.at(n));
        }
    } else if (strings.count() == 1) {
        camera = new QCamera(cameras.first());
    }

    if (camera) {
        surface = new LAUVideoSurface();
        surface->setLabel(label);

        QCameraViewfinderSettings set = camera->viewfinderSettings();
        set.setResolution(LAUWEBCAMERAWIDGETWIDTH, LAUWEBCAMERAWIDGETHEIGHT);
        set.setMaximumFrameRate(30.0);
        set.setMinimumFrameRate(30.0);
        set.setPixelFormat(QVideoFrame::Format_ARGB32);

        camera->setViewfinderSettings(set);
        camera->setViewfinder(surface);
        camera->setCaptureMode(mode);

        if (mode == QCamera::CaptureStillImage) {
            imageCapture = new QCameraImageCapture(camera);
            imageCapture->setCaptureDestination(QCameraImageCapture::CaptureToBuffer);
            connect(imageCapture, SIGNAL(imageCaptured(int, QImage)), this, SLOT(onImageAvailable(int, QImage)));
        }

        // CREATE A NEW THREAD TO HOST THE CAMERA
        thread = new QThread();
        camera->moveToThread(thread);
        thread->start();
    }
    label->setMinimumSize(qMin(LAUWEBCAMERAWIDGETWIDTH, 640), qMin(LAUWEBCAMERAWIDGETHEIGHT, 480));
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
LAUWebCameraWidget::~LAUWebCameraWidget()
{
    if (thread) {
        thread->quit();
        while (thread->isRunning()) {
            qApp->processEvents();
        }
        delete thread;
    }

    if (surface) {
        surface->stop();
        delete surface;
    }

    if (camera) {
        camera->stop();
        delete camera;
    }
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
void LAUWebCameraWidget::onCapture()
{
    if (imageCapture) {
        // WAIT HERE UNTIL CAMERA IS READY TO CAPTURE
        while (imageCapture->isReadyForCapture() == false) {
            qApp->processEvents();
        }
        imageCapture->capture();
        if (imageCapture->error() != QCameraImageCapture::NoError) {
            qDebug() << imageCapture->errorString();
        }
    }
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
void LAUWebCameraWidget::onImageAvailable(int id, QImage image)
{
    Q_UNUSED(id);

    QLabel *label = new QLabel();
    label->setPixmap(QPixmap::fromImage(image));
    label->show();
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
void LAUWebCameraWidget::grabImage()
{
    if (label) {
        QImage image = label->grabImage();
        if (image.isNull() == false) {
            QSettings settings;
            QString directory = settings.value("LAUWebCameraWidget::lastUsedDirectory", QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)).toString();
            QString filename = QFileDialog::getSaveFileName(0, QString("Save image to disk (*.tif)"), directory, QString("*.tif *.tiff"));
            if (filename.isEmpty() == false) {
                settings.setValue("LAUWebCameraWidget::lastUsedDirectory", QFileInfo(filename).absolutePath());
                if (filename.toLower().endsWith(".tif") == false && filename.toLower().endsWith(".tiff")) {
                    filename.append(".tif");
                }
            } else {
                return;
            }
            image.save(filename, "TIFF");
        }
    }
}
