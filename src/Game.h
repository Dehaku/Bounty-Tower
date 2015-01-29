#ifndef __GAME_H_INCLUDED__
#define __GAME_H_INCLUDED__

#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <set>
#include <list>
#include <string>
#include <vector>
#include <dirent.h>
#include <memory.h>
#include <cmath>
#ifdef GALAXY_LINUX
#include <sys/stat.h>
#else
#include <windows.h>
#include <direct.h>
#endif
#include <time.h>

using std::abs;

/// Get the absolute of a numeric value,
/// converted to size_t, suitable for indexing
template <typename T> size_t abs_to_index(T value)
{
    return static_cast<size_t>(std::abs(value));
}

inline void galaxy_mkdir(std::string const &name)
{
#ifdef GALAXY_LINUX
    mkdir(name.c_str(), 0755);
#else
    _mkdir(name.c_str());
#endif
}

#endif // GAME_H_INCLUDED
