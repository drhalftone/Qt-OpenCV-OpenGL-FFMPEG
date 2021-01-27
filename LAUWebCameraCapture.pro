#-------------------------------------------------
#
# Project created by QtCreator 2017-12-24T17:00:22
#
#-------------------------------------------------

#CONFIG += visage

QT += core gui multimedia widgets multimediawidgets opengl
TARGET = LAUWebCamerarCapture
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
        lauwebcamerawidget.cpp \
        lauvideosurface.cpp \
        lauvideoglwidget.cpp \
        laufacialfeaturedetectorglwidget.cpp \
        lauharriscornerdetectorglwidget.cpp \
        laurandomizepixelsglwidget.cpp \
        lausobeledgedetectorglwidget.cpp

HEADERS += \
        lauwebcamerawidget.h \
        lauvideosurface.h \
        lauvideoglwidget.h \
        laufacialfeaturedetectorglwidget.h \
        lauharriscornerdetectorglwidget.h \
        laurandomizepixelsglwidget.h \
        lausobeledgedetectorglwidget.h

RESOURCES += lauwebcameracapture.qrc

unix:macx {
    CONFIG        += c++11
    INCLUDEPATH   += /usr/local/include/opencv4
    DEPENDPATH    += /usr/local/include/opencv4
    LIBS          += /usr/local/lib/libopencv_core.dylib /usr/local/lib/libopencv_objdetect.dylib
    LIBS          += /usr/local/lib/libopencv_imgproc.dylib /usr/local/lib/libopencv_calib3d.dylib
    LIBS          += /usr/local/lib/libopencv_highgui.dylib /usr/local/lib/libopencv_ml.dylib
    LIBS          += /usr/local/lib/libopencv_face.dylib /usr/local/lib/libtiff.5.dylib

    QMAKE_CXXFLAGS += -msse2 -msse3 -mssse3 -msse4.1

    QMAKE_INFO_PLIST = Info.plist

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
    LIBS        += -L$$quote(C:/usr/lib) -llibtiff_i -lopengl32

    INCLUDEPATH   += $$quote(C:/usr/opencv/include)
    DEPENDPATH    += $$quote(C:/usr/opencv/include)
    LIBS          += -L$$quote(C:/usr/opencv/x64/vc15/lib)
    CONFIG(release, debug|release): LIBS += -lopencv_core411 -lopencv_objdetect411 -lopencv_imgproc411 -lopencv_calib3d411 -lopencv_highgui411 -lopencv_ml411 -lopencv_face411
    CONFIG(debug, debug|release):   LIBS += -lopencv_core411d -lopencv_objdetect411d -lopencv_imgproc411d -lopencv_calib3d411d -lopencv_highgui411d -lopencv_ml411d -lopencv_face411d

    visage {
        DEFINES += USEVISAGE
        INCLUDEPATH   += $$quote(C:/usr/visageSDK/include)
        DEPENDPATH    += $$quote(C:/usr/visageSDK/include)
        LIBS += -L$$quote(C:/usr/visageSDK/lib) -llibVisageAnalyser64 -llibVisageGaze64 -llibVisageVision64
    }
}
