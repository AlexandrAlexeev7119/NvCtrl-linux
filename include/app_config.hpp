#pragma once

namespace GWEpp::config
{
#ifdef NDEBUG
constexpr const char* APP_VERSION_STRING {APP_VERSION "-Release build"};
constexpr bool IS_DEBUG_BUILD {false};
#else
constexpr const char* APP_VERSION_STRING {APP_VERSION "-Debug build"};
constexpr bool IS_DEBUG_BUILD {true};
#endif
} // namespace GWEpp::config
