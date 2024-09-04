#-------------------------------------------------
#
# Project created by QtCreator 2017-12-24T17:00:22
#
#-------------------------------------------------

CONFIG -= visage
CONFIG -= ffmpeg

QT += core gui multimedia widgets multimediawidgets opengl
TARGET = LAUWebCamerasCapture
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

QMAKE_CXXFLAGS += -fdeclspec

SOURCES += \
        main.cpp \
        laumemoryobject.cpp \
        lauvideosurface.cpp \
        lauvideoglwidget.cpp \
        lauwebcamerawidget.cpp \
        lautiredetectorglfilter.cpp \
        laurandomizepixelsglwidget.cpp \
        lausobeledgedetectorglwidget.cpp \
        lauharriscornerdetectorglwidget.cpp \
        lauhistogramequalizationglwidget.cpp \
        laufacialfeaturedetectorglwidget.cpp

HEADERS += \
        laumemoryobject.h \
        lauvideosurface.h \
        lauvideoglwidget.h \
        lauwebcamerawidget.h \
        lautiredetectorglfilter.h \
        laurandomizepixelsglwidget.h \
        lausobeledgedetectorglwidget.h \
        lauharriscornerdetectorglwidget.h \
        lauhistogramequalizationglwidget.h \
        laufacialfeaturedetectorglwidget.h

RESOURCES += lauwebcameracapture.qrc

ffmpeg {
    HEADERS += lauffmpegobject.h
    SOURCES += lauffmpegobject.cpp
}

unix:macx {
    CONFIG        += c++11
    INCLUDEPATH   += /usr/local/include/opencv4 /usr/local/include/TIFF
    DEPENDPATH    += /usr/local/include/opencv4 /usr/local/include/TIFF
    LIBS          += /usr/local/lib/libopencv_core.dylib /usr/local/lib/libopencv_objdetect.dylib
    LIBS          += /usr/local/lib/libopencv_imgproc.dylib /usr/local/lib/libopencv_calib3d.dylib
    LIBS          += /usr/local/lib/libopencv_highgui.dylib /usr/local/lib/libopencv_ml.dylib
    LIBS          += /usr/local/lib/libopencv_face.dylib /usr/local/lib/libtiff.5.dylib

    QMAKE_CXXFLAGS += -msse2 -msse3 -mssse3 -msse4.1

    QMAKE_INFO_PLIST = Info.plist

    ffmpeg {
        INCLUDEPATH   += /usr/local/include
        DEPENDPATH    += /usr/local/include
        LIBS          += /usr/local/lib/libavcodec.dylib /usr/local/lib/libavdevice.dylib
        LIBS          += /usr/local/lib/libavfilter.dylib /usr/local/lib/libavformat.dylib
        LIBS          += /usr/local/lib/libavutil.dylib /usr/local/lib/libopus.dylib
        LIBS          += /usr/local/lib/libswresample.dylib /usr/local/lib/libswscale.dylib
    }

    visage {
        DEFINES += USEVISAGE
        INCLUDEPATH += $$PWD/../visageSDK-macOS/include
        DEPENDPATH += $$PWD/../visageSDK-macOS/include
        LIBS += -framework CoreFoundation -framework Foundation -framework AppKit -framework Accelerate -L$$PWD/../visageSDK-macOS/lib -lVisageAnalyser -lVisageGaze -lVisageVision
    }
}

unix:!macx {
    CONFIG        += c++11
    INCLUDEPATH   += /usr/local/opt/opencv/include
    DEPENDPATH    += /usr/local/opt/opencv/include
    LIBS          += -L/usr/local/lib -lopencv_core -lopencv_objdetect -lopencv_imgproc -lopencv_calib3d -lopencv_highgui -lopencv_ml
}

win32 {
    INCLUDEPATH += $$quote(C:/usr/include)
    DEPENDPATH  += $$quote(C:/usr/include)
    LIBS        += -L$$quote(C:/usr/lib) -ltiff -lopengl32
    CONFIG      += c++11

    INCLUDEPATH   += $$quote(C:/usr/opencv/include)
    DEPENDPATH    += $$quote(C:/usr/opencv/include)
    LIBS          += -L$$quote(C:/usr/opencv/x64/vc17/lib)
    CONFIG(release, debug|release): LIBS += -lopencv_core490 -lopencv_objdetect490 -lopencv_imgproc490 -lopencv_calib3d490 -lopencv_highgui490 -lopencv_ml490 -lopencv_face490 -lopencv_videoio490
    CONFIG(debug, debug|release):   LIBS += -lopencv_core490d -lopencv_objdetect490d -lopencv_imgproc490d -lopencv_calib3d490d -lopencv_highgui490d -lopencv_ml490d -lopencv_face490d -lopencv_videoio490d

    # GET WINDOWS LIBRARIES FROM https://github.com/mcmtroffaes/ffmpeg-msvc-build
    ffmpeg {
        INCLUDEPATH += $$quote(C:/usr/ffmpeg/include)
        DEPENDPATH  += $$quote(C:/usr/ffmpeg/include)
        LIBS        += -lmf -lmfplat -lmfplay -lmfreadwrite -lmfuuid -lStrmiids -lole32 -luser32 -lBcrypt
        LIBS        += -L$$quote(C:/usr/ffmpeg/debug/lib) -lavcodec -lavdevice -lavfilter -lavformat -lavutil -lopus -lswresample -lswscale -lvpxmdd
    }

    visage {
        DEFINES += USEVISAGE
        INCLUDEPATH   += $$quote(C:/usr/visageSDK/include)
        DEPENDPATH    += $$quote(C:/usr/visageSDK/include)
        LIBS += -L$$quote(C:/usr/visageSDK/lib) -llibVisageAnalyser64 -llibVisageGaze64 -llibVisageVision64
    }
}
