#ifndef __IMAGES_H_INCLUDED__
#define __IMAGES_H_INCLUDED__

#include <SFML/Graphics.hpp>

class cImageHolder
{
public:
    sf::Texture Image;
    std::string name;
};

class cImageManager
{
public:
    std::vector<cImageHolder> GlobalImage;
    sf::Texture *GetImage(std::string Input);
    sf::Texture GetvImage(std::string Input);
    void InitializeImages();
};
extern cImageManager imagemanager;

#endif // IMAGES_H_INCLUDED
