/**
 *
 *  @file MetadataStatus.hpp
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

#ifndef SOFTADASTRA_METADATA_STATUS_HPP
#define SOFTADASTRA_METADATA_STATUS_HPP

#include <cstdint>
#include <string_view>

namespace softadastra::metadata::types
{

  /**
   * @brief Runtime status of the metadata engine.
   *
   * MetadataStatus describes the lifecycle of the metadata engine.
   *
   * It is used by:
   * - MetadataEngine
   * - MetadataService
   * - metadata providers
   * - diagnostics and metrics
   *
   * Rules:
   * - Values must remain stable over time.
   * - Do not reorder existing values.
   * - Do not remove existing values once released.
   * - Add new values only at the end.
   */
  enum class MetadataStatus : std::uint8_t
  {
    /**
     * @brief Metadata engine is stopped.
     */
    Stopped = 0,

    /**
     * @brief Metadata engine is starting.
     */
    Starting,

    /**
     * @brief Metadata engine is running.
     */
    Running,

    /**
     * @brief Metadata engine is stopping.
     */
    Stopping,

    /**
     * @brief Metadata engine failed and needs inspection.
     */
    Failed
  };

  /**
   * @brief Returns a stable string representation of a metadata status.
   *
   * @param status Metadata status.
   * @return Stable string representation.
   */
  [[nodiscard]] constexpr std::string_view
  to_string(MetadataStatus status) noexcept
  {
    switch (status)
    {
    case MetadataStatus::Stopped:
      return "stopped";

    case MetadataStatus::Starting:
      return "starting";

    case MetadataStatus::Running:
      return "running";

    case MetadataStatus::Stopping:
      return "stopping";

    case MetadataStatus::Failed:
      return "failed";

    default:
      return "invalid";
    }
  }

  /**
   * @brief Returns true if the metadata status is known.
   *
   * @param status Metadata status.
   * @return true for all defined statuses.
   */
  [[nodiscard]] constexpr bool is_valid(MetadataStatus status) noexcept
  {
    return status == MetadataStatus::Stopped ||
           status == MetadataStatus::Starting ||
           status == MetadataStatus::Running ||
           status == MetadataStatus::Stopping ||
           status == MetadataStatus::Failed;
  }

  /**
   * @brief Returns true if metadata can be collected and served.
   *
   * @param status Metadata status.
   * @return true when running.
   */
  [[nodiscard]] constexpr bool is_running(MetadataStatus status) noexcept
  {
    return status == MetadataStatus::Running;
  }

  /**
   * @brief Returns true if metadata is transitioning.
   *
   * @param status Metadata status.
   * @return true for Starting and Stopping.
   */
  [[nodiscard]] constexpr bool is_transitioning(MetadataStatus status) noexcept
  {
    return status == MetadataStatus::Starting ||
           status == MetadataStatus::Stopping;
  }

  /**
   * @brief Returns true if metadata is stopped or failed.
   *
   * @param status Metadata status.
   * @return true for Stopped and Failed.
   */
  [[nodiscard]] constexpr bool is_terminal(MetadataStatus status) noexcept
  {
    return status == MetadataStatus::Stopped ||
           status == MetadataStatus::Failed;
  }

} // namespace softadastra::metadata::types

#endif // SOFTADASTRA_METADATA_STATUS_HPP
