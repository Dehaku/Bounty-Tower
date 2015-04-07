#include "Sounds.h"

#include "filesystemUtils.hpp"
#include <iostream>

SoundManager soundmanager;
//MusicManager musicmanager;

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
            SoundPlayer SP;
            SP.sound.setBuffer(sounds[i].soundstorage);
            SP.sound.setVolume(gvars::soundVolume);
            //SP.sound.play();
            playSounds.push_back(SP);
            playSounds.back().sound.play();
            return;
        }
    }
    for (size_t i = 0; i != sounds.size(); i++)
    {
        if (sounds.at(i).name == "Error.wav")
        {
            //return sounds.at(i).soundstorage;
            SoundPlayer SP;
            SP.sound.setBuffer(sounds[i].soundstorage);
            SP.sound.setVolume(gvars::soundVolume);
            playSounds.push_back(SP);
            playSounds.back().sound.play();
            return;
        }
    }
}

SoundPlayer::SoundPlayer()
{
    toDelete = false;
}

void checkSounds(std::vector<SoundPlayer> &playsound)
{
    for (auto &i : playsound)
    {
        SoundPlayer SP;
        //SP.sound.getStatus
        //std::cout << "Sound Status: " << i.sound.getStatus() << ", VS: " << sf::Sound::Status::Stopped << std::endl;
        if(i.sound.getStatus() == sf::Sound::Status::Stopped)
        {
            i.toDelete = true;
        }
    }
}

void SoundManager::cleanSounds()
{
    checkSounds(playSounds);
    bool done = false;
    while (done == false)
    {
        bool yet = false;
        for (auto it = playSounds.begin(); it != playSounds.end(); ++it)
        {
            if (it->toDelete)
            {
                //std::cout << it->name << " to be deleted. \n";
                playSounds.erase(it);
                yet = true;
                break;
            }
        }
        if (yet == false)
        {
            done = true;
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

            std::string namebit = file;
            sound.name = namebit;
            sounds.push_back(sound);
        }
    }
}

std::vector<MusicHolder*> musics;

void initializeMusic()
{
    using namespace std;
    string dir = string("./data/music");
    vector<string> files = vector<string>();
    getdir(dir, files);
    for (auto &file : files)
    {
        string line("data/music/");
        string ending(file);
        if (file != "." && file != "..")
        {
            line.append(ending);
            MusicHolder * music;
            music = new MusicHolder;
            music->musictrack.openFromFile(line);

            std::string namebit = file;
            music->name = namebit;
            musics.push_back(music);
        }
    }
}

void playMusic(std::string track)
{
    for (size_t i = 0; i != musics.size(); i++)
    {
        if (musics[i]->name == track)
        {
            musics[i]->musictrack.setVolume(gvars::musicVolume);
            musics[i]->musictrack.play();
            return;
        }
    }
}

