/*
 * PlatformInfo.hpp
 */

#ifndef SOFTADASTRA_METADATA_PLATFORM_INFO_HPP
#define SOFTADASTRA_METADATA_PLATFORM_INFO_HPP

#include <string>

namespace softadastra::metadata::utils
{
  class PlatformInfo
  {
  public:
    /**
     * @brief Return the current operating system name
     */
    static std::string os_name()
    {
#if defined(_WIN32)
      return "windows";
#elif defined(__APPLE__)
      return "macos";
#elif defined(__linux__)
      return "linux";
#elif defined(__unix__)
      return "unix";
#else
      return "unknown-os";
#endif
    }

    /**
     * @brief Return the current compiler name
     */
    static std::string compiler_name()
    {
#if defined(__clang__)
      return "clang";
#elif defined(__GNUC__)
      return "gcc";
#elif defined(_MSC_VER)
      return "msvc";
#else
      return "unknown-compiler";
#endif
    }
  };

} // namespace softadastra::metadata::utils

#endif
