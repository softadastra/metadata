/*
 * Hostname.hpp
 */

#ifndef SOFTADASTRA_METADATA_HOSTNAME_HPP
#define SOFTADASTRA_METADATA_HOSTNAME_HPP

#include <string>

#if defined(_WIN32)
#include <winsock2.h>
#else
#include <unistd.h>
#endif

namespace softadastra::metadata::utils
{
  class Hostname
  {
  public:
    /**
     * @brief Return the local hostname when available
     */
    static std::string get()
    {
      char buffer[256] = {0};

#if defined(_WIN32)
      if (::gethostname(buffer, static_cast<int>(sizeof(buffer))) != 0)
      {
        return "unknown-host";
      }
#else
      if (::gethostname(buffer, sizeof(buffer)) != 0)
      {
        return "unknown-host";
      }
#endif

      if (buffer[0] == '\0')
      {
        return "unknown-host";
      }

      return std::string(buffer);
    }
  };

} // namespace softadastra::metadata::utils

#endif
