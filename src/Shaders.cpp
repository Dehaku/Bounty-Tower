#include "Shaders.h"
#include <iostream>

ShaderManager shadermanager;

void ShaderManager::shaderLoop()
{
    shockTime += 0.05;
    //shockPower -= 0.5;
    if(shockPower < 0)
        shockPower = 0;
    if(shockThickness < shockTime)
        shockThickness -= 0.005;
    if(shockThickness < 0)
        shockThickness = 0;
    //float time = fpsKeeper.startTime.getElapsedTime().asSeconds();
    float x = gvars::mousePos.x, y = gvars::mousePos.y;
    float radius = shockTime;
    if(radius < 0)
        radius = -radius;


    shockwaveShader.setParameter("in_Time", radius);
    //shockwaveShader.setParameter("in_Center", x*2, y*2);
    shockwaveShader.setParameter("in_ShockParams", shockPower, 0.8f, shockThickness);
    //10 instead of 20 for a weak blast, 30 looks like it's ripping tiles, any more and it's broken.
    // 0.05 instead of 0.15, is a much smaller wave, may have uses, but I like it big.
    shockwaveShader.setParameter("in_WindowWidth", RESOLUTION.y);
    shockwaveShader.setParameter("in_WindowHeight", RESOLUTION.y);

}

void ShaderManager::setShockwave(sf::Vector2f vPos)
{
    shockTime = 0;
    shockwaveShader.setParameter("in_Center", vPos.x*2, vPos.y*2);
    shockwaveShader.setParameter("in_ShockParams", 20.0f, 0.8f, 0.15f);
    shockPower = 20;
    shockThickness = 0.15;
}

void ShaderManager::setupShaders()
{
    if (shockwaveShader.loadFromFile("data/shaders/Shockwave.vert","data/shaders/Shockwave.frag"))
    {
        shockwaveShader.setParameter("in_ShockParams", 20.0f, 0.8f, 0.15f);
    }
    else
    {
        std::cout << "Failed to load Shockwave \n";
        return;
    }

    if (!lazorShader.loadFromFile("data/shaders/glow.frag",sf::Shader::Fragment))
        return;
}
