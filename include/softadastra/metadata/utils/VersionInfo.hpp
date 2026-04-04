/*
 * VersionInfo.hpp
 */

#ifndef SOFTADASTRA_METADATA_VERSION_INFO_HPP
#define SOFTADASTRA_METADATA_VERSION_INFO_HPP

#include <string>

namespace softadastra::metadata::utils
{
  class VersionInfo
  {
  public:
    /**
     * @brief Return the module or runtime version string
     */
    static std::string current()
    {
      return "0.1.0";
    }
  };

} // namespace softadastra::metadata::utils

#endif
