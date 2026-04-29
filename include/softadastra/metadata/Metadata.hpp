/**
 *
 *  @file Metadata.hpp
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

#ifndef SOFTADASTRA_METADATA_HPP
#define SOFTADASTRA_METADATA_HPP

/*
 * Public API
 */
#include <softadastra/metadata/MetadataOptions.hpp>
#include <softadastra/metadata/MetadataService.hpp>

/*
 * Types
 */
#include <softadastra/metadata/types/CapabilityType.hpp>
#include <softadastra/metadata/types/MetadataField.hpp>
#include <softadastra/metadata/types/MetadataStatus.hpp>

/*
 * Core
 */
#include <softadastra/metadata/core/MetadataConfig.hpp>
#include <softadastra/metadata/core/MetadataContext.hpp>
#include <softadastra/metadata/core/NodeCapabilities.hpp>
#include <softadastra/metadata/core/NodeIdentity.hpp>
#include <softadastra/metadata/core/NodeMetadata.hpp>
#include <softadastra/metadata/core/NodeRuntimeInfo.hpp>

/*
 * Encoding
 */
#include <softadastra/metadata/encoding/MetadataDecoder.hpp>
#include <softadastra/metadata/encoding/MetadataEncoder.hpp>

/*
 * Registry
 */
#include <softadastra/metadata/registry/MetadataRegistry.hpp>

/*
 * Backend
 */
#include <softadastra/metadata/backend/IMetadataProvider.hpp>

/*
 * Engine
 */
#include <softadastra/metadata/engine/MetadataEngine.hpp>

/*
 * Utils
 */
#include <softadastra/metadata/utils/Hostname.hpp>
#include <softadastra/metadata/utils/PlatformInfo.hpp>
#include <softadastra/metadata/utils/VersionInfo.hpp>

#endif // SOFTADASTRA_METADATA_HPP
