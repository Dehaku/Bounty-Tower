#ifndef SAVELOAD_H_INCLUDED
#define SAVELOAD_H_INCLUDED

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include "filesystemUtils.hpp"
#include "util.h"
#include "Faction.h"


void saveGame(std::string profileName);
void loadGame(std::string profileName);

#endif // SAVELOAD_H_INCLUDED
