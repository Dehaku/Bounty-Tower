#include "Textures.h"

#include "filesystemUtils.hpp"

TextureManager texturemanager;

sf::Texture *TextureManager::GetImage(std::string Input)
{
    for (size_t i = 0; i != textures.size(); i++)
    {
        if (textures.at(i).name == Input)
        {
            return &textures.at(i).texture;
        }
    }
    for (size_t i = 0; i != textures.size(); i++)
    {
        if (textures.at(i).name == "Error.bmp")
        {
            return &textures.at(i).texture;
        }
    }
    throw std::runtime_error("GetImage: Couldn't find image.");
}

void TextureManager::init()
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
            textures.push_back(image);
        }
    }
}
