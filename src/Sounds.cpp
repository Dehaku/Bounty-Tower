#include "Sounds.h"

#include "filesystemUtils.hpp"

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

std::vector<MusicHolder*> musics;


/*

void MusicManager::playMusic(std::string input)
{
    for (size_t i = 0; i != musics.size(); i++)
    {
        if (musics.at(i).name == input)
        {
            musics[i].musictrack.setVolume(50);
            musics[i].musictrack.play();
            return;
        }
    }
    for (size_t i = 0; i != musics.size(); i++)
    {
        if (musics.at(i).name == "Error.wav")
        {
            musics[i].musictrack.setVolume(50);
            musics[i].musictrack.play();
            return;
        }
    }
}



void MusicManager::init()
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
            MusicHolder music;
            music.musictrack.openFromFile(line);

            std::string namebit = file;
            music.name = namebit;
            //musics.push_back(music);
        }
    }
}

*/

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

