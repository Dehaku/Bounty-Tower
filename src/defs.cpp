#include "defs.h"

ResolutionManager resolution;

int ResolutionManager::X()
{
    return window.getSize().x;
}


int ResolutionManager::Y()
{
    return window.getSize().y;
}


ResolutionManager::ResolutionManager()
{
    x = 1280;
    y = 720;
    newX = x;
    newY = y;
    currentRes = 0;
    fullscreen = false;
    resolutions = sf::VideoMode::getFullscreenModes();
}
