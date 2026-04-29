/**
 *
 *  @file NodeMetadata.hpp
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

#ifndef SOFTADASTRA_METADATA_NODE_METADATA_HPP
#define SOFTADASTRA_METADATA_NODE_METADATA_HPP

#include <utility>
#include <string>

#include <softadastra/metadata/core/NodeCapabilities.hpp>
#include <softadastra/metadata/core/NodeIdentity.hpp>
#include <softadastra/metadata/core/NodeRuntimeInfo.hpp>

namespace softadastra::metadata::core
{
  /**
   * @brief Aggregated metadata describing one node.
   *
   * NodeMetadata combines:
   * - stable node identity
   * - runtime information
   * - declared capabilities
   *
   * This is the main metadata object exchanged, stored, encoded, decoded,
   * and exposed by the metadata module.
   */
  struct NodeMetadata
  {
    /**
     * @brief Stable node identity.
     */
    NodeIdentity identity{};

    /**
     * @brief Runtime information.
     */
    NodeRuntimeInfo runtime{};

    /**
     * @brief Declared capabilities.
     */
    NodeCapabilities capabilities{};

    /**
     * @brief Creates an empty invalid metadata object.
     */
    NodeMetadata() = default;

    /**
     * @brief Creates metadata from identity, runtime info, and capabilities.
     *
     * @param node_identity Stable node identity.
     * @param runtime_info Runtime information.
     * @param node_capabilities Declared capabilities.
     */
    NodeMetadata(
        NodeIdentity node_identity,
        NodeRuntimeInfo runtime_info,
        NodeCapabilities node_capabilities)
        : identity(std::move(node_identity)),
          runtime(std::move(runtime_info)),
          capabilities(std::move(node_capabilities))
    {
    }

    /**
     * @brief Creates minimal metadata for a node.
     *
     * @param node_id Local node id.
     * @param hostname Local hostname.
     * @param os_name Operating system name.
     * @param version Runtime or product version.
     * @return Node metadata.
     */
    [[nodiscard]] static NodeMetadata minimal(
        std::string node_id,
        std::string hostname,
        std::string os_name,
        std::string version)
    {
      return NodeMetadata{
          NodeIdentity::local(std::move(node_id)),
          NodeRuntimeInfo{
              std::move(hostname),
              std::move(os_name),
              std::move(version)},
          NodeCapabilities::minimal()};
    }

    /**
     * @brief Creates foundation metadata for a node.
     *
     * @param node_id Local node id.
     * @param hostname Local hostname.
     * @param os_name Operating system name.
     * @param version Runtime or product version.
     * @return Node metadata.
     */
    [[nodiscard]] static NodeMetadata foundation(
        std::string node_id,
        std::string hostname,
        std::string os_name,
        std::string version)
    {
      return NodeMetadata{
          NodeIdentity::local(std::move(node_id)),
          NodeRuntimeInfo{
              std::move(hostname),
              std::move(os_name),
              std::move(version)},
          NodeCapabilities::foundation()};
    }

    /**
     * @brief Returns the node id.
     *
     * @return Node id.
     */
    [[nodiscard]] const std::string &node_id() const noexcept
    {
      return identity.node_id;
    }

    /**
     * @brief Returns the display label.
     *
     * @return Display name when present, otherwise node id.
     */
    [[nodiscard]] const std::string &label() const noexcept
    {
      return identity.label();
    }

    /**
     * @brief Returns true if the metadata declares a capability.
     *
     * @param capability Capability to search.
     * @return true when capability is present.
     */
    [[nodiscard]] bool has_capability(
        types::CapabilityType capability) const noexcept
    {
      return capabilities.has(capability);
    }

    /**
     * @brief Refreshes runtime information.
     */
    void refresh_runtime() noexcept
    {
      runtime.refresh_uptime();
    }

    /**
     * @brief Returns true if this metadata is usable.
     *
     * @return true when identity, runtime, and capabilities are valid.
     */
    [[nodiscard]] bool is_valid() const noexcept
    {
      return identity.is_valid() &&
             runtime.is_valid() &&
             capabilities.is_valid();
    }

    /**
     * @brief Backward-compatible valid alias.
     *
     * @return true when metadata is valid.
     */
    [[nodiscard]] bool valid() const noexcept
    {
      return is_valid();
    }

    /**
     * @brief Clears all metadata fields.
     */
    void clear() noexcept
    {
      identity.clear();
      runtime.clear();
      capabilities.clear();
    }
  };

} // namespace softadastra::metadata::core

#endif // SOFTADASTRA_METADATA_NODE_METADATA_HPP
