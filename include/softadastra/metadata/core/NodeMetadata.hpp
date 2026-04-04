/*
 * NodeMetadata.hpp
 */

#ifndef SOFTADASTRA_METADATA_NODE_METADATA_HPP
#define SOFTADASTRA_METADATA_NODE_METADATA_HPP

#include <softadastra/metadata/core/NodeCapabilities.hpp>
#include <softadastra/metadata/core/NodeIdentity.hpp>
#include <softadastra/metadata/core/NodeRuntimeInfo.hpp>

namespace softadastra::metadata::core
{
  /**
   * @brief Aggregated metadata describing one node
   */
  struct NodeMetadata
  {
    /**
     * Stable node identity
     */
    NodeIdentity identity;

    /**
     * Runtime information
     */
    NodeRuntimeInfo runtime;

    /**
     * Declared capabilities
     */
    NodeCapabilities capabilities;

    /**
     * @brief Check whether this metadata is usable
     */
    bool valid() const noexcept
    {
      return identity.valid() &&
             runtime.valid();
    }
  };

} // namespace softadastra::metadata::core

#endif
