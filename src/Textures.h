#ifndef __IMAGES_H_INCLUDED__
#define __IMAGES_H_INCLUDED__

#include <SFML/Graphics.hpp>

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
    sf::Texture &getTexture(std::string Input);
    void init();
};
extern TextureManager texturemanager;

#endif // IMAGES_H_INCLUDED
