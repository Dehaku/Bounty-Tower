#ifndef __IMAGES_H_INCLUDED__
#define __IMAGES_H_INCLUDED__

#include "Game.h"

#include <SFML/Graphics.hpp>

extern int getdir(std::string dir, std::vector<std::string> &files);

class cImageHolder
{
public:
    sf::Texture Image;
    std::string name;
    cImageHolder()
    {
    }
};

class cImageManager
{
public:
    std::vector<cImageHolder> GlobalImage;

    sf::Texture *GetImage(std::string Input)
    {
        for (size_t i = 0; i != GlobalImage.size(); i++)
        {
            if (GlobalImage.at(i).name == Input)
            {
                return &GlobalImage.at(i).Image;
            }
        }
        for (size_t i = 0; i != GlobalImage.size(); i++)
        {
            if (GlobalImage.at(i).name == "Error.bmp")
            {
                return &GlobalImage.at(i).Image;
            }
        }
        throw std::runtime_error("GetImage: Couldn't find image.");
    }

    sf::Texture GetvImage(std::string Input)
    {
        for (size_t i = 0; i != GlobalImage.size(); i++)
        {
            if (GlobalImage.at(i).name == Input)
            {
                return GlobalImage.at(i).Image;
            }
        }
        for (size_t i = 0; i != GlobalImage.size(); i++)
        {
            if (GlobalImage.at(i).name == "Error.bmp")
            {
                return GlobalImage.at(i).Image;
            }
        }
        throw std::runtime_error("GetvImage: Couldn't find image.");
    }

    void InitializeImages()
    {
        using namespace std;
        string dir = string("./data/gfx");
        vector<string> files = vector<string>();
        getdir(dir, files);
        for (auto &file : files)
        {
            string line("data/gfx/");
            string ending(file);
            if (file != "." && file != "..")
            {
                line.append(ending);
                cImageHolder image;
                image.Image.loadFromFile(line);
                //std::cout << Size.x << " : " << Size.y << std::endl;

                std::string namebit = file;
                image.name = namebit;
                image.Image.setSmooth(false);
                GlobalImage.push_back(image);
            }
        }
    }
    cImageManager()
    {
    }
};
cImageManager imagemanager;

#endif // IMAGES_H_INCLUDED
