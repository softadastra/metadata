/**
 *
 *  @file MetadataEngine.hpp
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

#ifndef SOFTADASTRA_METADATA_ENGINE_HPP
#define SOFTADASTRA_METADATA_ENGINE_HPP

#include <optional>
#include <string>
#include <utility>

#include <softadastra/core/Core.hpp>
#include <softadastra/metadata/backend/IMetadataProvider.hpp>
#include <softadastra/metadata/core/MetadataContext.hpp>
#include <softadastra/metadata/core/NodeCapabilities.hpp>
#include <softadastra/metadata/core/NodeIdentity.hpp>
#include <softadastra/metadata/core/NodeMetadata.hpp>
#include <softadastra/metadata/core/NodeRuntimeInfo.hpp>
#include <softadastra/metadata/registry/MetadataRegistry.hpp>
#include <softadastra/metadata/types/CapabilityType.hpp>
#include <softadastra/metadata/types/MetadataStatus.hpp>
#include <softadastra/metadata/utils/Hostname.hpp>
#include <softadastra/metadata/utils/PlatformInfo.hpp>
#include <softadastra/metadata/utils/VersionInfo.hpp>

namespace softadastra::metadata::engine
{
  namespace metadata_backend = softadastra::metadata::backend;
  namespace metadata_core = softadastra::metadata::core;
  namespace metadata_registry = softadastra::metadata::registry;
  namespace metadata_types = softadastra::metadata::types;
  namespace metadata_utils = softadastra::metadata::utils;

  namespace core_time = softadastra::core::time;

  /**
   * @brief Orchestrates metadata creation, refresh, and registry storage.
   *
   * MetadataEngine is the high-level metadata facade.
   *
   * Responsibilities:
   * - build local node metadata
   * - refresh runtime metadata
   * - store local metadata in the registry
   * - expose the current local metadata snapshot
   * - optionally use a custom metadata provider
   *
   * The engine does not own the MetadataContext.
   * The optional provider is also not owned.
   */
  class MetadataEngine : public softadastra::core::types::NonCopyable
  {
  public:
    /**
     * @brief Creates a metadata engine.
     *
     * @param context Metadata context.
     * @param provider Optional metadata provider.
     */
    explicit MetadataEngine(
        const metadata_core::MetadataContext &context,
        metadata_backend::IMetadataProvider *provider = nullptr) noexcept
        : context_(context),
          provider_(provider)
    {
    }

    /**
     * @brief Stops the metadata engine on destruction.
     */
    ~MetadataEngine()
    {
      stop();
    }

    /**
     * @brief Move constructor.
     */
    MetadataEngine(MetadataEngine &&) noexcept = default;

    /**
     * @brief Move assignment.
     */
    MetadataEngine &operator=(MetadataEngine &&) noexcept = default;

    /**
     * @brief Starts the metadata engine.
     *
     * @return true on success.
     */
    bool start()
    {
      if (metadata_types::is_running(status_))
      {
        return true;
      }

      if (!context_.is_valid())
      {
        status_ = metadata_types::MetadataStatus::Failed;
        return false;
      }

      status_ = metadata_types::MetadataStatus::Starting;
      started_at_ = core_time::Timestamp::now();

      const auto refreshed = refresh_local();

      if (!refreshed.has_value() ||
          !refreshed->is_valid())
      {
        status_ = metadata_types::MetadataStatus::Failed;
        return false;
      }

      status_ = metadata_types::MetadataStatus::Running;

      return true;
    }

    /**
     * @brief Stops the metadata engine.
     */
    void stop()
    {
      if (status_ == metadata_types::MetadataStatus::Stopped)
      {
        return;
      }

      status_ = metadata_types::MetadataStatus::Stopping;
      status_ = metadata_types::MetadataStatus::Stopped;
    }

    /**
     * @brief Returns current engine status.
     *
     * @return Metadata status.
     */
    [[nodiscard]] metadata_types::MetadataStatus status() const noexcept
    {
      return status_;
    }

    /**
     * @brief Returns true if the engine is running.
     *
     * @return true when running.
     */
    [[nodiscard]] bool is_running() const noexcept
    {
      return metadata_types::is_running(status_);
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
     * @brief Builds and refreshes local metadata.
     *
     * @return Refreshed local metadata, or std::nullopt.
     */
    std::optional<metadata_core::NodeMetadata> refresh_local()
    {
      std::optional<metadata_core::NodeMetadata> metadata;

      if (provider_ != nullptr)
      {
        metadata = provider_->refresh_local_metadata();
      }
      else
      {
        metadata = build_default_local_metadata();
      }

      if (!metadata.has_value() ||
          !metadata->is_valid())
      {
        return std::nullopt;
      }

      metadata->refresh_runtime();

      local_metadata_ = *metadata;
      registry_.upsert(*metadata);
      last_refresh_at_ = core_time::Timestamp::now();

      return local_metadata_;
    }

    /**
     * @brief Returns the current local metadata snapshot.
     *
     * @return Local metadata, or std::nullopt.
     */
    [[nodiscard]] std::optional<metadata_core::NodeMetadata>
    local_metadata() const
    {
      return local_metadata_;
    }

    /**
     * @brief Returns local metadata, refreshing automatically when needed.
     *
     * @return Local metadata, or std::nullopt.
     */
    std::optional<metadata_core::NodeMetadata>
    local_metadata_or_refresh()
    {
      if (!is_running())
      {
        return std::nullopt;
      }

      auto config_result = context_.config_checked();

      if (config_result.is_err())
      {
        return std::nullopt;
      }

      const auto &config = *config_result.value();

      if (!local_metadata_.has_value())
      {
        return refresh_local();
      }

      if (config.auto_refresh &&
          should_refresh(core_time::Timestamp::now()))
      {
        return refresh_local();
      }

      return local_metadata_;
    }

    /**
     * @brief Inserts or replaces metadata for one node.
     *
     * @param metadata Node metadata.
     */
    void upsert(const metadata_core::NodeMetadata &metadata)
    {
      if (!metadata.is_valid())
      {
        return;
      }

      registry_.upsert(metadata);
    }

    /**
     * @brief Inserts or replaces metadata for one node by move.
     *
     * @param metadata Node metadata.
     */
    void upsert(metadata_core::NodeMetadata &&metadata)
    {
      if (!metadata.is_valid())
      {
        return;
      }

      registry_.upsert(std::move(metadata));
    }

    /**
     * @brief Returns read-only access to the registry.
     *
     * @return Metadata registry.
     */
    [[nodiscard]] const metadata_registry::MetadataRegistry &
    registry() const noexcept
    {
      return registry_;
    }

    /**
     * @brief Returns mutable access to the registry.
     *
     * @return Metadata registry.
     */
    [[nodiscard]] metadata_registry::MetadataRegistry &
    registry() noexcept
    {
      return registry_;
    }

    /**
     * @brief Returns the local node id.
     *
     * @return Local node id.
     */
    [[nodiscard]] std::string node_id() const
    {
      auto config_result = context_.config_checked();

      if (config_result.is_err())
      {
        return {};
      }

      return config_result.value()->node_id;
    }

    /**
     * @brief Returns read-only access to context.
     *
     * @return Metadata context.
     */
    [[nodiscard]] const metadata_core::MetadataContext &
    context() const noexcept
    {
      return context_;
    }

  private:
    /**
     * @brief Builds default local metadata without a custom provider.
     */
    [[nodiscard]] std::optional<metadata_core::NodeMetadata>
    build_default_local_metadata() const
    {
      metadata_core::NodeMetadata metadata;

      metadata.identity = build_identity();
      metadata.runtime = build_runtime_info();
      metadata.capabilities = build_capabilities();

      if (!metadata.is_valid())
      {
        return std::nullopt;
      }

      return metadata;
    }

    /**
     * @brief Builds local node identity from config.
     */
    [[nodiscard]] metadata_core::NodeIdentity build_identity() const
    {
      auto config_result = context_.config_checked();

      if (config_result.is_err())
      {
        return {};
      }

      const auto &config = *config_result.value();

      return metadata_core::NodeIdentity{
          config.node_id,
          config.display_name};
    }

    /**
     * @brief Builds local runtime information from platform utilities.
     */
    [[nodiscard]] metadata_core::NodeRuntimeInfo build_runtime_info() const
    {
      auto config_result = context_.config_checked();

      if (config_result.is_err())
      {
        return {};
      }

      const auto &config = *config_result.value();

      const std::string version =
          config.version.empty()
              ? metadata_utils::VersionInfo::current()
              : config.version;

      metadata_core::NodeRuntimeInfo runtime{
          metadata_utils::Hostname::get(),
          metadata_utils::PlatformInfo::os_name(),
          version};

      runtime.started_at = started_at_.is_valid()
                               ? started_at_
                               : core_time::Timestamp::now();

      runtime.refresh_uptime();

      return runtime;
    }

    /**
     * @brief Builds default foundation capabilities.
     */
    [[nodiscard]] static metadata_core::NodeCapabilities
    build_capabilities()
    {
      return metadata_core::NodeCapabilities::foundation();
    }

    /**
     * @brief Returns true if local metadata should be refreshed.
     */
    [[nodiscard]] bool should_refresh(
        core_time::Timestamp now) const
    {
      auto config_result = context_.config_checked();

      if (config_result.is_err() ||
          !now.is_valid())
      {
        return false;
      }

      const auto &config = *config_result.value();

      if (!last_refresh_at_.is_valid())
      {
        return true;
      }

      if (now.millis() < last_refresh_at_.millis())
      {
        return true;
      }

      return now.millis() - last_refresh_at_.millis() >=
             config.refresh_interval.millis();
    }

  private:
    const metadata_core::MetadataContext &context_;
    metadata_backend::IMetadataProvider *provider_{nullptr};

    metadata_registry::MetadataRegistry registry_{};
    std::optional<metadata_core::NodeMetadata> local_metadata_{};

    metadata_types::MetadataStatus status_{
        metadata_types::MetadataStatus::Stopped};

    core_time::Timestamp started_at_{};
    core_time::Timestamp last_refresh_at_{};
  };

} // namespace softadastra::metadata::engine

#endif // SOFTADASTRA_METADATA_ENGINE_HPP
