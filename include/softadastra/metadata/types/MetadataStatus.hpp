/*
 * MetadataStatus.hpp
 */

#ifndef SOFTADASTRA_METADATA_STATUS_HPP
#define SOFTADASTRA_METADATA_STATUS_HPP

#include <cstdint>

namespace softadastra::metadata::types
{
  /**
   * @brief Runtime state of the metadata engine
   */
  enum class MetadataStatus : std::uint8_t
  {
    Stopped = 0,
    Starting,
    Running,
    Stopping,
    Failed
  };

} // namespace softadastra::metadata::types

#endif
