#include "lauwebcamerawidget.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QSettings>

QUrl LAUWebCameraWidget::localURL = QUrl::fromLocalFile(QString("%1/videofile.mp4").arg(QStandardPaths::writableLocation(QStandardPaths::TempLocation)));

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
LAUWebCameraWidget::LAUWebCameraWidget(QCamera::CaptureMode capture, QWidget *parent) : QWidget(parent), mode(capture), thread(NULL), camera(NULL), recorder(NULL), imageCapture(NULL), surface(NULL)
{
    // SEE IF THERE IS A LEFTOVER VIDEO FILE FROM A PREVIOUS RUN OF THE SOFTWARE
    saveVideoFile();

    this->setLayout(new QVBoxLayout());
    this->layout()->setContentsMargins(6, 6, 6, 6);

    // ASK THE USER WHAT FILTER THEY WANT TO IMPLEMENT
    QStringList items;
    items << QString("Facial Features");
    items << QString("Harris Corners");
    items << QString("Randomized Pixels");
    items << QString("Raw Video");
    items << QString("Sobel Edges");

    bool ok = false;
    QString string = QInputDialog::getItem(nullptr, QString("Web Camera Widget"), QString("Select video filter"), items, 3, false, &ok);

    if (ok) {
        if (string == QString("Raw Video")) {
            label = new LAUVideoGLWidget();
        } else if (string == QString("Facial Features")) {
            label = new LAUFacialFeatureDetectorGLWidget();
        } else if (string == QString("Harris Corners")) {
            label = new LAUHarrisCornerDetectorGLWidget();
        } else if (string == QString("Randomized Pixels")) {
            label = new LAURandomizePixelsGLWidget();
        } else if (string == QString("Sobel Edges")) {
            label = new LAUSobelEdgeDetectorGLWidget();
        }
    } else {
        label = new LAUVideoGLWidget();
    }
#ifdef Q_OS_WIN
    label->setVideoRecorder(&recorder);
#endif
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

        //QList<QSize> resolutions = camera->supportedViewfinderResolutions(set);
        //QList<QVideoFrame::PixelFormat> formats = camera->supportedViewfinderPixelFormats(set);
        //QList<QCamera::FrameRateRange> ranges = camera->supportedViewfinderFrameRateRanges(set);

        set.setResolution(LAUWEBCAMERAWIDGETWIDTH, LAUWEBCAMERAWIDGETHEIGHT);
        set.setMaximumFrameRate(LAUWEBCAMERAWIDGETFPS);
        set.setMinimumFrameRate(LAUWEBCAMERAWIDGETFPS);
        //set.setPixelFormat(QVideoFrame::Format_ARGB32);

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

    // DELETE TEMPORARY VIDEO RECORDING FILE IF IT EXISTS
    if (QFile::exists(localURL.toLocalFile())) {
        QFile::remove(localURL.toLocalFile());
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
void LAUWebCameraWidget::onTriggerVideo(bool state)
{
    qDebug() << "Trigger video recording:" << state;

    if (recorder) {
#ifndef Q_OS_WIN
        // GET OUTPUT LOCATION
        localURL = recorder->outputLocation();

        // STOP RECORDING AND DELETE THE RECORDER
        recorder->stop();
#endif
        // DELETE THE RECORDER
#ifdef Q_OS_WIN
        delete recorder;
#else
        recorder->deleteLater();
#endif
        recorder = nullptr;

        // LET THE USER SAVE THE VIDEO FILE TO DISK
        saveVideoFile();
    } else {
#ifdef Q_OS_WIN
        recorder = new cv::VideoWriter();
        if (recorder->open(localURL.toString().toStdString(), cv::VideoWriter::fourcc('M','J','P','G'), 10.0, cv::Size(LAUWEBCAMERAWIDGETWIDTH, LAUWEBCAMERAWIDGETHEIGHT), true)){
            qDebug() << "Recording to file:" << localURL.toString();
        }
#else
        // CREATE NEW RECORDER
        recorder = new QMediaRecorder(camera);

        // SET AUDIO PARAMETERS
        QAudioEncoderSettings audioSettings;
        audioSettings.setCodec("audio/amr");
        audioSettings.setQuality(QMultimedia::HighQuality);
        recorder->setAudioSettings(audioSettings);

        // SET THE SINK
        recorder->setOutputLocation(localURL);
        recorder->record();
#endif
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

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
bool LAUWebCameraWidget::saveVideoFile()
{
    // MAKE SURE TEMPORARY VIDEO FILE EXISTS
    if (QFile::exists(localURL.toLocalFile()) == false) {
        return (false);
    }

    // GET THE LAST USED DIRECTORY FROM SETTINGS
    QSettings settings;
    QString directory = settings.value("LAUWebCameraWidget::lastUsedDirectory", QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)).toString();
    if (QDir().exists(directory) == false) {
        directory = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    }

    QString filename;
    for (int counter = 0; counter < 1000; counter++) {
        if (counter < 10) {
            filename = QString("%1/intubation00%2.mp4").arg(directory).arg(counter);
        } else if (counter < 100) {
            filename = QString("%1/intubation0%2.mp4").arg(directory).arg(counter);
        } else {
            filename = QString("%1/intubation%2.mp4").arg(directory).arg(counter);
        }

        if (QFile::exists(filename) == false) {
            break;
        }
    }

    while (1) {
        // COPY TO A USER SPECIFIED FILE
        filename = QFileDialog::getSaveFileName(nullptr, QString("Save video to disk (*.mp4)"), filename, QString("*.mp4"));
        if (filename.isEmpty() == false) {
            if (filename.toLower().endsWith(".mp4") == false) {
                filename.append(".mp4");
            }
            settings.setValue("LAUWebCameraWidget::lastUsedDirectory", QFileInfo(filename).absolutePath());

            // RENAME THE TEMPORARY RECORDING TO A PERMANENT FILE
            return (QFile::rename(localURL.toLocalFile(), filename));
        }

        // GIVE THE USER ANOTHER CHANCE
        if (QMessageBox::warning(this, QString("Webcam Recorder"), QString("You are about to discard the recording and lose the data forever.\n\nDo you want to do this?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes) {
            if (QMessageBox::warning(this, QString("Webcam Recorder"), QString("Are you sure?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes) {
                return (false);
            }
        }
    }
}
