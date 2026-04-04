/*
 * CapabilityType.hpp
 */

#ifndef SOFTADASTRA_METADATA_CAPABILITY_TYPE_HPP
#define SOFTADASTRA_METADATA_CAPABILITY_TYPE_HPP

#include <cstdint>

namespace softadastra::metadata::types
{
  /**
   * @brief Capability exposed by a node
   *
   * Describes which Softadastra subsystems are available
   * on the current node.
   */
  enum class CapabilityType : std::uint8_t
  {
    Unknown = 0,

    Core,
    Fs,
    Wal,
    Store,
    Sync,
    Transport,
    Discovery,
    Metadata,
    App,
    Cli
  };

} // namespace softadastra::metadata::types

#endif
