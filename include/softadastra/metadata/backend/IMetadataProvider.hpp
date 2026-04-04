/*
 * IMetadataProvider.hpp
 */

#ifndef SOFTADASTRA_METADATA_I_METADATA_PROVIDER_HPP
#define SOFTADASTRA_METADATA_I_METADATA_PROVIDER_HPP

#include <optional>

#include <softadastra/metadata/core/NodeMetadata.hpp>

namespace softadastra::metadata::backend
{
  namespace core = softadastra::metadata::core;

  /**
   * @brief Abstract metadata provider interface
   *
   * A provider is responsible for building or refreshing node metadata.
   * It does not contain registry or engine orchestration logic.
   */
  class IMetadataProvider
  {
  public:
    virtual ~IMetadataProvider() = default;

    /**
     * @brief Return a snapshot of local node metadata
     */
    virtual std::optional<core::NodeMetadata> local_metadata() const = 0;

    /**
     * @brief Refresh and return local node metadata
     */
    virtual std::optional<core::NodeMetadata> refresh_local_metadata() = 0;
  };

} // namespace softadastra::metadata::backend

#endif
