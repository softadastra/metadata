# softadastra/metadata

> Node metadata primitives for reliable Softadastra products.

`softadastra/metadata` provides the node description layer of the Softadastra C++ stack.

Softadastra builds reliability-first products for local-first, offline-first, and distributed applications. This module describes what a node is, what it runs, and what capabilities it supports.

## Purpose

`softadastra/metadata` exists to describe Softadastra nodes in a clear and inspectable way.

It is used by higher-level SDKs, product APIs, discovery flows, and distributed Softadastra infrastructure.

The core rule is simple:

> Metadata describes nodes. Discovery finds nodes. Transport connects nodes.

It is designed to be:

- Local-first
- Descriptive
- Capability-oriented
- Observable
- Serializable
- Product-ready

## What it provides

This module provides metadata primitives such as:

- Node identity
- Runtime information
- Platform information
- Version information
- Node capabilities
- Metadata snapshots
- Metadata encoding and decoding
- Metadata registry
- Metadata engine
- High-level metadata service

## What it does not do

`softadastra/metadata` does not contain:

- WAL persistence
- Store mutation
- Sync propagation
- Transport delivery
- Peer discovery
- Conflict resolution
- Encryption
- Product-specific logic

It describes nodes and their capabilities. Other modules decide how nodes are discovered, connected, synchronized, or trusted.

## Core model

```txt
Node identity
      |
Runtime information
      |
Capabilities
      |
Node metadata
      |
Registry / service
```

Metadata keeps node description separate from discovery, transport, sync, storage, and durability.

## Where it fits

```txt
Softadastra products
        |
SDKs and product APIs
        |
softadastra/metadata
        |
softadastra/discovery
        |
softadastra/transport
        |
softadastra/sync
        |
softadastra/store
        |
softadastra/wal
        |
softadastra/core
```

`softadastra/metadata` works with discovery and transport to help higher-level systems understand what each node is and what it can do.

## Installation

```bash
vix add @softadastra/metadata
```

## Usage

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

## Example

```cpp
#include <softadastra/metadata/Metadata.hpp>

#include <iostream>

int main()
{
    auto options = softadastra::metadata::MetadataOptions::local(
        "node-a",
        "1.0.0"
    );

    if (!options.is_valid())
    {
        std::cout << "invalid metadata options\n";
        return 1;
    }

    auto metadata = softadastra::metadata::core::NodeMetadata::foundation(
        "node-a",
        softadastra::metadata::utils::Hostname::get(),
        softadastra::metadata::utils::PlatformInfo::os_name(),
        "1.0.0"
    );

    if (!metadata.is_valid())
    {
        std::cout << "invalid node metadata\n";
        return 1;
    }

    std::cout << "Node: " << metadata.node_id() << "\n";
    std::cout << "OS: " << metadata.runtime.os_name << "\n";
    std::cout << "Capabilities: " << metadata.capabilities.size() << "\n";

    return 0;
}
```

## Requirements

- C++20
- `softadastra/core`
- `softadastra/discovery`

## Documentation

For the full documentation, visit [docs.softadastra.com](https://docs.softadastra.com).

## License

Licensed under the Apache License, Version 2.0.
