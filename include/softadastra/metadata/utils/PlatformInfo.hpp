/**
 *
 *  @file PlatformInfo.hpp
 *  @author Gaspard Kirira
 *
 *  Copyright 2026, Softadastra.
 *  All rights reserved.
 *  https://github.com/softadastra/softadastra
 *
 *  Licensed under the Apache License, Version 2.0.
 *
 *  Softadastra Metadata
 *
 */

#ifndef SOFTADASTRA_METADATA_PLATFORM_INFO_HPP
#define SOFTADASTRA_METADATA_PLATFORM_INFO_HPP

#include <string>
#include <string_view>

namespace softadastra::metadata::utils
{
  /**
   * @brief Utility for exposing basic platform information.
   *
   * PlatformInfo provides compile-time detection helpers for the current
   * operating system, compiler, architecture, and build mode.
   */
  class PlatformInfo
  {
  public:
    /**
     * @brief Returns the current operating system name.
     *
     * @return Stable OS name.
     */
    [[nodiscard]] static constexpr std::string_view os_name_view() noexcept
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
     * @brief Returns the current operating system name.
     *
     * @return Stable OS name.
     */
    [[nodiscard]] static std::string os_name()
    {
      return std::string{os_name_view()};
    }

    /**
     * @brief Returns the current compiler name.
     *
     * @return Stable compiler name.
     */
    [[nodiscard]] static constexpr std::string_view compiler_name_view() noexcept
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

    /**
     * @brief Returns the current compiler name.
     *
     * @return Stable compiler name.
     */
    [[nodiscard]] static std::string compiler_name()
    {
      return std::string{compiler_name_view()};
    }

    /**
     * @brief Returns the current CPU architecture.
     *
     * @return Stable architecture name.
     */
    [[nodiscard]] static constexpr std::string_view architecture_view() noexcept
    {
#if defined(__x86_64__) || defined(_M_X64)
      return "x86_64";
#elif defined(__aarch64__) || defined(_M_ARM64)
      return "arm64";
#elif defined(__i386__) || defined(_M_IX86)
      return "x86";
#elif defined(__arm__) || defined(_M_ARM)
      return "arm";
#else
      return "unknown-arch";
#endif
    }

    /**
     * @brief Returns the current CPU architecture.
     *
     * @return Stable architecture name.
     */
    [[nodiscard]] static std::string architecture()
    {
      return std::string{architecture_view()};
    }

    /**
     * @brief Returns the current build mode.
     *
     * @return debug when NDEBUG is not defined, otherwise release.
     */
    [[nodiscard]] static constexpr std::string_view build_mode_view() noexcept
    {
#ifdef NDEBUG
      return "release";
#else
      return "debug";
#endif
    }

    /**
     * @brief Returns the current build mode.
     *
     * @return Build mode string.
     */
    [[nodiscard]] static std::string build_mode()
    {
      return std::string{build_mode_view()};
    }

    /**
     * @brief Returns a compact platform summary.
     *
     * @return OS/compiler/architecture/build-mode summary.
     */
    [[nodiscard]] static std::string summary()
    {
      return os_name() + "/" +
             compiler_name() + "/" +
             architecture() + "/" +
             build_mode();
    }
  };

} // namespace softadastra::metadata::utils

#endif // SOFTADASTRA_METADATA_PLATFORM_INFO_HPP
