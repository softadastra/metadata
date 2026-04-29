/**
 *
 *  @file MetadataField.hpp
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

#ifndef SOFTADASTRA_METADATA_FIELD_HPP
#define SOFTADASTRA_METADATA_FIELD_HPP

#include <cstdint>
#include <string_view>

namespace softadastra::metadata::types
{
  /**
   * @brief Logical metadata field identifier.
   *
   * MetadataField identifies a field inside NodeMetadata.
   *
   * It can be used for:
   * - filtering
   * - encoding
   * - selective export
   * - diagnostics
   * - field-level inspection
   *
   * Rules:
   * - Values must remain stable over time.
   * - Do not reorder existing values.
   * - Do not remove existing values once released.
   * - Add new values only at the end.
   */
  enum class MetadataField : std::uint8_t
  {
    /**
     * @brief Unknown or invalid field.
     */
    Unknown = 0,

    /**
     * @brief Node identifier field.
     */
    NodeId,

    /**
     * @brief Human-readable display name field.
     */
    DisplayName,

    /**
     * @brief Hostname field.
     */
    Hostname,

    /**
     * @brief Operating system name field.
     */
    OsName,

    /**
     * @brief Software version field.
     */
    Version,

    /**
     * @brief Node start timestamp field.
     */
    StartedAt,

    /**
     * @brief Node uptime field.
     */
    UptimeMs,

    /**
     * @brief Node capabilities field.
     */
    Capabilities
  };

  /**
   * @brief Returns a stable string representation of a metadata field.
   *
   * @param field Metadata field.
   * @return Stable string representation.
   */
  [[nodiscard]] constexpr std::string_view
  to_string(MetadataField field) noexcept
  {
    switch (field)
    {
    case MetadataField::Unknown:
      return "unknown";

    case MetadataField::NodeId:
      return "node_id";

    case MetadataField::DisplayName:
      return "display_name";

    case MetadataField::Hostname:
      return "hostname";

    case MetadataField::OsName:
      return "os_name";

    case MetadataField::Version:
      return "version";

    case MetadataField::StartedAt:
      return "started_at";

    case MetadataField::UptimeMs:
      return "uptime_ms";

    case MetadataField::Capabilities:
      return "capabilities";

    default:
      return "invalid";
    }
  }

  /**
   * @brief Returns true if the field is known and usable.
   *
   * Unknown is intentionally treated as invalid.
   *
   * @param field Metadata field.
   * @return true for usable metadata fields.
   */
  [[nodiscard]] constexpr bool is_valid(MetadataField field) noexcept
  {
    return field == MetadataField::NodeId ||
           field == MetadataField::DisplayName ||
           field == MetadataField::Hostname ||
           field == MetadataField::OsName ||
           field == MetadataField::Version ||
           field == MetadataField::StartedAt ||
           field == MetadataField::UptimeMs ||
           field == MetadataField::Capabilities;
  }

  /**
   * @brief Returns true if the field identifies the node.
   *
   * @param field Metadata field.
   * @return true for identity-related fields.
   */
  [[nodiscard]] constexpr bool is_identity_field(MetadataField field) noexcept
  {
    return field == MetadataField::NodeId ||
           field == MetadataField::DisplayName ||
           field == MetadataField::Hostname;
  }

  /**
   * @brief Returns true if the field describes runtime state.
   *
   * @param field Metadata field.
   * @return true for runtime-related fields.
   */
  [[nodiscard]] constexpr bool is_runtime_field(MetadataField field) noexcept
  {
    return field == MetadataField::StartedAt ||
           field == MetadataField::UptimeMs;
  }

} // namespace softadastra::metadata::types

#endif // SOFTADASTRA_METADATA_FIELD_HPP
