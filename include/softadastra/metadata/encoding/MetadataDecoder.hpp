/**
 *
 *  @file MetadataDecoder.hpp
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

#ifndef SOFTADASTRA_METADATA_DECODER_HPP
#define SOFTADASTRA_METADATA_DECODER_HPP

#include <cstddef>
#include <cstdint>
#include <optional>
#include <span>
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
  namespace core_time = softadastra::core::time;

  /**
   * @brief Decodes binary metadata payloads into NodeMetadata.
   *
   * MetadataDecoder is the inverse of MetadataEncoder.
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
  class MetadataDecoder
  {
  public:
    /**
     * @brief Decodes one node metadata payload.
     *
     * @param data Encoded metadata bytes.
     * @return NodeMetadata or std::nullopt on invalid input.
     */
    [[nodiscard]] static std::optional<core::NodeMetadata>
    decode(std::span<const std::uint8_t> data)
    {
      if (data.size() < minimum_size())
      {
        return std::nullopt;
      }

      std::size_t offset = 0;
      core::NodeMetadata metadata{};

      if (!read_string(data, offset, metadata.identity.node_id))
      {
        return std::nullopt;
      }

      if (!read_string(data, offset, metadata.identity.display_name))
      {
        return std::nullopt;
      }

      if (!read_string(data, offset, metadata.runtime.hostname))
      {
        return std::nullopt;
      }

      if (!read_string(data, offset, metadata.runtime.os_name))
      {
        return std::nullopt;
      }

      if (!read_string(data, offset, metadata.runtime.version))
      {
        return std::nullopt;
      }

      std::int64_t started_at_millis = 0;
      std::int64_t uptime_millis = 0;

      if (!store_utils::Serializer::read_i64(
              data,
              offset,
              started_at_millis))
      {
        return std::nullopt;
      }

      if (!store_utils::Serializer::read_i64(
              data,
              offset,
              uptime_millis))
      {
        return std::nullopt;
      }

      metadata.runtime.started_at =
          core_time::Timestamp::from_millis(started_at_millis);

      metadata.runtime.uptime =
          core_time::Duration::from_millis(uptime_millis);

      if (!read_capabilities(
              data,
              offset,
              metadata.capabilities.values))
      {
        return std::nullopt;
      }

      if (offset != data.size())
      {
        return std::nullopt;
      }

      if (!metadata.is_valid())
      {
        return std::nullopt;
      }

      return metadata;
    }

    /**
     * @brief Decodes one node metadata payload from raw pointer and size.
     *
     * @param data Encoded metadata bytes.
     * @param size Byte count.
     * @return NodeMetadata or std::nullopt on invalid input.
     */
    [[nodiscard]] static std::optional<core::NodeMetadata>
    decode(const std::uint8_t *data, std::size_t size)
    {
      if (data == nullptr)
      {
        return std::nullopt;
      }

      return decode(
          std::span<const std::uint8_t>(data, size));
    }

    /**
     * @brief Decodes one node metadata payload from a raw byte vector.
     *
     * @param buffer Encoded metadata bytes.
     * @return NodeMetadata or std::nullopt on invalid input.
     */
    [[nodiscard]] static std::optional<core::NodeMetadata>
    decode(const std::vector<std::uint8_t> &buffer)
    {
      if (buffer.empty())
      {
        return std::nullopt;
      }

      return decode(
          std::span<const std::uint8_t>(buffer.data(), buffer.size()));
    }

  private:
    /**
     * @brief Minimum possible metadata payload size.
     */
    [[nodiscard]] static constexpr std::size_t minimum_size() noexcept
    {
      return sizeof(std::uint32_t) +
             sizeof(std::uint32_t) +
             sizeof(std::uint32_t) +
             sizeof(std::uint32_t) +
             sizeof(std::uint32_t) +
             sizeof(std::int64_t) +
             sizeof(std::int64_t) +
             sizeof(std::uint32_t);
    }

    /**
     * @brief Reads a size-prefixed string.
     */
    [[nodiscard]] static bool read_string(
        std::span<const std::uint8_t> data,
        std::size_t &offset,
        std::string &out)
    {
      std::uint32_t length = 0;

      if (!store_utils::Serializer::read_u32(
              data,
              offset,
              length))
      {
        return false;
      }

      if (!store_utils::Serializer::can_read(
              data,
              offset,
              length))
      {
        return false;
      }

      out.assign(
          reinterpret_cast<const char *>(data.data() + offset),
          length);

      offset += length;
      return true;
    }

    /**
     * @brief Reads capability list.
     */
    [[nodiscard]] static bool read_capabilities(
        std::span<const std::uint8_t> data,
        std::size_t &offset,
        std::vector<types::CapabilityType> &out)
    {
      std::uint32_t count = 0;

      if (!store_utils::Serializer::read_u32(
              data,
              offset,
              count))
      {
        return false;
      }

      if (!store_utils::Serializer::can_read(data, offset, count))
      {
        return false;
      }

      out.clear();
      out.reserve(count);

      for (std::uint32_t i = 0; i < count; ++i)
      {
        const auto capability =
            static_cast<types::CapabilityType>(data[offset]);

        ++offset;

        if (!types::is_valid(capability))
        {
          return false;
        }

        out.push_back(capability);
      }

      return true;
    }
  };

} // namespace softadastra::metadata::encoding

#endif // SOFTADASTRA_METADATA_DECODER_HPP
