/*
 * MetadataConfig.hpp
 */

#ifndef SOFTADASTRA_METADATA_CONFIG_HPP
#define SOFTADASTRA_METADATA_CONFIG_HPP

#include <cstdint>
#include <string>

namespace softadastra::metadata::core
{
  /**
   * @brief Runtime configuration for the metadata layer
   */
  struct MetadataConfig
  {
    /**
     * Local logical node identifier
     */
    std::string node_id;

    /**
     * Human-readable node display name
     */
    std::string display_name;

    /**
     * Product or runtime version string
     */
    std::string version{"0.1.0"};

    /**
     * Enable automatic runtime refresh
     */
    bool auto_refresh{true};

    /**
     * Runtime refresh interval in milliseconds
     */
    std::uint64_t refresh_interval_ms{5000};

    /**
     * @brief Check whether the configuration is minimally valid
     */
    bool valid() const noexcept
    {
      return !node_id.empty() &&
             !version.empty() &&
             refresh_interval_ms > 0;
    }
  };

} // namespace softadastra::metadata::core

#endif
