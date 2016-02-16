#ifndef __gatherer__Logger__
#define __gatherer__Logger__

#include <spdlog/spdlog.h>

#include "graphics/gatherer_graphics.h"

_GATHERER_GRAPHICS_BEGIN

class Logger
{
public:
    using Pointer = std::shared_ptr<spdlog::logger>;
    static Pointer create(const char* name);
    static Pointer get(const char* name);
    static void drop(const char* name);
};

_GATHERER_GRAPHICS_END

#endif // __gatherer__Logger__
