/*
 * NodeIdentity.hpp
 */

#ifndef SOFTADASTRA_METADATA_NODE_IDENTITY_HPP
#define SOFTADASTRA_METADATA_NODE_IDENTITY_HPP

#include <string>

namespace softadastra::metadata::core
{
  /**
   * @brief Stable logical identity of a node
   */
  struct NodeIdentity
  {
    /**
     * Logical node identifier
     */
    std::string node_id;

    /**
     * Human-readable node display name
     */
    std::string display_name;

    /**
     * @brief Check whether this identity is usable
     */
    bool valid() const noexcept
    {
      return !node_id.empty();
    }
  };

} // namespace softadastra::metadata::core

#endif
