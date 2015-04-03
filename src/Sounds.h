#ifndef SOUNDS_H_INCLUDED
#define SOUNDS_H_INCLUDED

#include <SFML/Audio.hpp>

class SoundHolder
{
public:
    sf::SoundBuffer soundstorage;
    sf::Sound sound;
    std::string name;
};



class SoundManager
{
public:
    std::vector<SoundHolder> sounds;
    sf::SoundBuffer &getSound(std::string input);
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


#endif // SOUNDS_H_INCLUDED
