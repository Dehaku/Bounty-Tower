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
    // http://en.sfml-dev.org/forums/index.php?topic=10281.0

    int xDim = 60;
    int yDim = 60;


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
            int pos = file.find("SpriteSheet");
            if(pos != file.npos)
            {
                cout << file << endl;
                std::string outPut;
                std::string filter = "SpriteSheet";
                outPut.append(file,pos+filter.size(),file.size()-4);
                outPut.erase(outPut.size()-4);
                cout << outPut << endl;

                line.append(ending);

                AnimationHolder AniD;
                AniD.texture.loadFromFile(line);
                //AniD.texture = texturemanager.getTexture(file);

                AniD.name = outPut;
                AniD.name.append("DownWalk");
                AniD.animationImage.addFrame(sf::IntRect( 0, 0, 60, 60));
                AniD.animationImage.addFrame(sf::IntRect(60, 0, 60, 60));
                AniD.animationImage.addFrame(sf::IntRect( 0, 0, 60, 60));
                AniD.animationImage.addFrame(sf::IntRect(120, 0, 60, 60));
                animationmanager.animations.push_back(AniD);

                AnimationHolder AniR;
                AniR.texture.loadFromFile(line);

                AniR.name = outPut;
                AniR.name.append("RightWalk");
                AniR.animationImage.addFrame(sf::IntRect( 0, 60, 60, 60));
                AniR.animationImage.addFrame(sf::IntRect(60, 60, 60, 60));
                AniR.animationImage.addFrame(sf::IntRect( 0, 60, 60, 60));
                AniR.animationImage.addFrame(sf::IntRect(120, 60, 60, 60));
                animationmanager.animations.push_back(AniR);

                AnimationHolder AniU;
                AniU.texture.loadFromFile(line);

                AniU.name = outPut;
                AniU.name.append("UpWalk");
                AniU.animationImage.addFrame(sf::IntRect( 0, 120, 60, 60));
                AniU.animationImage.addFrame(sf::IntRect(60, 120, 60, 60));
                AniU.animationImage.addFrame(sf::IntRect( 0, 120, 60, 60));
                AniU.animationImage.addFrame(sf::IntRect(120, 120, 60, 60));
                animationmanager.animations.push_back(AniU);


                AnimationHolder AniL;
                AniL.texture.loadFromFile(line);

                AniL.name = outPut;
                AniL.name.append("LeftWalk");
                AniL.animationImage.addFrame(sf::IntRect( 0, 60, 60, 60));
                AniL.animationImage.addFrame(sf::IntRect(60, 60, 60, 60));
                AniL.animationImage.addFrame(sf::IntRect( 0, 60, 60, 60));
                AniL.animationImage.addFrame(sf::IntRect(120, 60, 60, 60));
                animationmanager.animations.push_back(AniL);
                //AnimatedSprite.

            }
        }
    }

    for(auto &Ani : animationmanager.animations)
    {
        Ani.animationImage.setSpriteSheet(Ani.texture);
        //Ani.animationImage.setSpriteSheet(texturemanager.getTexture("SpriteSheetBTHuman.png"));

        if(Ani.name.find("LeftWalk") != Ani.name.npos)
            Ani.animation.scale(-1,1);

        if(Ani.name.find("Noirves") != Ani.name.npos)
            Ani.animation.scale(0.75,0.75);



        Ani.animation.setAnimation(Ani.animationImage);
        Ani.animation.setOrigin(30,30);

    }

}


