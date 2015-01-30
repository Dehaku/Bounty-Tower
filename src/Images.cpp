#include "Images.h"

#include "filesystemUtils.hpp"

cImageManager imagemanager;

sf::Texture *cImageManager::GetImage(std::string Input)
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

sf::Texture cImageManager::GetvImage(std::string Input)
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

void cImageManager::InitializeImages()
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
