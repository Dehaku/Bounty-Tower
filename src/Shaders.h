#ifndef SHADERS_H_INCLUDED
#define SHADERS_H_INCLUDED

#include <SFML/Graphics.hpp>
#include "globalvars.h"

class ShaderManager
{
public:
    sf::Shader shockwaveShader;

    float shockTime;

    void shaderLoop();

    void setupShockwaveShader();

    shadermanager();

    void setShockwave(sf::Vector2f vPos);


};
extern ShaderManager shadermanager;





#endif // SHADERS_H_INCLUDED
