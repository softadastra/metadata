/**
 *
 *  @file MetadataConfig.hpp
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

#ifndef SOFTADASTRA_METADATA_CONFIG_HPP
#define SOFTADASTRA_METADATA_CONFIG_HPP

#include <string>
#include <utility>

#include <softadastra/core/Core.hpp>

namespace softadastra::metadata::core
{
  namespace core_time = softadastra::core::time;

  /**
   * @brief Runtime configuration for the metadata layer.
   *
   * MetadataConfig controls how node metadata is created and refreshed.
   *
   * It contains:
   * - local node id
   * - human-readable display name
   * - product or runtime version
   * - automatic refresh flag
   * - refresh interval
   */
  struct MetadataConfig
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
     * @brief Product or runtime version string.
     */
    std::string version{"0.1.0"};

    /**
     * @brief Enable automatic runtime refresh.
     */
    bool auto_refresh{true};

    /**
     * @brief Runtime refresh interval.
     */
    core_time::Duration refresh_interval{
        core_time::Duration::from_seconds(5)};

    /**
     * @brief Creates a default invalid metadata config.
     */
    MetadataConfig() = default;

    /**
     * @brief Creates metadata config from node id and version.
     *
     * @param local_node_id Local node id.
     * @param runtime_version Product or runtime version.
     */
    MetadataConfig(
        std::string local_node_id,
        std::string runtime_version = "0.1.0")
        : node_id(std::move(local_node_id)),
          version(std::move(runtime_version))
    {
    }

    /**
     * @brief Creates local metadata config.
     *
     * @param node_id Local node id.
     * @param version Runtime or product version.
     * @return MetadataConfig.
     */
    [[nodiscard]] static MetadataConfig local(
        std::string node_id,
        std::string version = "0.1.0")
    {
      MetadataConfig config{
          std::move(node_id),
          std::move(version)};

      config.display_name = config.node_id;

      return config;
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
     * @brief Returns true if the configuration is usable.
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
     * @return true when config is valid.
     */
    [[nodiscard]] bool valid() const noexcept
    {
      return is_valid();
    }
  };

} // namespace softadastra::metadata::core

#endif // SOFTADASTRA_METADATA_CONFIG_HPP
