#include "Sounds.h"

#include "filesystemUtils.hpp"
#include <iostream>

SoundManager soundmanager;
//MusicManager musicmanager;

void SoundManager::playSound(std::string input)
{
    auto play = [this](sf::SoundBuffer const & buf) {
        auto sound = std::make_unique<sf::Sound>(buf);
        sound->setVolume(gvars::soundVolume);
        playSounds.push_back(std::move(sound));
        playSounds.back()->play();
    };
    auto it = buffers.find(input);
    if (it != buffers.end()) {
        play(it->second);
    } else {
        play(buffers["Error.wav"]);
    }
}

void SoundManager::cleanSounds()
{
    auto it = std::remove_if(playSounds.begin(), playSounds.end(), [](auto const & s) {
        return s->getStatus() == sf::Sound::Stopped;
    });
    playSounds.erase(it, playSounds.end());
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
            sf::SoundBuffer buf;
            buf.loadFromFile(line);
            buffers[file] = buf;
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

void setMusicVolume()
{
    for (size_t i = 0; i != musics.size(); i++)
        musics[i]->musictrack.setVolume(gvars::musicVolume);
}

void playMusic(std::string track)
{
    for (size_t i = 0; i != musics.size(); i++)
        musics[i]->musictrack.stop();

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

