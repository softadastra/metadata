/*
 * MetadataRegistry.hpp
 */

#ifndef SOFTADASTRA_METADATA_REGISTRY_HPP
#define SOFTADASTRA_METADATA_REGISTRY_HPP

#include <cstddef>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include <softadastra/metadata/core/NodeMetadata.hpp>

namespace softadastra::metadata::registry
{
  namespace core = softadastra::metadata::core;

  /**
   * @brief In-memory registry of known node metadata
   */
  class MetadataRegistry
  {
  public:
    /**
     * @brief Insert or replace node metadata
     */
    void upsert(const core::NodeMetadata &metadata)
    {
      registry_[metadata.identity.node_id] = metadata;
    }

    /**
     * @brief Insert or replace node metadata by move
     */
    void upsert(core::NodeMetadata &&metadata)
    {
      registry_[metadata.identity.node_id] = std::move(metadata);
    }

    /**
     * @brief Return true if metadata exists for the given node id
     */
    bool contains(const std::string &node_id) const
    {
      return registry_.find(node_id) != registry_.end();
    }

    /**
     * @brief Get node metadata copy
     */
    std::optional<core::NodeMetadata> get(const std::string &node_id) const
    {
      auto it = registry_.find(node_id);
      if (it == registry_.end())
      {
        return std::nullopt;
      }

      return it->second;
    }

    /**
     * @brief Remove metadata by node id
     */
    bool erase(const std::string &node_id)
    {
      return registry_.erase(node_id) > 0;
    }

    /**
     * @brief Number of registered metadata entries
     */
    std::size_t size() const noexcept
    {
      return registry_.size();
    }

    /**
     * @brief Return true if registry is empty
     */
    bool empty() const noexcept
    {
      return registry_.empty();
    }

    /**
     * @brief Remove all metadata entries
     */
    void clear()
    {
      registry_.clear();
    }

    /**
     * @brief Return all node metadata entries
     */
    std::vector<core::NodeMetadata> all() const
    {
      std::vector<core::NodeMetadata> result;
      result.reserve(registry_.size());

      for (const auto &[_, metadata] : registry_)
      {
        result.push_back(metadata);
      }

      return result;
    }

  private:
    std::unordered_map<std::string, core::NodeMetadata> registry_;
  };

} // namespace softadastra::metadata::registry

#endif
