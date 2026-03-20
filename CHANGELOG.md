# Changelog - softadastra/metadata

All notable changes to the **metadata module** are documented in this file.

The `metadata` module provides a **structured, queryable representation of system state**, derived from operations and used by the sync engine.

---

## [0.1.0] - Initial Metadata Layer

### Added

* Core metadata abstractions:

  * `FileEntry`
  * `DeviceState`
  * `SyncState`
* `MetadataStore` interface:

  * Read/write operations
  * Query capabilities
* SQLite-based implementation (`SqliteMetadataStore`)
* Basic file tracking:

  * Path
  * Size
  * Last modified
  * Version (basic)
* Initial device tracking:

  * Device ID
  * Last seen timestamp

### Design

* Strict separation from WAL (no log responsibilities)
* Designed as a projection of system state
* No dependency on sync logic

---

## [0.1.1] - Stability Improvements

### Improved

* More consistent handling of file updates
* Safer update operations in metadata store
* Better timestamp normalization

### Fixed

* Edge cases where file state could become inconsistent
* Incorrect handling of missing file entries

---

## [0.2.0] - State Accuracy Enhancements

### Added

* File hash tracking
* Sync status field:

  * synced
  * pending
  * out-of-date
* Version mapping improvements

### Improved

* Accuracy of file state representation
* Better alignment with WAL replay data

---

## [0.3.0] - Device & Sync Tracking

### Added

* Enhanced `DeviceState`:

  * Known sequence position
  * Connection state
* Improved `SyncState`:

  * Pending operations tracking
  * Applied operations tracking

### Improved

* Visibility into synchronization progress
* Better support for multi-device scenarios

---

## [0.4.0] - Query & Access Improvements

### Added

* Query helpers:

  * Get files by state
  * Get pending changes
  * Get outdated entries
* Indexed queries (basic)

### Improved

* Performance of common metadata queries
* Cleaner API for state retrieval

---

## [0.5.0] - Recovery Integration

### Added

* Integration hooks for WAL replay
* Metadata rebuild support
* Partial state recovery handling

### Improved

* Faster recovery after restart
* Better consistency after crash

---

## [0.6.0] - Storage Layer Improvements

### Added

* Transaction support in SQLite store
* Batched updates for performance
* Safer write operations

### Improved

* Reduced risk of inconsistent state during updates
* Better performance under load

---

## [0.7.0] - Extraction Ready

### Added

* Namespace stabilization (`softadastra::metadata`)
* Public API cleanup
* Documentation for all exposed structures

### Improved

* Decoupling from application-specific assumptions
* Prepared for reuse in:

  * Softadastra Sync OS
  * SDK
  * Distributed systems tools

---

## Roadmap

### Planned

* Advanced indexing strategies
* In-memory caching layer
* Snapshot support
* Conflict metadata tracking
* Multi-device optimization
* Pluggable storage backends

---

## Philosophy

Metadata is not the source of truth.

> It is a projection of truth derived from the WAL.

---

## Rules

* Never replace WAL
* Never assume correctness without replay
* Always remain queryable
* Always maintain consistency

---

## Summary

The `metadata` module ensures:

* Structured state representation
* Efficient querying
* Sync visibility

It is the **memory layer of Softadastra**.
