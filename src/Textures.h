#ifndef __IMAGES_H_INCLUDED__
#define __IMAGES_H_INCLUDED__

#include <iostream>

#include <SFML/Graphics.hpp>
#include "AnimatedSprite.hpp"

class TextureHolder
{
public:
    sf::Texture texture;
    std::string name;
};

class TextureManager
{
public:
    std::vector<TextureHolder> textures;
    sf::Texture &getTexture(std::string input);
    void init();
};
extern TextureManager texturemanager;

class AnimationHolder
{
public:
    std::string texture;
    Animation animationImage;
    AnimatedSprite animation;
    std::string name;
};

class AnimationManager
{
public:
    std::vector<AnimationHolder> animations;
    Animation &getAnimation(std::string input);
    void init();
};
extern AnimationManager animationmanager;

#endif // IMAGES_H_INCLUDED
