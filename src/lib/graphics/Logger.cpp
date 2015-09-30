#include "graphics/Logger.h"

#include <spdlog/spdlog.h>

#if defined(__ANDROID__)
# include <spdlog/sinks/android_sink.h>
#endif

_GATHERER_GRAPHICS_BEGIN

std::shared_ptr<spdlog::logger> Logger::create(const char* name)
{
    std::vector<spdlog::sink_ptr> sinks;
    sinks.push_back(std::make_shared<spdlog::sinks::stdout_sink_mt>());
#if defined(__ANDROID__)
    sinks.push_back(std::make_shared<spdlog::sinks::android_sink_mt>());
#endif
    auto logger = std::make_shared<spdlog::logger>(
        name, begin(sinks), end(sinks)
    );
    spdlog::register_logger(logger);
    spdlog::set_pattern("[%H:%M:%S.%e | thread:%t | %n | %l]: %v");
    return logger;
}

std::shared_ptr<spdlog::logger> Logger::get(const char* name)
{
    return spdlog::get(name);
}

_GATHERER_GRAPHICS_END
