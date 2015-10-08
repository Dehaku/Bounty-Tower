#include "Shaders.h"

sf::Shader m_shader;

void shaderStuff(float time)
{
    //float time = fpsKeeper.startTime.getElapsedTime().asSeconds();
    float x = gvars::mousePos.x, y = gvars::mousePos.y;
    float radius = std::cos(time);
    if(radius < 0)
        radius = -radius;


    m_shader.setParameter("in_Time", radius);
    m_shader.setParameter("in_Center", x*2, y*2);
    m_shader.setParameter("in_ShockParams", 20.0f, 0.8f, 0.15f);
    //10 instead of 20 for a weak blast, 30 looks like it's ripping tiles, any more and it's broken.
    // 0.05 instead of 0.15, is a much smaller wave, may have uses, but I like it big.
    m_shader.setParameter("in_WindowWidth", RESOLUTION.y);
    m_shader.setParameter("in_WindowHeight", RESOLUTION.y);

}
