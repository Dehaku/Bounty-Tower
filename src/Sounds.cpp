#include "Sounds.h"

#include "filesystemUtils.hpp"

SoundManager soundmanager;

sf::SoundBuffer &SoundManager::getSound(std::string input)
{
    for (size_t i = 0; i != sounds.size(); i++)
    {
        if (sounds.at(i).name == input)
        {
            return sounds.at(i).soundstorage;
        }
    }
    for (size_t i = 0; i != sounds.size(); i++)
    {
        if (sounds.at(i).name == "Error.bmp")
        {
            return sounds.at(i).soundstorage;
        }
    }
    //throw std::runtime_error("GetImage: Couldn't find image.");
}

void SoundManager::playSound(std::string input)
{
    for (size_t i = 0; i != sounds.size(); i++)
    {
        if (sounds.at(i).name == input)
        {
            //return sounds.at(i).soundstorage;
            sounds[i].sound.setBuffer(sounds[i].soundstorage);
            sounds[i].sound.setVolume(50);
            sounds[i].sound.play();
            return;
        }
    }
    for (size_t i = 0; i != sounds.size(); i++)
    {
        if (sounds.at(i).name == "Error.wav")
        {
            //return sounds.at(i).soundstorage;
            sounds[i].sound.setBuffer(sounds[i].soundstorage);
            sounds[i].sound.setVolume(50);
            sounds[i].sound.play();
            return;
        }
    }
}

void SoundManager::init()
{
    using namespace std;
    string dir = string("./data/sfx");
    vector<string> files = vector<string>();
    getdir(dir, files);
    for (auto &file : files)
    {
        string line("data/sfx/");
        string ending(file);
        if (file != "." && file != "..")
        {
            line.append(ending);
            SoundHolder sound;
            sound.soundstorage.loadFromFile(line);
            //std::cout << Size.x << " : " << Size.y << std::endl;

            std::string namebit = file;
            sound.name = namebit;
            //image.sound.setSmooth(false);
            sounds.push_back(sound);
        }
    }
}

