/**
 *
 *  @file Hostname.hpp
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

#ifndef SOFTADASTRA_METADATA_HOSTNAME_HPP
#define SOFTADASTRA_METADATA_HOSTNAME_HPP

#include <array>
#include <string>

#if defined(_WIN32)
#include <winsock2.h>
#else
#include <unistd.h>
#endif

namespace softadastra::metadata::utils
{

  /**
   * @brief Utility for reading the local hostname.
   *
   * Hostname provides a small cross-platform helper around gethostname().
   */
  class Hostname
  {
  public:
    /**
     * @brief Fallback hostname used when the system hostname is unavailable.
     */
    static constexpr const char *fallback = "unknown-host";

    /**
     * @brief Returns the local hostname when available.
     *
     * @return Local hostname, or fallback when unavailable.
     */
    [[nodiscard]] static std::string get()
    {
      std::array<char, 256> buffer{};

#if defined(_WIN32)
      const int result =
          ::gethostname(
              buffer.data(),
              static_cast<int>(buffer.size()));
#else
      const int result =
          ::gethostname(
              buffer.data(),
              buffer.size());
#endif

      if (result != 0 || buffer[0] == '\0')
      {
        return fallback;
      }

      buffer.back() = '\0';

      return std::string{buffer.data()};
    }

    /**
     * @brief Returns true if the hostname is usable.
     *
     * @param value Hostname value.
     * @return true when non-empty and not equal to fallback.
     */
    [[nodiscard]] static bool is_valid(
        const std::string &value) noexcept
    {
      return !value.empty() &&
             value != fallback;
    }
  };

} // namespace softadastra::metadata::utils

#endif // SOFTADASTRA_METADATA_HOSTNAME_HPP
