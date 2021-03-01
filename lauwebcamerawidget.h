#ifndef LAUWEBCAMERAWIDGET_H
#define LAUWEBCAMERAWIDGET_H

#include <QList>
#include <QTimer>
#include <QLabel>
#include <QImage>
#include <QCamera>
#include <QWidget>
#include <QThread>
#include <QSettings>
#include <QFileDialog>
#include <QPushButton>
#include <QVideoFrame>
#include <QStringList>
#include <QVBoxLayout>
#include <QCameraInfo>
#include <QInputDialog>
#include <QApplication>
#include <QMediaRecorder>
#include <QDialogButtonBox>
#include <QCameraImageCapture>

#include "lauvideosurface.h"
#include "laurandomizepixelsglwidget.h"
#include "lausobeledgedetectorglwidget.h"
#include "lauharriscornerdetectorglwidget.h"
#include "laufacialfeaturedetectorglwidget.h"

#define LAUWEBCAMERAWIDGETWIDTH  640
#define LAUWEBCAMERAWIDGETHEIGHT 480
#define LAUWEBCAMERAWIDGETFPS     30.0f

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
class LAUWebCameraWidget : public QWidget
{
    Q_OBJECT

public:
    LAUWebCameraWidget(QCamera::CaptureMode capture, QWidget *parent = 0);
    ~LAUWebCameraWidget();

    bool isValid() const
    {
        return (camera);
    }

    bool isNull() const
    {
        return (!isValid());
    }

    void grabImage();
    bool saveVideoFile();

public slots:
    void onCapture();
    void onImageAvailable(int id, QImage image);
    void onTriggerVideo(bool state);

protected:
    void showEvent(QShowEvent *)
    {
        if (camera) {
            camera->start();
            QTimer::singleShot(1000, this, SLOT(onCapture()));
        }
    }

private:
    QCamera::CaptureMode mode;
    LAUVideoGLWidget *label;
    QThread *thread;
    QCamera *camera;
#ifdef Q_OS_WIN
    cv::VideoWriter *recorder;
#else
    QMediaRecorder *recorder;
#endif
    QCameraImageCapture *imageCapture;
    LAUVideoSurface *surface;

    static QUrl localURL;
};

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
class LAUWebCameraDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LAUWebCameraDialog(QCamera::CaptureMode capture, QWidget *parent = 0) : QDialog(parent), buttonBox(nullptr)
    {
        // CREATE A WIDGET TO WRAP AROUND
        widget = new LAUWebCameraWidget(capture);

        // SET THE LAYOUT AND DISPLAY OUR WIDGET INSIDE OF IT
        this->setWindowTitle(QString("Video Recorder"));
        this->setLayout(new QVBoxLayout());
        this->layout()->setContentsMargins(6, 6, 6, 6);
        this->layout()->addWidget(widget);

        buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
        connect(buttonBox->button(QDialogButtonBox::Ok), SIGNAL(clicked()), this, SLOT(accept()));
        connect(buttonBox->button(QDialogButtonBox::Cancel), SIGNAL(clicked()), this, SLOT(reject()));

        buttonBox->button(QDialogButtonBox::Ok)->setText(QString("Snapshot"));
        buttonBox->button(QDialogButtonBox::Cancel)->setText(QString("Quit"));

        QPushButton *button = buttonBox->addButton(QString("Record"), QDialogButtonBox::ActionRole);
        button->setCheckable(true);
        button->setChecked(false);
        connect(button, SIGNAL(clicked(bool)), this, SLOT(onTriggerVideo(bool)));

        this->layout()->addWidget(buttonBox);
    }

    bool isValid() const
    {
        return (widget->isValid());
    }

    bool isNull() const
    {
        return (widget->isNull());
    }

public slots:
    void onTriggerVideo(bool state)
    {
        if (buttonBox) {
            buttonBox->button(QDialogButtonBox::Ok)->setDisabled(state);
            buttonBox->button(QDialogButtonBox::Cancel)->setDisabled(state);
        }

        if (widget) {
            widget->onTriggerVideo(state);
        }
    }

protected:
    void accept()
    {
        widget->grabImage();
    }

private:
    LAUWebCameraWidget *widget;
    QDialogButtonBox *buttonBox;
};

#endif // LAUWEBCAMERAWIDGET_H
