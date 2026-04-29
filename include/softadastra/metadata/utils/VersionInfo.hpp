/**
 *
 *  @file VersionInfo.hpp
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

#ifndef SOFTADASTRA_METADATA_VERSION_INFO_HPP
#define SOFTADASTRA_METADATA_VERSION_INFO_HPP

#include <string>
#include <string_view>

namespace softadastra::metadata::utils
{
  /**
   * @brief Utility for exposing Softadastra metadata version information.
   *
   * VersionInfo centralizes the default version used by the metadata module
   * when no explicit runtime or product version is provided.
   */
  class VersionInfo
  {
  public:
    /**
     * @brief Default metadata module version.
     */
    static constexpr std::string_view default_version{"0.1.0"};

    /**
     * @brief Returns the current module or runtime version view.
     *
     * @return Version string view.
     */
    [[nodiscard]] static constexpr std::string_view current_view() noexcept
    {
      return default_version;
    }

    /**
     * @brief Returns the current module or runtime version string.
     *
     * @return Version string.
     */
    [[nodiscard]] static std::string current()
    {
      return std::string{current_view()};
    }

    /**
     * @brief Returns true if a version string is usable.
     *
     * @param version Version string.
     * @return true when non-empty.
     */
    [[nodiscard]] static bool is_valid(
        const std::string &version) noexcept
    {
      return !version.empty();
    }
  };

} // namespace softadastra::metadata::utils

#endif // SOFTADASTRA_METADATA_VERSION_INFO_HPP
