/*
 * MetadataField.hpp
 */

#ifndef SOFTADASTRA_METADATA_FIELD_HPP
#define SOFTADASTRA_METADATA_FIELD_HPP

#include <cstdint>

namespace softadastra::metadata::types
{
  /**
   * @brief Logical metadata field identifier
   *
   * Can be used for filtering, encoding, selective export,
   * or field-level inspection.
   */
  enum class MetadataField : std::uint8_t
  {
    Unknown = 0,

    NodeId,
    DisplayName,
    Hostname,
    OsName,
    Version,
    StartedAt,
    UptimeMs,
    Capabilities
  };

} // namespace softadastra::metadata::types

#endif
