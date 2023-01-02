#pragma once

#include <string_view>
#include <chrono>

class MeasureTime final
{
public:
    MeasureTime(const MeasureTime&) = delete;
    MeasureTime& operator=(const MeasureTime&) = delete;
    MeasureTime(MeasureTime&&) = delete;
    MeasureTime& operator=(MeasureTime&&) = delete;

    MeasureTime(std::string_view start_message = "", std::string_view end_message = "Time elapsed: {} ms") noexcept;
    ~MeasureTime();

private:
    const std::chrono::time_point<std::chrono::high_resolution_clock> time_start_;
    const std::string_view end_message_;
};
