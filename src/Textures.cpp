#include "Textures.h"

#include "filesystemUtils.hpp"

TextureManager texturemanager;

sf::Texture *TextureManager::GetImage(std::string Input)
{
    for (size_t i = 0; i != GlobalImage.size(); i++)
    {
        if (GlobalImage.at(i).name == Input)
        {
            return &GlobalImage.at(i).texture;
        }
    }
    for (size_t i = 0; i != GlobalImage.size(); i++)
    {
        if (GlobalImage.at(i).name == "Error.bmp")
        {
            return &GlobalImage.at(i).texture;
        }
    }
    throw std::runtime_error("GetImage: Couldn't find image.");
}

void TextureManager::InitializeImages()
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
            TextureHolder image;
            image.texture.loadFromFile(line);
            //std::cout << Size.x << " : " << Size.y << std::endl;

            std::string namebit = file;
            image.name = namebit;
            image.texture.setSmooth(false);
            GlobalImage.push_back(image);
        }
    }
}
