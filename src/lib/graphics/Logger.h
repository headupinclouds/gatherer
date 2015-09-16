#ifndef __gatherer__Logger__
#define __gatherer__Logger__

#include <spdlog/spdlog.h>

#include "graphics/gatherer_graphics.h"

_GATHERER_GRAPHICS_BEGIN

class Logger
{
public:
    static std::shared_ptr<spdlog::logger> create(const char* name);
    static std::shared_ptr<spdlog::logger> get(const char* name);
};

_GATHERER_GRAPHICS_END

#endif // __gatherer__Logger__
