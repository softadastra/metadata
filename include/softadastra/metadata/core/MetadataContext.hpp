/*
 * MetadataContext.hpp
 */

#ifndef SOFTADASTRA_METADATA_CONTEXT_HPP
#define SOFTADASTRA_METADATA_CONTEXT_HPP

#include <stdexcept>

#include <softadastra/discovery/engine/DiscoveryEngine.hpp>
#include <softadastra/metadata/core/MetadataConfig.hpp>

namespace softadastra::metadata::core
{
  namespace discovery_engine = softadastra::discovery::engine;

  /**
   * @brief Shared runtime dependencies for the metadata module
   *
   * The metadata layer can use DiscoveryEngine as an integration point
   * for enriching discovered peers with node metadata.
   */
  struct MetadataContext
  {
    /**
     * Metadata configuration
     */
    const MetadataConfig *config{nullptr};

    /**
     * Discovery engine used as peer visibility integration point
     */
    discovery_engine::DiscoveryEngine *discovery{nullptr};

    /**
     * @brief Check whether the context is usable
     */
    bool valid() const noexcept
    {
      return config != nullptr &&
             discovery != nullptr &&
             config->valid();
    }

    /**
     * @brief Return metadata configuration
     */
    const MetadataConfig &config_ref() const
    {
      if (config == nullptr)
      {
        throw std::runtime_error(
            "MetadataContext: config is null");
      }

      return *config;
    }

    /**
     * @brief Return discovery engine reference
     */
    discovery_engine::DiscoveryEngine &discovery_ref() const
    {
      if (discovery == nullptr)
      {
        throw std::runtime_error(
            "MetadataContext: discovery is null");
      }

      return *discovery;
    }
  };

} // namespace softadastra::metadata::core

#endif
