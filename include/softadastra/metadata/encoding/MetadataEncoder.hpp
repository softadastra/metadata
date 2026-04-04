/*
 * MetadataEncoder.hpp
 */

#ifndef SOFTADASTRA_METADATA_ENCODER_HPP
#define SOFTADASTRA_METADATA_ENCODER_HPP

#include <cstdint>
#include <cstring>
#include <vector>

#include <softadastra/metadata/core/NodeMetadata.hpp>
#include <softadastra/metadata/types/CapabilityType.hpp>

namespace softadastra::metadata::encoding
{
  namespace core = softadastra::metadata::core;
  namespace types = softadastra::metadata::types;

  class MetadataEncoder
  {
  public:
    /**
     * @brief Encode one node metadata payload
     *
     * Payload format:
     *   [uint32_t node_id_size][node_id bytes]
     *   [uint32_t display_name_size][display_name bytes]
     *   [uint32_t hostname_size][hostname bytes]
     *   [uint32_t os_name_size][os_name bytes]
     *   [uint32_t version_size][version bytes]
     *   [uint64_t started_at]
     *   [uint64_t uptime_ms]
     *   [uint32_t capability_count]
     *   [uint8_t capability_0]
     *   [uint8_t capability_1]
     *   ...
     */
    static std::vector<std::uint8_t> encode(const core::NodeMetadata &metadata)
    {
      const std::uint32_t node_id_size =
          static_cast<std::uint32_t>(metadata.identity.node_id.size());

      const std::uint32_t display_name_size =
          static_cast<std::uint32_t>(metadata.identity.display_name.size());

      const std::uint32_t hostname_size =
          static_cast<std::uint32_t>(metadata.runtime.hostname.size());

      const std::uint32_t os_name_size =
          static_cast<std::uint32_t>(metadata.runtime.os_name.size());

      const std::uint32_t version_size =
          static_cast<std::uint32_t>(metadata.runtime.version.size());

      const std::uint32_t capability_count =
          static_cast<std::uint32_t>(metadata.capabilities.values.size());

      const std::size_t total_size =
          sizeof(std::uint32_t) + node_id_size +
          sizeof(std::uint32_t) + display_name_size +
          sizeof(std::uint32_t) + hostname_size +
          sizeof(std::uint32_t) + os_name_size +
          sizeof(std::uint32_t) + version_size +
          sizeof(std::uint64_t) +
          sizeof(std::uint64_t) +
          sizeof(std::uint32_t) +
          capability_count * sizeof(std::uint8_t);

      std::vector<std::uint8_t> buffer(total_size);
      std::size_t offset = 0;

      write_string(buffer, offset, metadata.identity.node_id);
      write_string(buffer, offset, metadata.identity.display_name);
      write_string(buffer, offset, metadata.runtime.hostname);
      write_string(buffer, offset, metadata.runtime.os_name);
      write_string(buffer, offset, metadata.runtime.version);
      write(buffer, offset, metadata.runtime.started_at);
      write(buffer, offset, metadata.runtime.uptime_ms);
      write_capabilities(buffer, offset, metadata.capabilities.values);

      return buffer;
    }

  private:
    template <typename T>
    static void write(std::vector<std::uint8_t> &buffer,
                      std::size_t &offset,
                      T value)
    {
      std::memcpy(buffer.data() + offset, &value, sizeof(T));
      offset += sizeof(T);
    }

    static void write_string(std::vector<std::uint8_t> &buffer,
                             std::size_t &offset,
                             const std::string &value)
    {
      const std::uint32_t size = static_cast<std::uint32_t>(value.size());
      write(buffer, offset, size);

      if (size > 0)
      {
        std::memcpy(buffer.data() + offset, value.data(), size);
        offset += size;
      }
    }

    static void write_capabilities(std::vector<std::uint8_t> &buffer,
                                   std::size_t &offset,
                                   const std::vector<types::CapabilityType> &values)
    {
      const std::uint32_t count = static_cast<std::uint32_t>(values.size());
      write(buffer, offset, count);

      for (const auto capability : values)
      {
        write(buffer, offset, static_cast<std::uint8_t>(capability));
      }
    }
  };

} // namespace softadastra::metadata::encoding

#endif
