#include <log.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#define SPDLOG_WCHAR_TO_UTF8_SUPPORT

std::shared_ptr<spdlog::logger> log::core_logger;

void log::init() {
  spdlog::set_pattern("%^[%T] %n: %v%$");
  core_logger = spdlog::stdout_color_mt("KACZKA");
  core_logger->set_level(spdlog::level::trace);
  GK2_KACZKA_INFO("Initialized log!");
}
