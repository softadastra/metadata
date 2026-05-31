/**
 *
 *  @file MetadataService.hpp
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

#ifndef SOFTADASTRA_METADATA_SERVICE_HPP
#define SOFTADASTRA_METADATA_SERVICE_HPP

#include <optional>
#include <string>
#include <utility>
#include <vector>

#include <softadastra/discovery/engine/DiscoveryEngine.hpp>
#include <softadastra/metadata/MetadataOptions.hpp>
#include <softadastra/metadata/backend/IMetadataProvider.hpp>
#include <softadastra/metadata/core/MetadataConfig.hpp>
#include <softadastra/metadata/core/MetadataContext.hpp>
#include <softadastra/metadata/core/NodeMetadata.hpp>
#include <softadastra/metadata/engine/MetadataEngine.hpp>
#include <softadastra/metadata/registry/MetadataRegistry.hpp>
#include <softadastra/metadata/types/CapabilityType.hpp>

namespace softadastra::metadata
{
  namespace metadata_backend = softadastra::metadata::backend;
  namespace metadata_core = softadastra::metadata::core;
  namespace metadata_engine = softadastra::metadata::engine;
  namespace metadata_registry = softadastra::metadata::registry;
  namespace metadata_types = softadastra::metadata::types;
  namespace discovery_engine = softadastra::discovery::engine;

  /**
   * @brief High-level metadata service.
   *
   * MetadataService is the simple user-facing wrapper around MetadataEngine.
   *
   * It owns:
   * - MetadataOptions
   * - MetadataConfig
   * - MetadataContext
   * - MetadataEngine
   *
   * It does not own the DiscoveryEngine.
   * The discovery engine must outlive this service.
   *
   * The optional metadata provider is not owned either.
   */
  class MetadataService
  {
  public:
    /**
     * @brief Creates a metadata service.
     *
     * @param options Public metadata options.
     * @param discovery Discovery engine used as integration point.
     * @param provider Optional metadata provider.
     */
    MetadataService(
        MetadataOptions options,
        discovery_engine::DiscoveryEngine &discovery,
        metadata_backend::IMetadataProvider *provider = nullptr)
        : options_(std::move(options)),
          config_(options_.to_config()),
          context_(config_, discovery),
          engine_(context_, provider)
    {
    }

    /**
     * @brief Creates a standalone metadata service.
     *
     * This constructor is used when metadata is needed without discovery.
     *
     * @param options Public metadata options.
     * @param provider Optional metadata provider.
     */
    MetadataService(
        MetadataOptions options,
        metadata_backend::IMetadataProvider *provider = nullptr)
        : options_(std::move(options)),
          config_(options_.to_config()),
          context_(config_),
          engine_(context_, provider)
    {
    }

    /**
     * @brief Starts metadata service.
     *
     * @return true on success.
     */
    bool start()
    {
      if (!options_.is_valid())
      {
        return false;
      }

      return engine_.start();
    }

    /**
     * @brief Stops metadata service.
     */
    void stop()
    {
      engine_.stop();
    }

    /**
     * @brief Returns whether metadata is running.
     *
     * @return true when running.
     */
    [[nodiscard]] bool is_running() const noexcept
    {
      return engine_.is_running();
    }

    /**
     * @brief Backward-compatible running alias.
     *
     * @return true when running.
     */
    [[nodiscard]] bool running() const noexcept
    {
      return is_running();
    }

    /**
     * @brief Returns the current local metadata snapshot.
     *
     * @return Local metadata, or std::nullopt.
     */
    [[nodiscard]] std::optional<metadata_core::NodeMetadata>
    local() const
    {
      return engine_.local_metadata();
    }

    /**
     * @brief Returns the current local metadata, refreshing if needed.
     *
     * @return Local metadata, or std::nullopt.
     */
    std::optional<metadata_core::NodeMetadata>
    local_or_refresh()
    {
      return engine_.local_metadata_or_refresh();
    }

    /**
     * @brief Refreshes local metadata immediately.
     *
     * @return Refreshed local metadata, or std::nullopt.
     */
    std::optional<metadata_core::NodeMetadata>
    refresh()
    {
      return engine_.refresh_local();
    }

    /**
     * @brief Inserts or replaces metadata for one node.
     *
     * @param metadata Node metadata.
     */
    void upsert(const metadata_core::NodeMetadata &metadata)
    {
      engine_.upsert(metadata);
    }

    /**
     * @brief Inserts or replaces metadata for one node by move.
     *
     * @param metadata Node metadata.
     */
    void upsert(metadata_core::NodeMetadata &&metadata)
    {
      engine_.upsert(std::move(metadata));
    }

    /**
     * @brief Gets metadata for one node.
     *
     * @param node_id Node id.
     * @return Node metadata, or std::nullopt.
     */
    [[nodiscard]] std::optional<metadata_core::NodeMetadata>
    get(const std::string &node_id) const
    {
      return engine_.registry().get(node_id);
    }

    /**
     * @brief Returns all known metadata entries.
     *
     * @return Metadata entries.
     */
    [[nodiscard]] std::vector<metadata_core::NodeMetadata>
    all() const
    {
      return engine_.registry().all();
    }

    /**
     * @brief Returns metadata entries matching a capability.
     *
     * @param capability Capability to filter by.
     * @return Metadata entries.
     */
    [[nodiscard]] std::vector<metadata_core::NodeMetadata>
    with_capability(metadata_types::CapabilityType capability) const
    {
      return engine_.registry().with_capability(capability);
    }

    /**
     * @brief Returns all foundation nodes.
     *
     * @return Metadata entries.
     */
    [[nodiscard]] std::vector<metadata_core::NodeMetadata>
    foundation_nodes() const
    {
      return engine_.registry().foundation_nodes();
    }

    /**
     * @brief Returns all user-facing nodes.
     *
     * @return Metadata entries.
     */
    [[nodiscard]] std::vector<metadata_core::NodeMetadata>
    user_facing_nodes() const
    {
      return engine_.registry().user_facing_nodes();
    }

    /**
     * @brief Returns read-only access to the metadata engine.
     *
     * @return MetadataEngine const reference.
     */
    [[nodiscard]] const metadata_engine::MetadataEngine &
    engine() const noexcept
    {
      return engine_;
    }

    /**
     * @brief Returns mutable access to the metadata engine.
     *
     * @return MetadataEngine reference.
     */
    [[nodiscard]] metadata_engine::MetadataEngine &
    engine() noexcept
    {
      return engine_;
    }

    /**
     * @brief Returns read-only access to registry.
     *
     * @return MetadataRegistry const reference.
     */
    [[nodiscard]] const metadata_registry::MetadataRegistry &
    registry() const noexcept
    {
      return engine_.registry();
    }

    /**
     * @brief Returns mutable access to registry.
     *
     * @return MetadataRegistry reference.
     */
    [[nodiscard]] metadata_registry::MetadataRegistry &
    registry() noexcept
    {
      return engine_.registry();
    }

    /**
     * @brief Returns the public options.
     *
     * @return Metadata options.
     */
    [[nodiscard]] const MetadataOptions &
    options() const noexcept
    {
      return options_;
    }

    /**
     * @brief Returns the core metadata config.
     *
     * @return Metadata config.
     */
    [[nodiscard]] const metadata_core::MetadataConfig &
    config() const noexcept
    {
      return config_;
    }

    /**
     * @brief Returns the metadata context.
     *
     * @return Metadata context.
     */
    [[nodiscard]] const metadata_core::MetadataContext &
    context() const noexcept
    {
      return context_;
    }

  private:
    MetadataOptions options_{};
    metadata_core::MetadataConfig config_{};
    metadata_core::MetadataContext context_{};
    metadata_engine::MetadataEngine engine_;
  };

} // namespace softadastra::metadata

#endif // SOFTADASTRA_METADATA_SERVICE_HPP
