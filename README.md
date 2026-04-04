# softadastra/metadata

> Structured state layer for local-first systems.

The `metadata` module is the **memory of Softadastra**.

It maintains a **structured, queryable view of the system state**, independent from:

* The filesystem (fs)
* The operation log (wal)
* The network (transport)

---

## Purpose

The goal of `softadastra/metadata` is simple:

> Maintain the current known state of files, devices, and synchronization.

---

## Core Principle

> WAL stores what happened. Metadata stores what is known.

* WAL = history
* Metadata = current state

---

## Responsibilities

The `metadata` module provides:

* File state tracking
* Device state tracking
* Sync state tracking
* Version mapping
* Queryable state access

---

## What this module does NOT do

* No filesystem observation (fs module)
* No durability log (wal module)
* No network communication (transport module)
* No sync decision logic (sync module)

👉 It stores state, nothing more.

---

## Design Principles

### 1. Derived state

Metadata is derived from:

* WAL replay
* Sync operations

---

### 2. Consistency

The state must always be:

* Internally consistent
* Queryable
* Recoverable

---

### 3. Separation from WAL

* WAL = append-only log
* Metadata = mutable structured state

⚠️ These must never be merged.

---

### 4. Query-first

Metadata exists to answer:

* What files do we have?
* What version is this file?
* What is synced or pending?

---

## Core Components

### FileEntry

Represents a file in the system.

Contains:

* Path
* Hash
* Version
* Size
* Last modified
* Sync status

---

### DeviceState

Represents a peer/device.

Contains:

* Device ID
* Last seen
* Known sequence
* Connection state

---

### SyncState

Tracks synchronization status.

Examples:

* Synced
* Pending
* Out-of-date

---

### MetadataStore

Abstract interface.

Provides:

* Read/write operations
* Queries
* State updates

---

### SqliteMetadataStore

Concrete implementation using SQLite.

Provides:

* Persistent state
* Query capabilities
* Transaction safety

---

## Example Usage

```cpp id="ex6"
#include <softadastra/metadata/SqliteMetadataStore.hpp>

using namespace softadastra::metadata;

SqliteMetadataStore store("data/metadata.db");

auto file = store.getFile("/docs/file.txt");

if (!file)
{
    // create entry
}
```

---

## Integration

Used by:

* sync (primary)
* wal (indirect via replay)
* app layer

---

## Data Model

### File state

Tracks:

* Current version
* Content hash
* Sync status

---

### Device state

Tracks:

* Known peers
* Last sync point
* Availability

---

### Sync state

Tracks:

* Pending operations
* Applied operations
* Missing operations

---

## Recovery Model

1. WAL is replayed
2. Metadata is rebuilt or updated
3. Sync resumes from consistent state

---

## Dependencies

### Internal

* softadastra/core

### External

* SQLite (initial implementation)

---

## MVP Scope

* Single device state tracking
* Basic file entries
* Simple sync status
* No advanced indexing

---

## Roadmap

* Indexed queries (performance)
* Multi-device state optimization
* Conflict metadata
* Snapshot support
* In-memory cache layer
* Alternative storage backends

---

## Rules

* Never act as a source of truth
* Never replace WAL
* Always reflect current known state
* Always remain queryable

---

## Philosophy

Metadata is not truth.

> It is a projection of truth.

---

## Summary

* Stores structured state
* Enables queries
* Supports sync decisions
* Complements WAL

---

## License

See root LICENSE file.
