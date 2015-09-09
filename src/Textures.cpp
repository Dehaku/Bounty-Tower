#include "Textures.h"

#include "filesystemUtils.hpp"

TextureManager texturemanager;

sf::Texture &TextureManager::getTexture(std::string input)
{
    for (size_t i = 0; i != textures.size(); i++)
    {
        if (textures.at(i).name == input)
        {
            return textures.at(i).texture;
        }
    }
    for (size_t i = 0; i != textures.size(); i++)
    {
        if (textures.at(i).name == "Error.bmp")
        {
            return textures.at(i).texture;
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



AnimationManager animationmanager;

Animation &AnimationManager::getAnimation(std::string input)
{
    for (size_t i = 0; i != animations.size(); i++)
    {
        if (animations.at(i).name == input)
        {
            return animations.at(i).animationImage;
        }
    }
    for (size_t i = 0; i != animations.size(); i++)
    {
        if (animations.at(i).name == "Error.bmp")
        {
            return animations.at(i).animationImage;
        }
    }
    throw std::runtime_error("GetAnimation: Couldn't find animation.");
}

void AnimationManager::init()
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
            int pos = file.find("SpriteList");
            if(pos != file.npos)
            {
                cout << file << endl;
                std::string outPut;
                std::string filter = "SpriteList";
                outPut.append(file,pos+filter.size(),file.size()-4);
                outPut.erase(outPut.size()-4);
                cout << outPut << endl;

                line.append(ending);

                AnimationHolder AniL;
                AniL.texture.loadFromFile(line);

                AniL.name = outPut;
                AniL.name.append("LeftWalk");
                AniL.animationImage.addFrame(sf::IntRect( 0, 48, 32, 48));
                AniL.animationImage.addFrame(sf::IntRect(96, 48, 32, 48));
                AniL.animationImage.addFrame(sf::IntRect( 0, 48, 32, 48));
                AniL.animationImage.addFrame(sf::IntRect(32, 48, 32, 48));

                animationmanager.animations.push_back(AniL);


                AnimationHolder AniR;
                AniR.texture.loadFromFile(line);

                AniR.name = outPut;
                AniR.name.append("RightWalk");
                AniR.animationImage.addFrame(sf::IntRect( 0, 48+48, 32, 48));
                AniR.animationImage.addFrame(sf::IntRect(96, 48+48, 32, 48));
                AniR.animationImage.addFrame(sf::IntRect( 0, 48+48, 32, 48));
                AniR.animationImage.addFrame(sf::IntRect(32, 48+48, 32, 48));

                animationmanager.animations.push_back(AniR);

                AnimationHolder AniU;
                AniU.texture.loadFromFile(line);

                AniU.name = outPut;
                AniU.name.append("UpWalk");
                AniU.animationImage.addFrame(sf::IntRect( 0, 48+48+48, 32, 48));
                AniU.animationImage.addFrame(sf::IntRect(96, 48+48+48, 32, 48));
                AniU.animationImage.addFrame(sf::IntRect( 0, 48+48+48, 32, 48));
                AniU.animationImage.addFrame(sf::IntRect(32, 48+48+48, 32, 48));

                animationmanager.animations.push_back(AniU);

                AnimationHolder AniD;
                AniD.texture.loadFromFile(line);

                AniD.name = outPut;
                AniD.name.append("DownWalk");
                AniD.animationImage.addFrame(sf::IntRect( 0, 0, 32, 48));
                AniD.animationImage.addFrame(sf::IntRect(96, 0, 32, 48));
                AniD.animationImage.addFrame(sf::IntRect( 0, 0, 32, 48));
                AniD.animationImage.addFrame(sf::IntRect(32, 0, 32, 48));

                animationmanager.animations.push_back(AniD);
            }
        }
    }

    for(auto &Ani : animationmanager.animations)
    {
        Ani.animationImage.setSpriteSheet(Ani.texture);

        Ani.animation.setAnimation(Ani.animationImage);
        Ani.animation.setOrigin(16,24);

    }

}


