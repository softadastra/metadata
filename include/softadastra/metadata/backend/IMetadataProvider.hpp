/**
 *
 *  @file IMetadataProvider.hpp
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

#ifndef SOFTADASTRA_METADATA_I_METADATA_PROVIDER_HPP
#define SOFTADASTRA_METADATA_I_METADATA_PROVIDER_HPP

#include <optional>

#include <softadastra/core/Core.hpp>
#include <softadastra/metadata/core/NodeMetadata.hpp>

namespace softadastra::metadata::backend
{
  namespace core = softadastra::metadata::core;
  namespace core_types = softadastra::core::types;

  /**
   * @brief Abstract metadata provider interface.
   *
   * IMetadataProvider defines the provider contract used by the metadata
   * engine.
   *
   * A provider is responsible for:
   * - building local node metadata
   * - returning local metadata snapshots
   * - refreshing runtime metadata
   *
   * It must not contain registry orchestration logic.
   * It must not contain discovery integration logic.
   */
  class IMetadataProvider : public core_types::NonCopyable
  {
  public:
    /**
     * @brief Default virtual destructor.
     */
    virtual ~IMetadataProvider() = default;

    /**
     * @brief Move constructor.
     */
    IMetadataProvider(IMetadataProvider &&) noexcept = default;

    /**
     * @brief Move assignment.
     */
    IMetadataProvider &operator=(IMetadataProvider &&) noexcept = default;

    /**
     * @brief Returns a snapshot of local node metadata.
     *
     * @return Local node metadata, or std::nullopt when unavailable.
     */
    [[nodiscard]] virtual std::optional<core::NodeMetadata>
    local_metadata() const = 0;

    /**
     * @brief Refreshes and returns local node metadata.
     *
     * @return Refreshed local node metadata, or std::nullopt when unavailable.
     */
    [[nodiscard]] virtual std::optional<core::NodeMetadata>
    refresh_local_metadata() = 0;

  protected:
    /**
     * @brief Protected default constructor.
     */
    IMetadataProvider() = default;
  };

} // namespace softadastra::metadata::backend

#endif // SOFTADASTRA_METADATA_I_METADATA_PROVIDER_HPP
