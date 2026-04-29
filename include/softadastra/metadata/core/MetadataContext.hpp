/**
 *
 *  @file MetadataContext.hpp
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

#ifndef SOFTADASTRA_METADATA_CONTEXT_HPP
#define SOFTADASTRA_METADATA_CONTEXT_HPP

#include <softadastra/core/Core.hpp>
#include <softadastra/discovery/engine/DiscoveryEngine.hpp>
#include <softadastra/metadata/core/MetadataConfig.hpp>

namespace softadastra::metadata::core
{
  namespace discovery_engine = softadastra::discovery::engine;
  namespace core_errors = softadastra::core::errors;
  namespace core_types = softadastra::core::types;

  /**
   * @brief Shared runtime dependencies for the metadata module.
   *
   * MetadataContext groups the objects required by the metadata layer.
   *
   * It provides access to:
   * - the active MetadataConfig
   * - the DiscoveryEngine used as peer visibility integration point
   *
   * The context does not own these objects.
   * The caller must ensure they outlive the metadata components using them.
   */
  struct MetadataContext
  {
    /**
     * @brief Metadata configuration.
     */
    const MetadataConfig *config{nullptr};

    /**
     * @brief Discovery engine used as peer visibility integration point.
     */
    discovery_engine::DiscoveryEngine *discovery{nullptr};

    /**
     * @brief Result type returned by checked accessors.
     */
    template <typename T>
    using Result = core_types::Result<T, core_errors::Error>;

    /**
     * @brief Creates an empty invalid context.
     */
    MetadataContext() = default;

    /**
     * @brief Creates a metadata context from dependencies.
     *
     * @param metadata_config Metadata configuration reference.
     * @param discovery_engine Discovery engine reference.
     */
    MetadataContext(
        const MetadataConfig &metadata_config,
        discovery_engine::DiscoveryEngine &discovery_engine) noexcept
        : config(&metadata_config),
          discovery(&discovery_engine)
    {
    }

    /**
     * @brief Returns true if the context is usable.
     *
     * @return true when config and discovery engine are present and valid.
     */
    [[nodiscard]] bool is_valid() const noexcept
    {
      return config != nullptr &&
             discovery != nullptr &&
             config->is_valid() &&
             discovery->is_running();
    }

    /**
     * @brief Backward-compatible valid alias.
     *
     * @return true when context is valid.
     */
    [[nodiscard]] bool valid() const noexcept
    {
      return is_valid();
    }

    /**
     * @brief Returns the metadata configuration pointer.
     *
     * @return MetadataConfig pointer, or nullptr.
     */
    [[nodiscard]] const MetadataConfig *
    config_ptr() const noexcept
    {
      return config;
    }

    /**
     * @brief Returns the discovery engine pointer.
     *
     * @return DiscoveryEngine pointer, or nullptr.
     */
    [[nodiscard]] discovery_engine::DiscoveryEngine *
    discovery_ptr() const noexcept
    {
      return discovery;
    }

    /**
     * @brief Returns the metadata configuration as a Result.
     *
     * @return MetadataConfig pointer on success, Error on failure.
     */
    [[nodiscard]] Result<const MetadataConfig *>
    config_checked() const
    {
      if (config == nullptr)
      {
        return Result<const MetadataConfig *>::err(
            core_errors::Error::make(
                core_errors::ErrorCode::InvalidState,
                "metadata context config is null"));
      }

      if (!config->is_valid())
      {
        return Result<const MetadataConfig *>::err(
            core_errors::Error::make(
                core_errors::ErrorCode::InvalidArgument,
                "metadata config is invalid"));
      }

      return Result<const MetadataConfig *>::ok(config);
    }

    /**
     * @brief Returns the discovery engine as a Result.
     *
     * @return DiscoveryEngine pointer on success, Error on failure.
     */
    [[nodiscard]] Result<discovery_engine::DiscoveryEngine *>
    discovery_checked() const
    {
      if (discovery == nullptr)
      {
        return Result<discovery_engine::DiscoveryEngine *>::err(
            core_errors::Error::make(
                core_errors::ErrorCode::InvalidState,
                "metadata context discovery engine is null"));
      }

      if (!discovery->is_running())
      {
        return Result<discovery_engine::DiscoveryEngine *>::err(
            core_errors::Error::make(
                core_errors::ErrorCode::InvalidState,
                "discovery engine is not running"));
      }

      return Result<discovery_engine::DiscoveryEngine *>::ok(discovery);
    }

    /**
     * @brief Returns true if a configuration is present.
     *
     * @return true when config is not null.
     */
    [[nodiscard]] bool has_config() const noexcept
    {
      return config != nullptr;
    }

    /**
     * @brief Returns true if a discovery engine is present.
     *
     * @return true when discovery is not null.
     */
    [[nodiscard]] bool has_discovery() const noexcept
    {
      return discovery != nullptr;
    }
  };

} // namespace softadastra::metadata::core

#endif // SOFTADASTRA_METADATA_CONTEXT_HPP
