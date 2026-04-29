/**
 *
 *  @file MetadataRegistry.hpp
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

#ifndef SOFTADASTRA_METADATA_REGISTRY_HPP
#define SOFTADASTRA_METADATA_REGISTRY_HPP

#include <cstddef>
#include <optional>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include <softadastra/metadata/core/NodeMetadata.hpp>
#include <softadastra/metadata/types/CapabilityType.hpp>

namespace softadastra::metadata::registry
{
  namespace core = softadastra::metadata::core;
  namespace types = softadastra::metadata::types;

  /**
   * @brief In-memory registry of known node metadata.
   *
   * MetadataRegistry stores NodeMetadata indexed by node id.
   *
   * This first version is intentionally simple:
   * - in-memory only
   * - unordered_map-backed
   * - node_id as stable key
   *
   * It does not perform network I/O.
   * It only stores and exposes metadata snapshots.
   */
  class MetadataRegistry
  {
  public:
    /**
     * @brief Internal registry map type.
     */
    using Map = std::unordered_map<std::string, core::NodeMetadata>;

    /**
     * @brief Creates an empty metadata registry.
     */
    MetadataRegistry() = default;

    /**
     * @brief Inserts or replaces node metadata.
     *
     * Invalid metadata is ignored.
     *
     * @param metadata Node metadata.
     */
    void upsert(const core::NodeMetadata &metadata)
    {
      if (!metadata.is_valid())
      {
        return;
      }

      registry_[metadata.node_id()] = metadata;
    }

    /**
     * @brief Inserts or replaces node metadata by move.
     *
     * Invalid metadata is ignored.
     *
     * @param metadata Node metadata.
     */
    void upsert(core::NodeMetadata &&metadata)
    {
      if (!metadata.is_valid())
      {
        return;
      }

      const auto key = metadata.node_id();

      registry_[key] = std::move(metadata);
    }

    /**
     * @brief Returns true if metadata exists for the given node id.
     *
     * @param node_id Node id.
     * @return true if found.
     */
    [[nodiscard]] bool contains(const std::string &node_id) const
    {
      return registry_.find(node_id) != registry_.end();
    }

    /**
     * @brief Gets node metadata by copy.
     *
     * @param node_id Node id.
     * @return Node metadata if found, std::nullopt otherwise.
     */
    [[nodiscard]] std::optional<core::NodeMetadata>
    get(const std::string &node_id) const
    {
      const auto it = registry_.find(node_id);

      if (it == registry_.end())
      {
        return std::nullopt;
      }

      return it->second;
    }

    /**
     * @brief Finds node metadata without copying.
     *
     * @param node_id Node id.
     * @return NodeMetadata pointer, or nullptr.
     */
    [[nodiscard]] core::NodeMetadata *
    find(const std::string &node_id) noexcept
    {
      const auto it = registry_.find(node_id);

      if (it == registry_.end())
      {
        return nullptr;
      }

      return &it->second;
    }

    /**
     * @brief Finds node metadata without copying.
     *
     * @param node_id Node id.
     * @return NodeMetadata pointer, or nullptr.
     */
    [[nodiscard]] const core::NodeMetadata *
    find(const std::string &node_id) const noexcept
    {
      const auto it = registry_.find(node_id);

      if (it == registry_.end())
      {
        return nullptr;
      }

      return &it->second;
    }

    /**
     * @brief Removes metadata by node id.
     *
     * @param node_id Node id.
     * @return true if metadata was removed.
     */
    bool erase(const std::string &node_id)
    {
      return registry_.erase(node_id) > 0;
    }

    /**
     * @brief Returns number of registered metadata entries.
     *
     * @return Metadata count.
     */
    [[nodiscard]] std::size_t size() const noexcept
    {
      return registry_.size();
    }

    /**
     * @brief Returns true if registry is empty.
     *
     * @return true when empty.
     */
    [[nodiscard]] bool empty() const noexcept
    {
      return registry_.empty();
    }

    /**
     * @brief Removes all metadata entries.
     */
    void clear() noexcept
    {
      registry_.clear();
    }

    /**
     * @brief Returns read-only access to internal entries.
     *
     * @return Metadata map.
     */
    [[nodiscard]] const Map &entries() const noexcept
    {
      return registry_;
    }

    /**
     * @brief Returns all node metadata entries.
     *
     * @return Metadata entries.
     */
    [[nodiscard]] std::vector<core::NodeMetadata> all() const
    {
      std::vector<core::NodeMetadata> result;
      result.reserve(registry_.size());

      for (const auto &[_, metadata] : registry_)
      {
        result.push_back(metadata);
      }

      return result;
    }

    /**
     * @brief Returns metadata entries matching a capability.
     *
     * @param capability Capability to filter by.
     * @return Metadata entries.
     */
    [[nodiscard]] std::vector<core::NodeMetadata>
    with_capability(types::CapabilityType capability) const
    {
      std::vector<core::NodeMetadata> result;

      if (!types::is_valid(capability))
      {
        return result;
      }

      for (const auto &[_, metadata] : registry_)
      {
        if (metadata.has_capability(capability))
        {
          result.push_back(metadata);
        }
      }

      return result;
    }

    /**
     * @brief Returns metadata entries exposing foundation capabilities.
     *
     * @return Metadata entries.
     */
    [[nodiscard]] std::vector<core::NodeMetadata>
    foundation_nodes() const
    {
      std::vector<core::NodeMetadata> result;

      for (const auto &[_, metadata] : registry_)
      {
        if (metadata.capabilities.has_foundation_capability())
        {
          result.push_back(metadata);
        }
      }

      return result;
    }

    /**
     * @brief Returns metadata entries exposing user-facing capabilities.
     *
     * @return Metadata entries.
     */
    [[nodiscard]] std::vector<core::NodeMetadata>
    user_facing_nodes() const
    {
      std::vector<core::NodeMetadata> result;

      for (const auto &[_, metadata] : registry_)
      {
        if (metadata.capabilities.has_user_facing_capability())
        {
          result.push_back(metadata);
        }
      }

      return result;
    }

    /**
     * @brief Refreshes runtime data for all stored metadata entries.
     */
    void refresh_all_runtime() noexcept
    {
      for (auto &[_, metadata] : registry_)
      {
        metadata.refresh_runtime();
      }
    }

  private:
    Map registry_{};
  };

} // namespace softadastra::metadata::registry

#endif // SOFTADASTRA_METADATA_REGISTRY_HPP
