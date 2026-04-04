/*
 * MetadataEngine.hpp
 */

#ifndef SOFTADASTRA_METADATA_ENGINE_HPP
#define SOFTADASTRA_METADATA_ENGINE_HPP

#include <cstdint>
#include <ctime>
#include <optional>
#include <stdexcept>
#include <string>

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

  /**
   * @brief Orchestrates metadata creation, refresh, and registry storage
   *
   * Responsibilities:
   * - build local node metadata
   * - refresh runtime metadata
   * - store local metadata in the registry
   * - expose the current local metadata snapshot
   */
  class MetadataEngine
  {
  public:
    explicit MetadataEngine(const metadata_core::MetadataContext &context,
                            metadata_backend::IMetadataProvider *provider = nullptr)
        : context_(context),
          provider_(provider)
    {
      if (!context_.valid())
      {
        throw std::runtime_error("MetadataEngine: invalid MetadataContext");
      }
    }

    /**
     * @brief Start the metadata engine
     */
    bool start()
    {
      if (status_ == metadata_types::MetadataStatus::Running)
      {
        return true;
      }

      status_ = metadata_types::MetadataStatus::Starting;
      started_at_ = now_ms();

      const auto refreshed = refresh_local();

      if (!refreshed.has_value() || !refreshed->valid())
      {
        status_ = metadata_types::MetadataStatus::Failed;
        return false;
      }

      status_ = metadata_types::MetadataStatus::Running;
      return true;
    }

    /**
     * @brief Stop the metadata engine
     */
    void stop()
    {
      status_ = metadata_types::MetadataStatus::Stopping;
      status_ = metadata_types::MetadataStatus::Stopped;
    }

    /**
     * @brief Return current engine status
     */
    metadata_types::MetadataStatus status() const noexcept
    {
      return status_;
    }

    /**
     * @brief Return true if the engine is running
     */
    bool running() const noexcept
    {
      return status_ == metadata_types::MetadataStatus::Running;
    }

    /**
     * @brief Build and refresh local metadata
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

      if (!metadata.has_value() || !metadata->valid())
      {
        return std::nullopt;
      }

      local_metadata_ = *metadata;
      registry_.upsert(*metadata);
      last_refresh_at_ = now_ms();

      return local_metadata_;
    }

    /**
     * @brief Return the current local metadata snapshot
     */
    std::optional<metadata_core::NodeMetadata> local_metadata() const
    {
      return local_metadata_;
    }

    /**
     * @brief Return the local metadata, refreshing automatically when needed
     */
    std::optional<metadata_core::NodeMetadata> local_metadata_or_refresh()
    {
      if (!running())
      {
        return std::nullopt;
      }

      const auto &config = context_.config_ref();

      if (!local_metadata_.has_value())
      {
        return refresh_local();
      }

      if (config.auto_refresh && should_refresh(now_ms()))
      {
        return refresh_local();
      }

      return local_metadata_;
    }

    /**
     * @brief Insert or replace metadata for one node
     */
    void upsert(const metadata_core::NodeMetadata &metadata)
    {
      if (!metadata.valid())
      {
        return;
      }

      registry_.upsert(metadata);
    }

    /**
     * @brief Insert or replace metadata for one node by move
     */
    void upsert(metadata_core::NodeMetadata &&metadata)
    {
      if (!metadata.valid())
      {
        return;
      }

      registry_.upsert(std::move(metadata));
    }

    /**
     * @brief Return read-only access to the registry
     */
    const metadata_registry::MetadataRegistry &registry() const noexcept
    {
      return registry_;
    }

    /**
     * @brief Return the local node id
     */
    const std::string &node_id() const
    {
      return context_.config_ref().node_id;
    }

  private:
    std::optional<metadata_core::NodeMetadata> build_default_local_metadata() const
    {
      metadata_core::NodeMetadata metadata;

      metadata.identity = build_identity();
      metadata.runtime = build_runtime_info();
      metadata.capabilities = build_capabilities();

      if (!metadata.valid())
      {
        return std::nullopt;
      }

      return metadata;
    }

    metadata_core::NodeIdentity build_identity() const
    {
      metadata_core::NodeIdentity identity;
      identity.node_id = context_.config_ref().node_id;
      identity.display_name = context_.config_ref().display_name;
      return identity;
    }

    metadata_core::NodeRuntimeInfo build_runtime_info() const
    {
      metadata_core::NodeRuntimeInfo runtime;
      runtime.hostname = metadata_utils::Hostname::get();
      runtime.os_name = metadata_utils::PlatformInfo::os_name();
      runtime.version = context_.config_ref().version.empty()
                            ? metadata_utils::VersionInfo::current()
                            : context_.config_ref().version;
      runtime.started_at = started_at_;
      runtime.uptime_ms = compute_uptime_ms();
      return runtime;
    }

    metadata_core::NodeCapabilities build_capabilities() const
    {
      metadata_core::NodeCapabilities capabilities;
      capabilities.values = {
          metadata_types::CapabilityType::Core,
          metadata_types::CapabilityType::Fs,
          metadata_types::CapabilityType::Wal,
          metadata_types::CapabilityType::Store,
          metadata_types::CapabilityType::Sync,
          metadata_types::CapabilityType::Transport,
          metadata_types::CapabilityType::Discovery,
          metadata_types::CapabilityType::Metadata};
      return capabilities;
    }

    bool should_refresh(std::uint64_t now) const noexcept
    {
      const auto &config = context_.config_ref();

      if (last_refresh_at_ == 0)
      {
        return true;
      }

      return now > last_refresh_at_ &&
             (now - last_refresh_at_) >= config.refresh_interval_ms;
    }

    std::uint64_t compute_uptime_ms() const noexcept
    {
      const std::uint64_t now = now_ms();

      if (started_at_ == 0 || now < started_at_)
      {
        return 0;
      }

      return now - started_at_;
    }

    static std::uint64_t now_ms()
    {
      return static_cast<std::uint64_t>(std::time(nullptr)) * 1000ULL;
    }

  private:
    const metadata_core::MetadataContext &context_;
    metadata_backend::IMetadataProvider *provider_{nullptr};

    metadata_registry::MetadataRegistry registry_;
    std::optional<metadata_core::NodeMetadata> local_metadata_;

    metadata_types::MetadataStatus status_{metadata_types::MetadataStatus::Stopped};
    std::uint64_t started_at_{0};
    std::uint64_t last_refresh_at_{0};
  };

} // namespace softadastra::metadata::engine

#endif
