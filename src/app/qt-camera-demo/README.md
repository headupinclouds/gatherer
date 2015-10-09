Qt Camera Demo v1.2.1
=====================

Qt Camera Demo is an example application that demonstrates the use of Qt
Mobility Qt camera classes to access the camera devices. The application has
capability to set properties of the camera, show view finder image, and capture
still images and video. The application also analyses the view finder image in
real time and shows a histogram window about the image. The over-exposed areas
are marked in the view finder to demonstrate the pixel manipulation techniques
in real-time images.

This example application is hosted on Nokia Developer Projects:
- http://projects.developer.nokia.com/camerademo

This example application demonstrates:
- Qt Mobility Qt camera classes
- Pixel manipulation of the view finder image in real-time

For more information on implementation and porting, visit the project's wiki
pages:
- http://projects.developer.nokia.com/camerademo/wiki

What's new
----------

The application now has both still image and video recording modes.


1. Usage
-------------------------------------------------------------------------------

The Qt Camera Demo relies on using the Qt Mobility Qt camera classes to access
the camera devices. The QCamera and related objects are wrapped into a custom
implemented QDeclarativeItem (QML element) so they can be accessed easily from
QML. The view finder image is drawn to the bounding rectangle of the custom QML
element by using the QAbstractVideoSurface to get the view finder frames and
drawing them with the method QDeclarativeItem::paint.

The example shows some ways to manipulate the view finder image in real time
by using pixel manipulation. This is done before the view finder image is
drawn.

The application has few other custom implemented QML elements: 
- ImageAnalyzer to analyse the view finder data, for example to calculate the
  histogram data and to detect the amount of movement in the image.

- HistogramDisplay to show the ImageAnalyzer's histogram data.

- PerformanceMeter to show FPS data in the form of a diagram. Currently this is
  commented out in the CameraView.qml.


2. Prerequisites
-------------------------------------------------------------------------------

 - Qt basics
 - Qt Quick basics


3. Project structure and implementation
-------------------------------------------------------------------------------

3.1 Folders
-----------

 |                   The root folder contains the licence information and
 |                   this file (release notes).
 |
 |- bin              Contains the compiled binaries.
 |
 |- doc              Contains documents and graphics projects that were used
 |                   during the development of the application.
 |
 |- src              Contains the project file and Qt / C++ files.
    |
    |- qml           Root folder for QML and JavaScript files.
       |
       |- images     Graphics used in the QML files.
       |
       |- symbian    Symbian-specific QML and JavaScript files.


3.3 Used APIs/QML elements/Qt Quick Components
----------------------------------------------

Qt Mobility Qt camera classes are used to access the camera devices.

QAbstractVideoSurface is used to implement the custom drawing of the view  
finder image.

Qt Quick Components are used to implement sliders and button row in the UI.

4. Compatibility
-------------------------------------------------------------------------------

 - Symbian devices with Qt 4.7.4 or higher.

Tested to work on Nokia 603, Nokia 700, Nokia 701, Nokia 808, Nokia C7-00,
Nokia E7-00 and Nokia N8-00. Developed with Qt SDK 1.2.

4.1 Required capabilities
-------------------------

Application uses UserEnvironment capability to access the camera devices.

Without the support to handle the pressing of HW camera button the application
can be self-signed on Symbian.

To support the HW camera button, uncomment the camerakeylistener.pri line in
the project file. Note that then the application uses SwEvent capability to
override the HW camera button. The application needs to be Symbian Signed
in order to be installable on a mobile device.


4.2 Known issues
----------------

The performance of the view finder image is low. The reason for this is the
transporting of the frames of the view finder image from the system memory to
the graphics memory. If the API would provide the handle of the frame in the
graphics memory, this could be avoided. Currently there is nothing to be done.

The video feature is disabled on devices with 32 MB of graphics memory or less.
The supported devices include Nokia 603, Nokia 700, Nokia 701 and Nokia 808
PureView.


5. Building, installing, and running the application
-------------------------------------------------------------------------------

5.1 Preparations
----------------

Check that you have the latest Qt SDK installed in the development environment
and the latest Qt version on the device.

Qt Quick Components 1.1 or higher is required.

5.2 Using the Qt SDK
--------------------

You can install and run the application on the device using the Qt SDK.
Open the project in the SDK, set up the correct target (depending on the device
platform), and click the Run button. For more details about this approach,
visit the Qt Getting Started section at Nokia Developer
(http://www.developer.nokia.com/Develop/Qt/Getting_started/).

5.3 Symbian device
------------------

Make sure your device is connected to your computer. Locate the .sis
installation file and open it with Ovi Suite. Accept all requests from Nokia
Suite and the device. Note that you can also install the application by copying
the installation file onto your device and opening it with the Symbian File
Manager application.

After the application is installed, locate the application icon from the
application menu and launch the application by tapping the icon.


6. Licence
-------------------------------------------------------------------------------

See the licence text file delivered with this project. The licence file is also
available online at
- http://projects.developer.nokia.com/todolists/browser/Licence.txt


7. Related documentation
-------------------------------------------------------------------------------
Qt Quick Components
- http://doc.qt.nokia.com/qt-components-symbian-1.0/index.html


8. Version history
-------------------------------------------------------------------------------

1.2.1 Mention about hiding video features if gfx memory < 32 MB.
1.2 Added video recording support.
1.1 Redesigned the UI, release camera resources when gallery is opened.
1.0 Initial version.
