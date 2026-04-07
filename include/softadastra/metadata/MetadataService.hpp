/*
 * MetadataService.hpp
 */

#ifndef SOFTADASTRA_METADATA_SERVICE_HPP
#define SOFTADASTRA_METADATA_SERVICE_HPP

#include <optional>
#include <vector>

#include <softadastra/discovery/engine/DiscoveryEngine.hpp>
#include <softadastra/metadata/MetadataOptions.hpp>
#include <softadastra/metadata/backend/IMetadataProvider.hpp>
#include <softadastra/metadata/core/MetadataConfig.hpp>
#include <softadastra/metadata/core/MetadataContext.hpp>
#include <softadastra/metadata/core/NodeMetadata.hpp>
#include <softadastra/metadata/engine/MetadataEngine.hpp>

namespace softadastra::metadata
{
  namespace metadata_backend = softadastra::metadata::backend;
  namespace metadata_core = softadastra::metadata::core;
  namespace metadata_engine = softadastra::metadata::engine;
  namespace metadata_registry = softadastra::metadata::registry;
  namespace discovery_engine = softadastra::discovery::engine;

  class MetadataService
  {
  public:
    MetadataService(const MetadataOptions &options,
                    discovery_engine::DiscoveryEngine &discovery,
                    metadata_backend::IMetadataProvider *provider = nullptr);

    bool start();
    void stop();
    bool running() const noexcept;

    std::optional<metadata_core::NodeMetadata> local() const;
    std::optional<metadata_core::NodeMetadata> local_or_refresh();
    std::optional<metadata_core::NodeMetadata> refresh();

    std::optional<metadata_core::NodeMetadata> get(const std::string &node_id) const;
    std::vector<metadata_core::NodeMetadata> all() const;

    const MetadataOptions &options() const noexcept;
    const metadata_core::MetadataConfig &config() const noexcept;
    const metadata_core::MetadataContext &context() const noexcept;

  private:
    static metadata_core::MetadataConfig make_config(const MetadataOptions &options);

  private:
    MetadataOptions options_;
    metadata_core::MetadataConfig config_;
    metadata_core::MetadataContext context_;
    metadata_engine::MetadataEngine engine_;
  };

} // namespace softadastra::metadata

#endif
