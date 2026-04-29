/**
 *
 *  @file MetadataOptions.hpp
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

#ifndef SOFTADASTRA_METADATA_OPTIONS_HPP
#define SOFTADASTRA_METADATA_OPTIONS_HPP

#include <string>
#include <utility>

#include <softadastra/core/Core.hpp>
#include <softadastra/metadata/core/MetadataConfig.hpp>

namespace softadastra::metadata
{
  namespace core_time = softadastra::core::time;
  namespace metadata_core = softadastra::metadata::core;

  /**
   * @brief User-facing metadata options.
   *
   * MetadataOptions is the simple public configuration object used by the
   * high-level metadata API.
   *
   * It maps cleanly to metadata::core::MetadataConfig while keeping a small
   * and approachable surface.
   */
  struct MetadataOptions
  {
    /**
     * @brief Local logical node identifier.
     */
    std::string node_id{};

    /**
     * @brief Human-readable node display name.
     */
    std::string display_name{};

    /**
     * @brief Runtime or product version string.
     */
    std::string version{"0.1.0"};

    /**
     * @brief Enable automatic metadata refresh.
     */
    bool auto_refresh{true};

    /**
     * @brief Runtime refresh interval.
     */
    core_time::Duration refresh_interval{
        core_time::Duration::from_seconds(5)};

    /**
     * @brief Creates default metadata options.
     */
    MetadataOptions() = default;

    /**
     * @brief Creates metadata options from node id and version.
     *
     * @param local_node_id Local node id.
     * @param runtime_version Runtime or product version.
     */
    MetadataOptions(
        std::string local_node_id,
        std::string runtime_version = "0.1.0")
        : node_id(std::move(local_node_id)),
          version(std::move(runtime_version))
    {
      if (display_name.empty())
      {
        display_name = node_id;
      }
    }

    /**
     * @brief Creates local metadata options.
     *
     * @param node_id Local node id.
     * @param version Runtime or product version.
     * @return MetadataOptions.
     */
    [[nodiscard]] static MetadataOptions local(
        std::string node_id,
        std::string version = "0.1.0")
    {
      return MetadataOptions{
          std::move(node_id),
          std::move(version)};
    }

    /**
     * @brief Returns the refresh interval in milliseconds.
     *
     * @return Milliseconds.
     */
    [[nodiscard]] core_time::Duration::rep
    refresh_interval_ms() const noexcept
    {
      return refresh_interval.millis();
    }

    /**
     * @brief Converts public options to core MetadataConfig.
     *
     * @return MetadataConfig.
     */
    [[nodiscard]] metadata_core::MetadataConfig to_config() const
    {
      metadata_core::MetadataConfig config;

      config.node_id = node_id;
      config.display_name = display_name.empty() ? node_id : display_name;
      config.version = version;
      config.auto_refresh = auto_refresh;
      config.refresh_interval = refresh_interval;

      return config;
    }

    /**
     * @brief Returns true if the options are usable.
     *
     * @return true when node id, version, and refresh interval are valid.
     */
    [[nodiscard]] bool is_valid() const noexcept
    {
      return !node_id.empty() &&
             !version.empty() &&
             refresh_interval.millis() > 0;
    }

    /**
     * @brief Backward-compatible valid alias.
     *
     * @return true when options are valid.
     */
    [[nodiscard]] bool valid() const noexcept
    {
      return is_valid();
    }
  };

} // namespace softadastra::metadata

#endif // SOFTADASTRA_METADATA_OPTIONS_HPP
