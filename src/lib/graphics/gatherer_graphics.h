//
//  gatherer_graphics.h
//  GATHERER
//
//  Created by David Hirvonen on 3/2/14.
//
//

#ifndef GATHERER_gatherer_graphics_h
#define GATHERER_gatherer_graphics_h

#define _GATHERER_GRAPHICS_BEGIN namespace gatherer { namespace graphics {
#define _GATHERER_GRAPHICS_END } }

#ifdef _WIN64
//define something for Windows (64-bit)
#elif _WIN32
//define something for Windows (32-bit)
#elif __APPLE__
#include "TargetConditionals.h"
#if TARGET_OS_IPHONE && TARGET_IPHONE_SIMULATOR
#  define GATHERER_IOS 1
// define something for simulator
#elif TARGET_OS_IPHONE
#  define GATHERER_IOS 1
#  include <OpenGLES/ES2/gl.h>
#  include <OpenGLES/ES2/glext.h>
#  include <arm_neon.h>
#  define DO_GRAPHICS_SIMD 1
#else
#  include <OpenGL/gl.h>
#  include <OpenGL/glext.h>
#  define DO_GRAPHICS_SIMD 0
#endif
#elif __ANDROID__ || ANDROID
//#include <GLES3/gl3.h>
//#include <GLES3/gl3ext.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#elif __linux
// linux
#elif __unix // all unices not caught above
// Unix
#elif __posix
// POSIX
#endif

#if !defined(GATHERER_IOS)
# define GATHERER_IOS 0
#endif

// ANDROID
//#include <GLES2/gl2.h>
//#include <GLES2/gl2ext.h>

//#include <GLES/gl.h>
//#include <GLES/glext.h>

#include <memory>

// http://clean-cpp.org/underprivileged-unique-pointers-make_unique/#more-54
template <typename Value, typename ... Arguments>
std::unique_ptr<Value> make_unique(Arguments && ... arguments_for_constructor)
{
    return std::unique_ptr<Value>(new Value(std::forward<Arguments>(arguments_for_constructor)...));
}



#endif
