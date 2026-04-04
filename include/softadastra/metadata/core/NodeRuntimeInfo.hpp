/*
 * NodeRuntimeInfo.hpp
 */

#ifndef SOFTADASTRA_METADATA_NODE_RUNTIME_INFO_HPP
#define SOFTADASTRA_METADATA_NODE_RUNTIME_INFO_HPP

#include <cstdint>
#include <string>

namespace softadastra::metadata::core
{
  /**
   * @brief Runtime information describing the local node
   */
  struct NodeRuntimeInfo
  {
    /**
     * Local hostname
     */
    std::string hostname;

    /**
     * Operating system name
     */
    std::string os_name;

    /**
     * Runtime or product version string
     */
    std::string version;

    /**
     * Process or node start timestamp in milliseconds
     */
    std::uint64_t started_at{0};

    /**
     * Current uptime in milliseconds
     */
    std::uint64_t uptime_ms{0};

    /**
     * @brief Check whether this runtime info is minimally usable
     */
    bool valid() const noexcept
    {
      return !hostname.empty() &&
             !os_name.empty() &&
             !version.empty();
    }
  };

} // namespace softadastra::metadata::core

#endif
