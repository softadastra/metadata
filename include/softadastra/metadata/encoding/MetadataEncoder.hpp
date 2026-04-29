/**
 *
 *  @file MetadataEncoder.hpp
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

#ifndef SOFTADASTRA_METADATA_ENCODER_HPP
#define SOFTADASTRA_METADATA_ENCODER_HPP

#include <cstdint>
#include <string>
#include <vector>

#include <softadastra/store/utils/Serializer.hpp>
#include <softadastra/metadata/core/NodeMetadata.hpp>
#include <softadastra/metadata/types/CapabilityType.hpp>

namespace softadastra::metadata::encoding
{
  namespace core = softadastra::metadata::core;
  namespace types = softadastra::metadata::types;
  namespace store_utils = softadastra::store::utils;

  /**
   * @brief Encodes node metadata into a deterministic binary payload.
   *
   * Payload format:
   *
   * @code
   * uint32 node_id_size
   * bytes  node_id
   * uint32 display_name_size
   * bytes  display_name
   * uint32 hostname_size
   * bytes  hostname
   * uint32 os_name_size
   * bytes  os_name
   * uint32 version_size
   * bytes  version
   * int64  started_at_millis
   * int64  uptime_millis
   * uint32 capability_count
   * uint8  capability_0
   * uint8  capability_1
   * ...
   * @endcode
   */
  class MetadataEncoder
  {
  public:
    /**
     * @brief Encodes one node metadata payload.
     *
     * Invalid metadata returns an empty buffer.
     *
     * @param metadata Node metadata.
     * @return Encoded metadata bytes.
     */
    [[nodiscard]] static std::vector<std::uint8_t>
    encode(const core::NodeMetadata &metadata)
    {
      if (!metadata.is_valid())
      {
        return {};
      }

      std::vector<std::uint8_t> buffer;

      append_string(buffer, metadata.identity.node_id);
      append_string(buffer, metadata.identity.display_name);
      append_string(buffer, metadata.runtime.hostname);
      append_string(buffer, metadata.runtime.os_name);
      append_string(buffer, metadata.runtime.version);

      store_utils::Serializer::append_i64(
          buffer,
          metadata.runtime.started_at.millis());

      store_utils::Serializer::append_i64(
          buffer,
          metadata.runtime.uptime.millis());

      append_capabilities(buffer, metadata.capabilities.values);

      return buffer;
    }

  private:
    /**
     * @brief Appends a size-prefixed string.
     *
     * @param buffer Output buffer.
     * @param value String value.
     */
    static void append_string(
        std::vector<std::uint8_t> &buffer,
        const std::string &value)
    {
      store_utils::Serializer::append_u32(
          buffer,
          static_cast<std::uint32_t>(value.size()));

      buffer.insert(
          buffer.end(),
          value.begin(),
          value.end());
    }

    /**
     * @brief Appends metadata capabilities.
     *
     * Invalid capabilities are skipped.
     *
     * @param buffer Output buffer.
     * @param values Capability list.
     */
    static void append_capabilities(
        std::vector<std::uint8_t> &buffer,
        const std::vector<types::CapabilityType> &values)
    {
      std::vector<types::CapabilityType> valid_values;

      valid_values.reserve(values.size());

      for (const auto capability : values)
      {
        if (types::is_valid(capability))
        {
          valid_values.push_back(capability);
        }
      }

      store_utils::Serializer::append_u32(
          buffer,
          static_cast<std::uint32_t>(valid_values.size()));

      for (const auto capability : valid_values)
      {
        buffer.push_back(
            static_cast<std::uint8_t>(capability));
      }
    }
  };

} // namespace softadastra::metadata::encoding

#endif // SOFTADASTRA_METADATA_ENCODER_HPP
