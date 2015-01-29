#include "filesystemUtils.hpp"

#ifdef GALAXY_LINUX
#include <sys/stat.h>
#else
#include <windows.h>
#include <direct.h>
#endif

void galaxy_mkdir(std::string const &name)
{
#ifdef GALAXY_LINUX
    mkdir(name.c_str(), 0755);
#else
    _mkdir(name.c_str());
#endif
}
