#ifndef SHADERS_H_INCLUDED
#define SHADERS_H_INCLUDED

#include <SFML/Graphics.hpp>
#include "globalvars.h"

class ShaderManager
{
public:
    sf::Shader shockwaveShader;
    sf::Shader lazorShader;

    float shockTime;
    float shockPower;
    float shockPowerDropRate;
    float shockThickness;
    float shockThicknessDropRate;

    void shaderLoop();

    void setupShaders();

    void shadermanager();

    void setShockwave(sf::Vector2f vPos);


};
extern ShaderManager shadermanager;





#endif // SHADERS_H_INCLUDED
