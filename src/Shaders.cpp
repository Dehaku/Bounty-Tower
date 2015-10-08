#include "Shaders.h"

ShaderManager shadermanager;

void ShaderManager::shaderLoop()
{
    shockTime += 0.05;
    //float time = fpsKeeper.startTime.getElapsedTime().asSeconds();
    float x = gvars::mousePos.x, y = gvars::mousePos.y;
    float radius = shockTime;
    if(radius < 0)
        radius = -radius;


    shockwaveShader.setParameter("in_Time", radius);
    //shockwaveShader.setParameter("in_Center", x*2, y*2);
    shockwaveShader.setParameter("in_ShockParams", 20.0f, 0.8f, 0.15f);
    //10 instead of 20 for a weak blast, 30 looks like it's ripping tiles, any more and it's broken.
    // 0.05 instead of 0.15, is a much smaller wave, may have uses, but I like it big.
    shockwaveShader.setParameter("in_WindowWidth", RESOLUTION.y);
    shockwaveShader.setParameter("in_WindowHeight", RESOLUTION.y);

}

void ShaderManager::setShockwave(sf::Vector2f vPos)
{
    shockTime = 0;
    shockwaveShader.setParameter("in_Center", vPos.x*2, vPos.y*2);
}

void ShaderManager::setupShockwaveShader()
{
    if (!shockwaveShader.loadFromFile("data/shaders/Shockwave.vert","data/shaders/Shockwave.frag"))
        return;
}
