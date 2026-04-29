/**
 *
 *  @file NodeIdentity.hpp
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

#ifndef SOFTADASTRA_METADATA_NODE_IDENTITY_HPP
#define SOFTADASTRA_METADATA_NODE_IDENTITY_HPP

#include <string>
#include <utility>

namespace softadastra::metadata::core
{
  /**
   * @brief Stable logical identity of a node.
   *
   * NodeIdentity contains the stable identifier and the optional
   * human-readable name used to display a node in tools, dashboards, logs,
   * registries, and diagnostics.
   */
  struct NodeIdentity
  {
    /**
     * @brief Logical node identifier.
     */
    std::string node_id{};

    /**
     * @brief Human-readable node display name.
     */
    std::string display_name{};

    /**
     * @brief Creates an empty invalid identity.
     */
    NodeIdentity() = default;

    /**
     * @brief Creates a node identity.
     *
     * @param id Logical node id.
     * @param name Human-readable display name.
     */
    NodeIdentity(
        std::string id,
        std::string name = {})
        : node_id(std::move(id)),
          display_name(std::move(name))
    {
      if (display_name.empty())
      {
        display_name = node_id;
      }
    }

    /**
     * @brief Creates a local node identity.
     *
     * @param node_id Local node id.
     * @return Node identity.
     */
    [[nodiscard]] static NodeIdentity local(std::string node_id)
    {
      return NodeIdentity{
          std::move(node_id),
          {}};
    }

    /**
     * @brief Returns true if this identity has a display name.
     *
     * @return true when display_name is not empty.
     */
    [[nodiscard]] bool has_display_name() const noexcept
    {
      return !display_name.empty();
    }

    /**
     * @brief Returns the best human-readable name.
     *
     * @return display_name when present, otherwise node_id.
     */
    [[nodiscard]] const std::string &label() const noexcept
    {
      return display_name.empty() ? node_id : display_name;
    }

    /**
     * @brief Returns true if this identity is usable.
     *
     * @return true when node_id is not empty.
     */
    [[nodiscard]] bool is_valid() const noexcept
    {
      return !node_id.empty();
    }

    /**
     * @brief Backward-compatible valid alias.
     *
     * @return true when identity is valid.
     */
    [[nodiscard]] bool valid() const noexcept
    {
      return is_valid();
    }

    /**
     * @brief Clears the identity.
     */
    void clear() noexcept
    {
      node_id.clear();
      display_name.clear();
    }
  };

} // namespace softadastra::metadata::core

#endif // SOFTADASTRA_METADATA_NODE_IDENTITY_HPP
