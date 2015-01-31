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
    std::vector<TextureHolder> GlobalImage;
    sf::Texture *GetImage(std::string Input);
    sf::Texture GetvImage(std::string Input);
    void InitializeImages();
};
extern TextureManager texturemanager;

#endif // IMAGES_H_INCLUDED
