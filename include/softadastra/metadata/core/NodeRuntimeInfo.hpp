/**
 *
 *  @file NodeRuntimeInfo.hpp
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

#ifndef SOFTADASTRA_METADATA_NODE_RUNTIME_INFO_HPP
#define SOFTADASTRA_METADATA_NODE_RUNTIME_INFO_HPP

#include <string>
#include <utility>

#include <softadastra/core/Core.hpp>

namespace softadastra::metadata::core
{
  namespace core_time = softadastra::core::time;

  /**
   * @brief Runtime information describing a node.
   *
   * NodeRuntimeInfo contains runtime-level information such as hostname,
   * operating system, version, start time, and uptime.
   */
  struct NodeRuntimeInfo
  {
    /**
     * @brief Local hostname.
     */
    std::string hostname{};

    /**
     * @brief Operating system name.
     */
    std::string os_name{};

    /**
     * @brief Runtime or product version string.
     */
    std::string version{};

    /**
     * @brief Process or node start timestamp.
     */
    core_time::Timestamp started_at{};

    /**
     * @brief Current uptime.
     */
    core_time::Duration uptime{};

    /**
     * @brief Creates an empty invalid runtime info object.
     */
    NodeRuntimeInfo() = default;

    /**
     * @brief Creates runtime info.
     *
     * @param node_hostname Hostname.
     * @param node_os_name Operating system name.
     * @param runtime_version Runtime or product version.
     */
    NodeRuntimeInfo(
        std::string node_hostname,
        std::string node_os_name,
        std::string runtime_version)
        : hostname(std::move(node_hostname)),
          os_name(std::move(node_os_name)),
          version(std::move(runtime_version)),
          started_at(core_time::Timestamp::now()),
          uptime(core_time::Duration{})
    {
    }

    /**
     * @brief Returns uptime in milliseconds.
     *
     * @return Uptime in milliseconds.
     */
    [[nodiscard]] core_time::Duration::rep
    uptime_ms() const noexcept
    {
      return uptime.millis();
    }

    /**
     * @brief Refreshes uptime from the current time.
     */
    void refresh_uptime() noexcept
    {
      if (!started_at.is_valid())
      {
        uptime = core_time::Duration{};
        return;
      }

      const auto now = core_time::Timestamp::now();

      if (!now.is_valid() || now.millis() < started_at.millis())
      {
        uptime = core_time::Duration{};
        return;
      }

      uptime =
          core_time::Duration::from_millis(
              now.millis() - started_at.millis());
    }

    /**
     * @brief Resets the start timestamp and uptime.
     */
    void reset_start_time() noexcept
    {
      started_at = core_time::Timestamp::now();
      uptime = core_time::Duration{};
    }

    /**
     * @brief Returns true if this runtime info is usable.
     *
     * @return true when hostname, OS name, version, and start time are valid.
     */
    [[nodiscard]] bool is_valid() const noexcept
    {
      return !hostname.empty() &&
             !os_name.empty() &&
             !version.empty() &&
             started_at.is_valid();
    }

    /**
     * @brief Backward-compatible valid alias.
     *
     * @return true when runtime info is valid.
     */
    [[nodiscard]] bool valid() const noexcept
    {
      return is_valid();
    }

    /**
     * @brief Clears the runtime info.
     */
    void clear() noexcept
    {
      hostname.clear();
      os_name.clear();
      version.clear();
      started_at = core_time::Timestamp{};
      uptime = core_time::Duration{};
    }
  };

} // namespace softadastra::metadata::core

#endif // SOFTADASTRA_METADATA_NODE_RUNTIME_INFO_HPP
