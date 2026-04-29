/**
 *
 *  @file NodeCapabilities.hpp
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

#ifndef SOFTADASTRA_METADATA_NODE_CAPABILITIES_HPP
#define SOFTADASTRA_METADATA_NODE_CAPABILITIES_HPP

#include <algorithm>
#include <initializer_list>
#include <vector>

#include <softadastra/metadata/types/CapabilityType.hpp>

namespace softadastra::metadata::core
{
  namespace types = softadastra::metadata::types;

  /**
   * @brief Declared capabilities of a node.
   *
   * NodeCapabilities stores the list of Softadastra subsystems supported by
   * a node.
   *
   * The list is kept unique by add().
   */
  struct NodeCapabilities
  {
    /**
     * @brief Supported capabilities.
     */
    std::vector<types::CapabilityType> values{};

    /**
     * @brief Creates an empty capabilities set.
     */
    NodeCapabilities() = default;

    /**
     * @brief Creates capabilities from an initializer list.
     *
     * Invalid capabilities are ignored.
     *
     * @param capabilities Capability list.
     */
    NodeCapabilities(
        std::initializer_list<types::CapabilityType> capabilities)
    {
      for (const auto capability : capabilities)
      {
        add(capability);
      }
    }

    /**
     * @brief Creates the default Softadastra foundation capabilities.
     *
     * @return NodeCapabilities.
     */
    [[nodiscard]] static NodeCapabilities foundation()
    {
      return NodeCapabilities{
          types::CapabilityType::Core,
          types::CapabilityType::Fs,
          types::CapabilityType::Wal,
          types::CapabilityType::Store,
          types::CapabilityType::Sync,
          types::CapabilityType::Transport,
          types::CapabilityType::Discovery,
          types::CapabilityType::Metadata};
    }

    /**
     * @brief Creates minimal node capabilities.
     *
     * @return NodeCapabilities.
     */
    [[nodiscard]] static NodeCapabilities minimal()
    {
      return NodeCapabilities{
          types::CapabilityType::Core,
          types::CapabilityType::Metadata};
    }

    /**
     * @brief Adds a capability if it is valid and not already present.
     *
     * @param capability Capability to add.
     * @return true if inserted.
     */
    bool add(types::CapabilityType capability)
    {
      if (!types::is_valid(capability) || has(capability))
      {
        return false;
      }

      values.push_back(capability);
      return true;
    }

    /**
     * @brief Removes a capability.
     *
     * @param capability Capability to remove.
     * @return true if removed.
     */
    bool remove(types::CapabilityType capability)
    {
      const auto it =
          std::remove(values.begin(), values.end(), capability);

      if (it == values.end())
      {
        return false;
      }

      values.erase(it, values.end());
      return true;
    }

    /**
     * @brief Returns true if a capability is present.
     *
     * @param capability Capability to search.
     * @return true when present.
     */
    [[nodiscard]] bool has(types::CapabilityType capability) const noexcept
    {
      return std::find(
                 values.begin(),
                 values.end(),
                 capability) != values.end();
    }

    /**
     * @brief Returns true if at least one foundation capability exists.
     *
     * @return true when a foundation capability is present.
     */
    [[nodiscard]] bool has_foundation_capability() const noexcept
    {
      return std::any_of(
          values.begin(),
          values.end(),
          [](types::CapabilityType capability)
          {
            return types::is_foundation(capability);
          });
    }

    /**
     * @brief Returns true if at least one user-facing capability exists.
     *
     * @return true when App or CLI is present.
     */
    [[nodiscard]] bool has_user_facing_capability() const noexcept
    {
      return std::any_of(
          values.begin(),
          values.end(),
          [](types::CapabilityType capability)
          {
            return types::is_user_facing(capability);
          });
    }

    /**
     * @brief Returns number of capabilities.
     *
     * @return Capability count.
     */
    [[nodiscard]] std::size_t size() const noexcept
    {
      return values.size();
    }

    /**
     * @brief Returns true if no capability is present.
     *
     * @return true when empty.
     */
    [[nodiscard]] bool empty() const noexcept
    {
      return values.empty();
    }

    /**
     * @brief Clears all capabilities.
     */
    void clear() noexcept
    {
      values.clear();
    }

    /**
     * @brief Returns true if all stored capabilities are valid.
     *
     * @return true when non-empty and all capabilities are valid.
     */
    [[nodiscard]] bool is_valid() const noexcept
    {
      return !values.empty() &&
             std::all_of(
                 values.begin(),
                 values.end(),
                 [](types::CapabilityType capability)
                 {
                   return types::is_valid(capability);
                 });
    }

    /**
     * @brief Backward-compatible valid alias.
     *
     * @return true when capabilities are valid.
     */
    [[nodiscard]] bool valid() const noexcept
    {
      return is_valid();
    }
  };

} // namespace softadastra::metadata::core

#endif // SOFTADASTRA_METADATA_NODE_CAPABILITIES_HPP
