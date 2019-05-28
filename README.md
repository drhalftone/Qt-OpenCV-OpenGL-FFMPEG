Welcome to the Qt-OpenCV-OpenGL wiki! This is a Qt application that displays live video from your webcam, and applies various video processing routines, namely OpenCV filters. However, unlike OpenCV which is CUDA centric, this project makes use of OpenGL shaders to replace as much of the CPU-based OpenCV filters. Users should see this project as a teaching tool for building their own filters. It should be noted that all of these filters run at 30 fps.

Here is an example of displaying raw video:

![](https://github.com/drhalftone/Qt-OpenCV-OpenGL/blob/master/Images/ScreenShot01.jpg)

Here is an example of Harris feature detection:

![](https://github.com/drhalftone/Qt-OpenCV-OpenGL/blob/master/Images/ScreenShot04.jpg)

Here is an example of Sobel edge detection:

![](https://github.com/drhalftone/Qt-OpenCV-OpenGL/blob/master/Images/ScreenShot02.jpg)

Here is an example of facial feature tracking.  For this to work, you will need to also download the pre-trained face detector model (https://github.com/opencv/opencv/blob/master/data/haarcascades/haarcascade_frontalface_alt2.xml) and the pre-trained facial feature point model (https://github.com/kurnianggoro/GSOC2017/blob/master/data/lbfmodel.yaml).  You will be prompted by a file dialog to locate these files on you system.

![](https://github.com/drhalftone/Qt-OpenCV-OpenGL/blob/master/Images/ScreenShot05.jpg)

Here is an example of randomly swapping pixels around (please note this takes a long time to launch):

![](https://github.com/drhalftone/Qt-OpenCV-OpenGL/blob/master/Images/ScreenShot03.jpg)
