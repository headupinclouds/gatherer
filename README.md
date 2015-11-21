# gatherer

Disclaimer: This is a work in progress made public to support collaboration during initial development.  This README will be updated when an initial release is available.

Note: The travis builds are currently failing due to build timeouts (mostly due to QT), although the develop branch should typically build fine.  This will likely need to be resolved by an increase in the travis timeout or a future binary package feature in the [Hunter](https://github.com/ruslo/hunter) package manager, on which this project is based.

[![Build Status](https://travis-ci.org/headupinclouds/gatherer.svg?branch=develop)](https://travis-ci.org/headupinclouds/gatherer) : develop

[![Build Status](https://travis-ci.org/headupinclouds/gatherer.svg?branch=master)](https://travis-ci.org/headupinclouds/gatherer) : master

The aim of this repository is to provide a lightweight C++ cross platform framework (really a meta-library) for OpenGL (ES) GPGPU shader processing, with GPU friendly video and camera support on most iOS, Android, and "desktop" devices -- essentially a cross platform version of the excellent [GPUImage](https://github.com/BradLarson/GPUImage) project by Brad Larson (and many others), which is written in objective-c for iOS and OS X platforms.  Much of this functionality is already out there in various forms, and can be brought together in this project through the magic of the CMake package manager [Hunter](https://github.com/ruslo/hunter), by Ruslan Baratov, who has done much of the work in this project.  
 
We have adopted the [ogles_gpgpu](https://github.com/internaut/ogles_gpgpu) project, by Markus Konrad, for the initial OpenGL (ES) shader pipeline framework.  It has some nice features, including some nicely abstracted and efficient cpu=>gpu=>cpu texture extensions on iOS (via [Core Video Texture Cache API](http://allmybrain.com/2011/12/08/rendering-to-a-texture-with-ios-5-texture-cache-api/)) and Android (via [KHRImage extensions](http://snorp.net/2011/12/16/android-direct-texture.html)).  This is currently maintained as a submodule within the project in a `hunter` branch to support initial development and OpenGL (sans ES) support.  These changes will probably get pushed back to the initial project once it is stable, and the submodule will be replaced with a [Hunter](https://github.com/ruslo/hunter) package.  The project also contains fully functional camera + video processing samples for both Android and iOS in the examples section.  

In this project, QT is being used for cross platform UI capabilities, as well as the initial camera input.  

The goal is to keep most of this project portable, and decopuled from QT (and the associated LGPL restriction) as much as possible, so the camera interface may be replaced at a later point.  

Background: The [OpenCV](https://github.com/Itseez/opencv) `highgui` lib provides a nice `VideoCapture` class.  This comes very close to providing a cross platform camera interface, but the native Android camera interface (which relies on a somewhat undocumented and unsupported API) has been deprecated, and the camera does not run on any recent Android devices we tested, despite a fair amount of testing and tinkering.  With the exception of Android, OpenCV's `VideoCapture` would seem to cover most iOS and "desktop" systems.  Android is the only wrinkle, due to Java, but it seems direct Android camera access is at least possible through (Necessitas)[https://necessitas.kde.org] extensions, which may be another option.

To be continued...

