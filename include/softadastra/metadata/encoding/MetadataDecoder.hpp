/*
 * MetadataDecoder.hpp
 */

#ifndef SOFTADASTRA_METADATA_DECODER_HPP
#define SOFTADASTRA_METADATA_DECODER_HPP

#include <cstdint>
#include <cstring>
#include <optional>
#include <string>
#include <vector>

#include <softadastra/metadata/core/NodeMetadata.hpp>
#include <softadastra/metadata/types/CapabilityType.hpp>

namespace softadastra::metadata::encoding
{
  namespace core = softadastra::metadata::core;
  namespace types = softadastra::metadata::types;

  class MetadataDecoder
  {
  public:
    /**
     * @brief Decode one node metadata payload
     */
    static std::optional<core::NodeMetadata> decode(const std::uint8_t *data,
                                                    std::size_t size)
    {
      if (data == nullptr || size < minimum_size())
      {
        return std::nullopt;
      }

      std::size_t offset = 0;
      core::NodeMetadata metadata;

      if (!read_string(data, size, offset, metadata.identity.node_id))
      {
        return std::nullopt;
      }

      if (!read_string(data, size, offset, metadata.identity.display_name))
      {
        return std::nullopt;
      }

      if (!read_string(data, size, offset, metadata.runtime.hostname))
      {
        return std::nullopt;
      }

      if (!read_string(data, size, offset, metadata.runtime.os_name))
      {
        return std::nullopt;
      }

      if (!read_string(data, size, offset, metadata.runtime.version))
      {
        return std::nullopt;
      }

      if (!read(data, size, offset, metadata.runtime.started_at))
      {
        return std::nullopt;
      }

      if (!read(data, size, offset, metadata.runtime.uptime_ms))
      {
        return std::nullopt;
      }

      if (!read_capabilities(data, size, offset, metadata.capabilities.values))
      {
        return std::nullopt;
      }

      if (offset != size)
      {
        return std::nullopt;
      }

      if (!metadata.valid())
      {
        return std::nullopt;
      }

      return metadata;
    }

    /**
     * @brief Decode one node metadata payload from a raw byte vector
     */
    static std::optional<core::NodeMetadata> decode(
        const std::vector<std::uint8_t> &buffer)
    {
      if (buffer.empty())
      {
        return std::nullopt;
      }

      return decode(buffer.data(), buffer.size());
    }

  private:
    static constexpr std::size_t minimum_size()
    {
      return sizeof(std::uint32_t) +
             sizeof(std::uint32_t) +
             sizeof(std::uint32_t) +
             sizeof(std::uint32_t) +
             sizeof(std::uint32_t) +
             sizeof(std::uint64_t) +
             sizeof(std::uint64_t) +
             sizeof(std::uint32_t);
    }

    template <typename T>
    static bool read(const std::uint8_t *data,
                     std::size_t size,
                     std::size_t &offset,
                     T &value)
    {
      if (offset + sizeof(T) > size)
      {
        return false;
      }

      std::memcpy(&value, data + offset, sizeof(T));
      offset += sizeof(T);
      return true;
    }

    static bool read_string(const std::uint8_t *data,
                            std::size_t size,
                            std::size_t &offset,
                            std::string &out)
    {
      std::uint32_t length = 0;
      if (!read(data, size, offset, length))
      {
        return false;
      }

      if (offset + length > size)
      {
        return false;
      }

      out.assign(reinterpret_cast<const char *>(data + offset), length);
      offset += length;
      return true;
    }

    static bool read_capabilities(const std::uint8_t *data,
                                  std::size_t size,
                                  std::size_t &offset,
                                  std::vector<types::CapabilityType> &out)
    {
      std::uint32_t count = 0;
      if (!read(data, size, offset, count))
      {
        return false;
      }

      out.clear();
      out.reserve(count);

      for (std::uint32_t i = 0; i < count; ++i)
      {
        std::uint8_t raw = 0;
        if (!read(data, size, offset, raw))
        {
          return false;
        }

        out.push_back(static_cast<types::CapabilityType>(raw));
      }

      return true;
    }
  };

} // namespace softadastra::metadata::encoding

#endif
