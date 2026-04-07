/*
 * MetadataOptions.hpp
 */

#ifndef SOFTADASTRA_METADATA_OPTIONS_HPP
#define SOFTADASTRA_METADATA_OPTIONS_HPP

#include <cstdint>
#include <string>

namespace softadastra::metadata
{
  struct MetadataOptions
  {
    std::string node_id;
    std::string display_name;
    std::string version{"0.1.0"};

    bool auto_refresh{true};
    std::uint64_t refresh_interval_ms{5000};

    bool valid() const noexcept
    {
      return !node_id.empty() &&
             !version.empty() &&
             refresh_interval_ms > 0;
    }
  };

} // namespace softadastra::metadata

#endif
