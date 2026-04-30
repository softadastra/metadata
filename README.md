# softadastra/metadata

> Node metadata layer for Softadastra.

`softadastra/metadata` is the module responsible for describing a Softadastra node.

It exposes node identity, runtime information, capabilities, version information, platform information, and metadata snapshots that can be stored, encoded, decoded, refreshed, and shared across the system.

The core rule is:

> *Metadata describes nodes. Discovery finds nodes. Transport connects nodes.*

## Purpose

Softadastra is built around local-first and offline-first systems. Each node needs a clear description of itself:

- who it is
- what version it runs
- what platform it runs on
- what capabilities it supports
- when it started
- how long it has been running

The metadata module provides this description. It helps Softadastra:

- build local node metadata
- refresh runtime metadata
- track node capabilities
- encode and decode metadata snapshots
- store metadata from multiple nodes
- filter nodes by capability
- expose a simple high-level metadata service
- integrate with discovery visibility

## What this module does

`softadastra/metadata` provides:

- metadata status types and capability types
- metadata field identifiers
- node identity, runtime information, and capabilities
- complete node metadata
- metadata configuration and context
- metadata provider interface
- metadata encoder and decoder
- metadata registry, engine, and service
- platform utilities (hostname, version, OS info)

## What this module does NOT do

- WAL persistence
- store mutation
- sync propagation
- transport delivery
- peer discovery
- conflict resolution or distributed consensus
- encryption or authentication

## Design Principles

### Metadata is descriptive

Metadata does not mutate application state. It only describes a node and its runtime environment.

### Metadata is local-first

Every node can build its own metadata without requiring a server.

### Metadata is observable

Metadata can be inspected, encoded, decoded, stored, and filtered.

### Metadata is capability-oriented

Nodes declare what they support through `NodeCapabilities`, enabling queries like:

```
Which nodes support Sync?
Which nodes expose user-facing apps?
Which nodes are foundation nodes?
```

### Layering

```
WAL        →  durable operation log
Store      →  local state
Sync       →  operation propagation
Transport  →  peer message delivery
Discovery  →  peer discovery
Metadata   →  node description
```

> Discovery finds a node. Metadata explains what that node is.

## Installation

```bash
vix add @softadastra/metadata
```

### Main header

```cpp
#include <softadastra/metadata/Metadata.hpp>
```

For full integration:

```cpp
#include <softadastra/store/Store.hpp>
#include <softadastra/sync/Sync.hpp>
#include <softadastra/transport/Transport.hpp>
#include <softadastra/discovery/Discovery.hpp>
#include <softadastra/metadata/Metadata.hpp>
```

## Module Structure

```
include/softadastra/metadata/
├── backend/
│   └── IMetadataProvider.hpp
├── core/
│   ├── MetadataConfig.hpp
│   ├── MetadataContext.hpp
│   ├── NodeCapabilities.hpp
│   ├── NodeIdentity.hpp
│   ├── NodeMetadata.hpp
│   └── NodeRuntimeInfo.hpp
├── encoding/
│   ├── MetadataDecoder.hpp
│   └── MetadataEncoder.hpp
├── engine/
│   └── MetadataEngine.hpp
├── registry/
│   └── MetadataRegistry.hpp
├── types/
│   ├── CapabilityType.hpp
│   ├── MetadataField.hpp
│   └── MetadataStatus.hpp
├── utils/
│   ├── Hostname.hpp
│   ├── PlatformInfo.hpp
│   └── VersionInfo.hpp
├── Metadata.hpp
├── MetadataOptions.hpp
└── MetadataService.hpp
```

## Core Concepts

### `CapabilityType`

Describes what a node supports:

- `metadata::types::CapabilityType::Core`
- `metadata::types::CapabilityType::Fs`
- `metadata::types::CapabilityType::Wal`
- `metadata::types::CapabilityType::Store`
- `metadata::types::CapabilityType::Sync`
- `metadata::types::CapabilityType::Transport`
- `metadata::types::CapabilityType::Discovery`
- `metadata::types::CapabilityType::Metadata`
- `metadata::types::CapabilityType::App`
- `metadata::types::CapabilityType::Cli`

Helpers:

```cpp
metadata::types::to_string(capability);
metadata::types::is_valid(capability);
metadata::types::is_foundation(capability);
metadata::types::is_user_facing(capability);
```

### `MetadataField`

Identifies logical fields in `NodeMetadata`:

- `NodeId`, `DisplayName`, `Hostname`, `OsName`, `Version`
- `StartedAt`, `UptimeMs`, `Capabilities`

Helpers:

```cpp
metadata::types::to_string(field);
metadata::types::is_valid(field);
metadata::types::is_identity_field(field);
metadata::types::is_runtime_field(field);
```

### `MetadataStatus`

- `Stopped`, `Starting`, `Running`, `Stopping`, `Failed`

Helpers:

```cpp
metadata::types::to_string(status);
metadata::types::is_valid(status);
metadata::types::is_running(status);
metadata::types::is_transitioning(status);
metadata::types::is_terminal(status);
```

## `MetadataConfig`

Low-level runtime configuration. Contains: `node_id`, `display_name`, `version`, `auto_refresh`, `refresh_interval`.

```cpp
auto config = metadata::core::MetadataConfig::local("node-a", "1.0.0");

config.refresh_interval = core::time::Duration::from_seconds(5);

if (!config.is_valid())
{
    return 1;
}

// Backward-compatible alias
config.valid();
```

## `MetadataOptions`

User-facing configuration used by `MetadataService`.

```cpp
auto options = metadata::MetadataOptions::local("node-a", "1.0.0");

auto config = options.to_config();

if (!options.is_valid())
{
    return 1;
}

// Backward-compatible alias
options.valid();
```

## `NodeIdentity`

Stores stable node identity.

```cpp
metadata::core::NodeIdentity identity{"node-a", "Node A"};

// Local helper
auto identity = metadata::core::NodeIdentity::local("node-a");

auto label = identity.label();

if (!identity.is_valid())
{
    return 1;
}
```

## `NodeRuntimeInfo`

Describes runtime state.

```cpp
metadata::core::NodeRuntimeInfo runtime{
    metadata::utils::Hostname::get(),
    metadata::utils::PlatformInfo::os_name(),
    metadata::utils::VersionInfo::current()};

runtime.refresh_uptime();

auto uptime_ms = runtime.uptime_ms();

runtime.reset_start_time();

if (!runtime.is_valid())
{
    return 1;
}
```

## `NodeCapabilities`

Stores supported capabilities.

```cpp
metadata::core::NodeCapabilities capabilities;

capabilities.add(metadata::types::CapabilityType::Core);
capabilities.add(metadata::types::CapabilityType::Store);
capabilities.add(metadata::types::CapabilityType::Sync);

// Presets
auto capabilities = metadata::core::NodeCapabilities::foundation();
auto capabilities = metadata::core::NodeCapabilities::minimal();

// Check
capabilities.has(metadata::types::CapabilityType::Sync);
capabilities.has_foundation_capability();
capabilities.has_user_facing_capability();

// Remove
capabilities.remove(metadata::types::CapabilityType::Sync);
```

## `NodeMetadata`

The complete metadata object. Combines `NodeIdentity`, `NodeRuntimeInfo`, and `NodeCapabilities`.

```cpp
// Foundation metadata
auto metadata_snapshot = metadata::core::NodeMetadata::foundation(
    "node-a",
    metadata::utils::Hostname::get(),
    metadata::utils::PlatformInfo::os_name(),
    "1.0.0");

// Minimal metadata
auto metadata_snapshot = metadata::core::NodeMetadata::minimal(
    "node-a", "host-a", "linux", "1.0.0");

metadata_snapshot.node_id();
metadata_snapshot.label();
metadata_snapshot.has_capability(metadata::types::CapabilityType::Sync);
metadata_snapshot.refresh_runtime();

if (!metadata_snapshot.is_valid())
{
    return 1;
}
```

## Encoding and Decoding

```cpp
// Encode
auto encoded = metadata::encoding::MetadataEncoder::encode(metadata_snapshot);

// Decode
auto decoded = metadata::encoding::MetadataDecoder::decode(encoded);

if (decoded.has_value())
{
    auto node_id = decoded->node_id();
}
```

Payload format:

```
uint32  node_id_size
bytes   node_id
uint32  display_name_size
bytes   display_name
uint32  hostname_size
bytes   hostname
uint32  os_name_size
bytes   os_name
uint32  version_size
bytes   version
int64   started_at_millis
int64   uptime_millis
uint32  capability_count
uint8   capability_0
uint8   capability_1
...
```

## `MetadataRegistry`

Stores known node metadata in memory.

```cpp
metadata::registry::MetadataRegistry registry;

registry.upsert(metadata_snapshot);

// Get
auto found = registry.get("node-a");

// Find without copying
auto *entry = registry.find("node-a");

if (entry != nullptr)
{
    entry->refresh_runtime();
}

// Filter
auto sync_nodes      = registry.with_capability(metadata::types::CapabilityType::Sync);
auto foundation_nodes = registry.foundation_nodes();
auto app_nodes       = registry.user_facing_nodes();

// Refresh all
registry.refresh_all_runtime();
```

## `IMetadataProvider`

Custom provider interface.

```cpp
class MyProvider final : public metadata::backend::IMetadataProvider
{
public:
    std::optional<metadata::core::NodeMetadata>
    local_metadata() const override
    {
        return metadata_;
    }

    std::optional<metadata::core::NodeMetadata>
    refresh_local_metadata() override
    {
        metadata_ = metadata::core::NodeMetadata::foundation(
            "custom-node", "custom-host", "linux", "1.0.0");

        metadata_->refresh_runtime();

        return metadata_;
    }

private:
    std::optional<metadata::core::NodeMetadata> metadata_;
};
```

## `MetadataContext`

Connects metadata to discovery.

```cpp
metadata::core::MetadataContext context{metadata_config, discovery_engine};

if (!context.is_valid())
{
    return 1;
}

auto config    = context.config_checked();
auto discovery = context.discovery_checked();
```

## `MetadataEngine`

The high-level metadata facade. Coordinates: local metadata creation, runtime refresh, registry storage, and optional custom provider usage.

```cpp
auto metadata_config = metadata::core::MetadataConfig::local("node-a", "1.0.0");

metadata::core::MetadataContext metadata_context{metadata_config, discovery_engine};

metadata::engine::MetadataEngine metadata_engine{metadata_context};

if (!metadata_engine.start())
{
    return 1;
}

auto local = metadata_engine.refresh_local();
auto local = metadata_engine.local_metadata();
auto local = metadata_engine.local_metadata_or_refresh();

auto all = metadata_engine.registry().all();

metadata_engine.stop();
```

## `MetadataService`

The simple user-facing wrapper. Owns: `MetadataOptions`, `MetadataConfig`, `MetadataContext`, `MetadataEngine`. Does **not** own `DiscoveryEngine` — the discovery engine must outlive the metadata service.

```cpp
auto options = metadata::MetadataOptions::local("node-a", "1.0.0");

metadata::MetadataService service{options, discovery_engine};

if (!service.start())
{
    return 1;
}

auto local     = service.local();
auto refreshed = service.refresh();
auto local     = service.local_or_refresh();

auto node       = service.get("node-a");
auto entries    = service.all();
auto sync_nodes = service.with_capability(metadata::types::CapabilityType::Sync);

service.stop();
```

## Utilities

### `Hostname`

```cpp
auto hostname = metadata::utils::Hostname::get();

if (metadata::utils::Hostname::is_valid(hostname))
{
    // usable hostname
}
```

### `PlatformInfo`

```cpp
auto os           = metadata::utils::PlatformInfo::os_name();
auto compiler     = metadata::utils::PlatformInfo::compiler_name();
auto architecture = metadata::utils::PlatformInfo::architecture();
auto build_mode   = metadata::utils::PlatformInfo::build_mode();
auto summary      = metadata::utils::PlatformInfo::summary();
```

### `VersionInfo`

```cpp
auto version = metadata::utils::VersionInfo::current();

if (metadata::utils::VersionInfo::is_valid(version))
{
    // usable version
}
```

## Full Integration Example

```cpp
#include <filesystem>
#include <iostream>

#include <softadastra/store/Store.hpp>
#include <softadastra/sync/Sync.hpp>
#include <softadastra/transport/Transport.hpp>
#include <softadastra/discovery/Discovery.hpp>
#include <softadastra/metadata/Metadata.hpp>

using namespace softadastra;

int main()
{
    const std::string wal_path = "metadata_node.wal";
    std::filesystem::remove(wal_path);

    store::engine::StoreEngine store{
        store::core::StoreConfig::durable(wal_path)};

    auto sync_config = sync::core::SyncConfig::durable("node-a");

    sync::core::SyncContext  sync_context{store, sync_config};
    sync::engine::SyncEngine sync_engine{sync_context};

    auto transport_config = transport::core::TransportConfig::local(7300);

    transport::core::TransportContext transport_context{
        transport_config,
        sync_engine};

    transport::backend::TcpTransportBackend transport_backend{transport_config};

    transport::engine::TransportEngine transport_engine{
        transport_context,
        transport_backend};

    if (!transport_engine.start())
    {
        return 1;
    }

    auto discovery_config =
        discovery::core::DiscoveryConfig::local("node-a", 9500, 7300);

    discovery::core::DiscoveryContext discovery_context{
        discovery_config,
        transport_engine};

    discovery::backend::UdpDiscoveryBackend discovery_backend{discovery_config};

    discovery::engine::DiscoveryEngine discovery_engine{
        discovery_context,
        discovery_backend};

    if (!discovery_engine.start())
    {
        transport_engine.stop();
        return 1;
    }

    auto metadata_options = metadata::MetadataOptions::local("node-a", "1.0.0");

    metadata::MetadataService metadata_service{metadata_options, discovery_engine};

    if (!metadata_service.start())
    {
        discovery_engine.stop();
        transport_engine.stop();
        return 1;
    }

    auto local = metadata_service.local_or_refresh();

    if (local.has_value())
    {
        std::cout << "node id:      " << local->node_id()          << "\n";
        std::cout << "hostname:     " << local->runtime.hostname   << "\n";
        std::cout << "os:           " << local->runtime.os_name    << "\n";
        std::cout << "capabilities: " << local->capabilities.size() << "\n";
    }

    metadata_service.stop();
    discovery_engine.stop();
    transport_engine.stop();

    std::filesystem::remove(wal_path);

    return 0;
}
```

## Metadata Flow

```
1.  MetadataConfig defines local node identity and version
2.  MetadataEngine starts
3.  MetadataEngine builds NodeIdentity
4.  MetadataEngine builds NodeRuntimeInfo
5.  MetadataEngine builds NodeCapabilities
6.  MetadataEngine aggregates them into NodeMetadata
7.  NodeMetadata is stored in MetadataRegistry
8.  Metadata can be encoded, decoded, queried, or refreshed
```

## Capability Flow

```
1.  Node declares capabilities
2.  NodeMetadata stores them
3.  MetadataRegistry indexes the metadata
4.  Callers filter nodes by capability
5.  Higher layers can choose compatible nodes
```

---

## Examples

| Example | Description |
|---------|-------------|
| `metadata_minimal.cpp` | Minimal setup |
| `metadata_local_snapshot.cpp` | Local snapshot |
| `metadata_encode_decode_demo.cpp` | Encoding and decoding |
| `metadata_registry_demo.cpp` | Registry usage |
| `metadata_capabilities.cpp` | Capability declaration |
| `metadata_registry_filter.cpp` | Filtering by capability |
| `metadata_custom_provider.cpp` | Custom provider |
| `metadata_service_demo.cpp` | Service usage |

```bash
vix build
```

## Production Notes

The current metadata module is intentionally simple. Recommended next steps:

- signed metadata snapshots
- metadata trust policy
- metadata exchange over transport
- metadata persistence and peer cache
- metadata version negotiation and compatibility checks
- structured metadata tracing and metrics
- metadata filtering by fields
- metadata export formats

## Design Rules

- Metadata describes nodes
- Discovery finds nodes
- Transport connects nodes
- Sync owns operation meaning
- Store owns state
- WAL owns durability
- Metadata must not mutate store
- Metadata must not apply sync operations
- Metadata must not connect peers directly
- Metadata must stay inspectable and serializable
- Providers must not contain registry orchestration logic
- Registry must remain in-memory and deterministic for now

## Dependencies

**Internal:**
- `softadastra/core`
- `softadastra/store`
- `softadastra/discovery`

**External:**
- C++20 standard library

## Roadmap

- [ ] Public `Metadata.hpp` aggregator
- [ ] Stable `MetadataService`
- [ ] Metadata encoding and decoding
- [ ] Metadata registry filters
- [ ] Custom metadata providers
- [ ] Metadata exchange over transport
- [ ] Metadata persistence
- [ ] Metadata trust policy and signed metadata
- [ ] Version compatibility checks
- [ ] Capability negotiation
- [ ] Metadata diagnostics, metrics, and tracing
- [ ] Field-level metadata export

## Summary

`softadastra/metadata` provides:

- identity, runtime info, and capabilities
- metadata snapshots
- encoding and decoding
- registry, engine, and service
- platform utilities

> Its job is simple: describe what a node is and what it can do.
