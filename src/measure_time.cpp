#include "spdlog/spdlog.h"

#include "measure_time.hpp"

MeasureTime::MeasureTime(std::string_view start_message, std::string_view end_message) noexcept
    : time_start_ {std::chrono::high_resolution_clock::now()}
    , end_message_ {end_message}
{
    spdlog::info(start_message);
}

MeasureTime::~MeasureTime()
{
    const std::chrono::duration<double, std::milli> time_end {std::chrono::high_resolution_clock::now() - time_start_};
    spdlog::info(end_message_, time_end.count());
}
