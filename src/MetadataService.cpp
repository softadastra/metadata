/*
 * MetadataService.cpp
 */

#include <softadastra/metadata/MetadataService.hpp>

namespace softadastra::metadata
{
  MetadataService::MetadataService(const MetadataOptions &options,
                                   discovery_engine::DiscoveryEngine &discovery,
                                   metadata_backend::IMetadataProvider *provider)
      : options_(options),
        config_(make_config(options)),
        context_{},
        engine_([&]() -> metadata_core::MetadataContext &
                {
          context_.config = &config_;
          context_.discovery = &discovery;
          return context_; }(),
                provider)
  {
  }

  bool MetadataService::start()
  {
    if (!options_.valid())
    {
      return false;
    }

    return engine_.start();
  }

  void MetadataService::stop()
  {
    engine_.stop();
  }

  bool MetadataService::running() const noexcept
  {
    return engine_.running();
  }

  std::optional<metadata_core::NodeMetadata> MetadataService::local() const
  {
    return engine_.local_metadata();
  }

  std::optional<metadata_core::NodeMetadata> MetadataService::local_or_refresh()
  {
    return engine_.local_metadata_or_refresh();
  }

  std::optional<metadata_core::NodeMetadata> MetadataService::refresh()
  {
    return engine_.refresh_local();
  }

  std::optional<metadata_core::NodeMetadata> MetadataService::get(const std::string &node_id) const
  {
    return engine_.registry().get(node_id);
  }

  std::vector<metadata_core::NodeMetadata> MetadataService::all() const
  {
    return engine_.registry().all();
  }

  const MetadataOptions &MetadataService::options() const noexcept
  {
    return options_;
  }

  const metadata_core::MetadataConfig &MetadataService::config() const noexcept
  {
    return config_;
  }

  const metadata_core::MetadataContext &MetadataService::context() const noexcept
  {
    return context_;
  }

  metadata_core::MetadataConfig MetadataService::make_config(const MetadataOptions &options)
  {
    metadata_core::MetadataConfig config;
    config.node_id = options.node_id;
    config.display_name = options.display_name;
    config.version = options.version;
    config.auto_refresh = options.auto_refresh;
    config.refresh_interval_ms = options.refresh_interval_ms;
    return config;
  }

} // namespace softadastra::metadata
