/**
 *
 *  @file CapabilityType.hpp
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

#ifndef SOFTADASTRA_METADATA_CAPABILITY_TYPE_HPP
#define SOFTADASTRA_METADATA_CAPABILITY_TYPE_HPP

#include <cstdint>
#include <string_view>

namespace softadastra::metadata::types
{
  /**
   * @brief Capability exposed by a Softadastra node.
   *
   * CapabilityType describes which Softadastra subsystems are available on the
   * current node.
   *
   * It is used by:
   * - NodeCapabilities
   * - NodeMetadata
   * - MetadataRegistry
   * - MetadataEncoder
   * - MetadataDecoder
   *
   * Rules:
   * - Values must remain stable over time.
   * - Do not reorder existing values.
   * - Do not remove existing values once released.
   * - Add new values only at the end.
   */
  enum class CapabilityType : std::uint8_t
  {
    /**
     * @brief Unknown or invalid capability.
     */
    Unknown = 0,

    /**
     * @brief Core runtime capability.
     */
    Core,

    /**
     * @brief Filesystem observation capability.
     */
    Fs,

    /**
     * @brief Write-ahead log capability.
     */
    Wal,

    /**
     * @brief Local store capability.
     */
    Store,

    /**
     * @brief Sync engine capability.
     */
    Sync,

    /**
     * @brief Transport capability.
     */
    Transport,

    /**
     * @brief Discovery capability.
     */
    Discovery,

    /**
     * @brief Metadata capability.
     */
    Metadata,

    /**
     * @brief Application runtime capability.
     */
    App,

    /**
     * @brief CLI capability.
     */
    Cli
  };

  /**
   * @brief Returns a stable string representation of a capability type.
   *
   * @param capability Capability type.
   * @return Stable string representation.
   */
  [[nodiscard]] constexpr std::string_view
  to_string(CapabilityType capability) noexcept
  {
    switch (capability)
    {
    case CapabilityType::Unknown:
      return "unknown";

    case CapabilityType::Core:
      return "core";

    case CapabilityType::Fs:
      return "fs";

    case CapabilityType::Wal:
      return "wal";

    case CapabilityType::Store:
      return "store";

    case CapabilityType::Sync:
      return "sync";

    case CapabilityType::Transport:
      return "transport";

    case CapabilityType::Discovery:
      return "discovery";

    case CapabilityType::Metadata:
      return "metadata";

    case CapabilityType::App:
      return "app";

    case CapabilityType::Cli:
      return "cli";

    default:
      return "invalid";
    }
  }

  /**
   * @brief Returns true if the capability is known and usable.
   *
   * Unknown is intentionally treated as invalid.
   *
   * @param capability Capability type.
   * @return true for usable capabilities.
   */
  [[nodiscard]] constexpr bool is_valid(CapabilityType capability) noexcept
  {
    return capability == CapabilityType::Core ||
           capability == CapabilityType::Fs ||
           capability == CapabilityType::Wal ||
           capability == CapabilityType::Store ||
           capability == CapabilityType::Sync ||
           capability == CapabilityType::Transport ||
           capability == CapabilityType::Discovery ||
           capability == CapabilityType::Metadata ||
           capability == CapabilityType::App ||
           capability == CapabilityType::Cli;
  }

  /**
   * @brief Returns true if the capability is part of the Softadastra runtime foundation.
   *
   * @param capability Capability type.
   * @return true for core infrastructure capabilities.
   */
  [[nodiscard]] constexpr bool is_foundation(CapabilityType capability) noexcept
  {
    return capability == CapabilityType::Core ||
           capability == CapabilityType::Fs ||
           capability == CapabilityType::Wal ||
           capability == CapabilityType::Store ||
           capability == CapabilityType::Sync ||
           capability == CapabilityType::Transport ||
           capability == CapabilityType::Discovery ||
           capability == CapabilityType::Metadata;
  }

  /**
   * @brief Returns true if the capability is user-facing.
   *
   * @param capability Capability type.
   * @return true for application or CLI capabilities.
   */
  [[nodiscard]] constexpr bool is_user_facing(CapabilityType capability) noexcept
  {
    return capability == CapabilityType::App ||
           capability == CapabilityType::Cli;
  }

} // namespace softadastra::metadata::types

#endif // SOFTADASTRA_METADATA_CAPABILITY_TYPE_HPP
