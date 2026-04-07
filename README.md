# Softadastra Metadata

**Node metadata for real-world distributed systems.**

Softadastra Metadata provides a unified way to describe, collect, and share information about nodes in a distributed system.

It allows you to:
- describe node identity and capabilities
- expose runtime information (OS, uptime, version)
- enrich discovered peers with metadata
- maintain a consistent view of the network

Built for systems that must operate reliably under unstable or local-first conditions.

---

## Why Metadata matters

In distributed systems:

- nodes are dynamic
- environments are heterogeneous
- identities are not always known in advance
- systems must adapt at runtime

Softadastra Metadata solves this by enabling:

> **Consistent, structured node information across the network.**

No assumptions. No central authority. Just observable nodes.

---

## Key Features

- 🧠 Structured node metadata (identity, runtime, capabilities)
- 🔁 Automatic refresh of runtime information
- 🔍 Integration with discovery layer
- 🌍 Works in offline-first environments
- ⚡ Lightweight and fast
- 🧩 Simple public API via `MetadataService`

---

## Installation

```bash
vix add @softadastra/metadata
vix install
```

## Quick Start

```cpp
#include <softadastra/metadata/metadata.hpp>

using namespace softadastra::metadata;

MetadataOptions options;
options.node_id = "node-a";
options.display_name = "Node A";
options.version = "0.1.0";

MetadataService metadata(options, discovery_engine);

metadata.start();

auto local = metadata.local_or_refresh();

if (local)
{
    std::cout << local->identity.node_id << std::endl;
    std::cout << local->runtime.hostname << std::endl;
}
```

---

## Concepts

### MetadataService

The main entry point of the module.

Responsible for:

- building local node metadata
- refreshing runtime information
- exposing metadata to other modules
- maintaining a registry of known nodes

### MetadataOptions

Simple configuration:

```cpp
MetadataOptions options;
options.node_id = "node-a";
options.display_name = "Node A";
options.version = "0.1.0";
```

### NodeMetadata

Represents the full state of a node:

```cpp
NodeMetadata metadata;

metadata.identity.node_id;
metadata.runtime.hostname;
metadata.runtime.os_name;
metadata.runtime.uptime_ms;
metadata.capabilities.values;
```

---

## How it works

- Metadata is built from local providers
- Runtime information is collected (hostname, OS, uptime)
- Metadata is refreshed periodically
- Discovery layer provides visibility of peers
- Metadata is attached to known nodes
- A registry maintains all known metadata

---

## Architecture

MetadataService (public API)
        ↓
MetadataEngine (orchestration)
        ↓
IMetadataProvider (data source)
        ↓
MetadataRegistry (state)

---

## Examples

Build:

```bash
vix build
```

Run minimal example:

```bash
./build-ninja/examples/metadata_minimal
```

---

## Designed for the real world

Softadastra Metadata is not built for ideal conditions.

It is built for:

- dynamic nodes
- unstable environments
- local-first systems
- peer-to-peer architectures

---

## Position in Softadastra

Metadata sits on top of discovery:

Discovery → Metadata → Transport → Sync → Store

- Discovery finds peers
- Metadata describes them
- Transport connects them
- Sync exchanges data

---

## Roadmap

- Distributed metadata propagation
- Metadata synchronization across peers
- Persistent metadata storage (optional backends)
- Filtering and querying capabilities
- Extended capabilities system

---

## License

MIT

---

## Softadastra

Softadastra is a foundational system for building reliable software in unreliable environments.

Write locally. Persist first. Sync later.

