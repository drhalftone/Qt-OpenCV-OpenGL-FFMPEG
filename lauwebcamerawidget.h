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
#include <QDialogButtonBox>
#include <QCameraImageCapture>

#include "lauvideosurface.h"
#include "laurandomizepixelsglwidget.h"
#include "lausobeledgedetectorglwidget.h"
#include "lauharriscornerdetectorglwidget.h"
#include "laufacialfeaturedetectorglwidget.h"

#define LAUWEBCAMERAWIDGETWIDTH  640
#define LAUWEBCAMERAWIDGETHEIGHT 480

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

public slots:
    void onCapture();
    void onImageAvailable(int id, QImage image);

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
    QCameraImageCapture *imageCapture;
    LAUVideoSurface *surface;
};

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
class LAUWebCameraDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LAUWebCameraDialog(QCamera::CaptureMode capture, QWidget *parent = 0) : QDialog(parent)
    {
        // CREATE A WIDGET TO WRAP AROUND
        widget = new LAUWebCameraWidget(capture);

        // SET THE LAYOUT AND DISPLAY OUR WIDGET INSIDE OF IT
        this->setWindowTitle(QString("Video Recorder"));
        this->setLayout(new QVBoxLayout());
        this->layout()->setContentsMargins(0, 0, 0, 0);
        this->layout()->addWidget(widget);

        QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
        connect(buttonBox->button(QDialogButtonBox::Ok), SIGNAL(clicked()), this, SLOT(accept()));
        connect(buttonBox->button(QDialogButtonBox::Cancel), SIGNAL(clicked()), this, SLOT(reject()));
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

protected:
    void accept()
    {
        widget->grabImage();
    }

private:
    LAUWebCameraWidget *widget;
};

#endif // LAUWEBCAMERAWIDGET_H
