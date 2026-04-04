/*
 * NodeCapabilities.hpp
 */

#ifndef SOFTADASTRA_METADATA_NODE_CAPABILITIES_HPP
#define SOFTADASTRA_METADATA_NODE_CAPABILITIES_HPP

#include <algorithm>
#include <vector>

#include <softadastra/metadata/types/CapabilityType.hpp>

namespace softadastra::metadata::core
{
  namespace types = softadastra::metadata::types;

  /**
   * @brief Declared capabilities of a node
   */
  struct NodeCapabilities
  {
    /**
     * Supported capabilities
     */
    std::vector<types::CapabilityType> values;

    /**
     * @brief Return true if a capability is present
     */
    bool has(types::CapabilityType capability) const noexcept
    {
      return std::find(values.begin(), values.end(), capability) != values.end();
    }

    /**
     * @brief Return true if at least one capability exists
     */
    bool valid() const noexcept
    {
      return !values.empty();
    }
  };

} // namespace softadastra::metadata::core

#endif
