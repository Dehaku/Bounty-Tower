#ifndef SOUNDS_H_INCLUDED
#define SOUNDS_H_INCLUDED

#include <SFML/Audio.hpp>
#include "globalvars.h"

class SoundHolder
{
public:
    sf::SoundBuffer soundstorage;
    std::string name;
};

class SoundPlayer
{
    public:
    sf::Sound sound;
    std::string name;
    bool toDelete;
    SoundPlayer();
};

class SoundManager
{
public:
    std::vector<SoundHolder> sounds;
    std::vector<SoundPlayer> playSounds;
    sf::SoundBuffer &getSound(std::string input);
    void cleanSounds();
    void playSound(std::string input);
    void init();
};
extern SoundManager soundmanager;


struct MusicHolder
{
    sf::Music musictrack;
    std::string name;
};

extern std::vector<MusicHolder*> musics;

void initializeMusic();
void playMusic(std::string track);


#endif // SOUNDS_H_INCLUDED