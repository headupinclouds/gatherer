# gatherer

Disclaimer: This is a work in progress made public to support collaboration during initial development.  This README will be updated when an initial release is available.

Note: The travis builds are currently failing due to build timeouts (mostly due to QT).  There is a WIP to resolved this based on a new binary cache feature in the [Hunter](https://github.com/ruslo/hunter) package manager.

[![Build Status](https://travis-ci.org/headupinclouds/gatherer.svg?branch=develop)](https://travis-ci.org/headupinclouds/gatherer) : develop

[![Build Status](https://travis-ci.org/headupinclouds/gatherer.svg?branch=master)](https://travis-ci.org/headupinclouds/gatherer) : master

The aim of this repository is to provide a lightweight C++ cross platform framework (really a meta-library) for OpenGL (ES) GPGPU shader processing, with GPU friendly video and camera support on most iOS, Android, and "desktop" devices -- essentially a cross platform version of the excellent [GPUImage](https://github.com/BradLarson/GPUImage) project by Brad Larson (and many others), which is written in objective-c for iOS and OS X platforms.  Much of this functionality is already out there in various forms, and can be brought together in this project through the magic of the CMake package manager [Hunter](https://github.com/ruslo/hunter), by Ruslan Baratov, who has done much of the work in this project.  The initial version of Gatherer will likely have very little actual code, with the exception of some interface classes, sample applications, and the core CMakeLists.txt files that glue everything together.

We have adopted the [ogles_gpgpu](https://github.com/internaut/ogles_gpgpu) project, by Markus Konrad, for the initial OpenGL (ES) shader pipeline framework.  It has some nice features, including some nicely abstracted and efficient cpu=>gpu=>cpu texture extensions on iOS (via [Core Video Texture Cache API](http://allmybrain.com/2011/12/08/rendering-to-a-texture-with-ios-5-texture-cache-api/)) and Android (via [KHRImage extensions](http://snorp.net/2011/12/16/android-direct-texture.html)) (GraphicBuffer).  This is currently maintained as a submodule within the project in a `hunter` branch to support initial development and OpenGL (sans ES) support.  These changes will probably get pushed back to the initial project once it is stable, and the submodule will be replaced with a [Hunter](https://github.com/ruslo/hunter) package.  The project also contains fully functional camera + video processing samples for both Android and iOS in the examples section.  

In this project, QT is being used for cross platform UI capabilities, as well as the initial camera input (via the QML Declarative camera interface).  The goal is to keep most of this project portable, and decoupled from QT (and the associated LGPL restrictions) as much as possible, so the camera interface may be replaced at a later point, and the existing GPGPU processing pieces can be used within an existing CMake application easily.  While QT does introduce some significant build overhead, [Hunter](https://github.com/ruslo/hunter) package management should make the build transparent.

While the main objective is simple and portable OpenGL (ES) GPGPU processing, OpenCL seems to be available on the majority of modern Android devices, so some OpenCL package support may also be included through one or more of:

* [OpenCV 3.0](https://github.com/Itseez/opencv) `cv::UMat` GPU extensions
* [Boost Compute](https://github.com/boostorg/compute)
* [ArrayFire](https://github.com/arrayfire/arrayfire)

Current development is performed primarily on OS X, but all official [Hunter](https://github.com/ruslo/hunter) platforms should theoretically be supported: Linux, Mac, Windows, iOS, Android, Raspberry Pi.  This project is still in its infancy, and may not be of much use to end users yet, apart from a collection of examples demonstrating core functionality of the above mentioned packages.  Currently QT QML camera samples are functioning on iOS and Android.  Some simple GPGPU video shader samples are also included. 

A number of these issues are discussed in more detail in the issues section.
