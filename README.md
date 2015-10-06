# gatherer

[![Build Status](https://travis-ci.org/headupinclouds/gatherer.svg?branch=develop)](https://travis-ci.org/headupinclouds/gatherer) : develop


[![Build Status](https://travis-ci.org/headupinclouds/gatherer.svg?branch=master)](https://travis-ci.org/headupinclouds/gatherer) : master

This is an initial sandbox repository to facilitate OpenCV + OpenGL + QT collaboration with the Hunter package manager.

The test video preview app makes uses of GLFW and GLEW to manage the OpenGL contex + window.  
These system libs can be installed via native host package managers.
On OS X this can be done using homebrew:

```
brew tap homebrew/versions
brew install --build-bottle --static glfw3
brew install glew
```

