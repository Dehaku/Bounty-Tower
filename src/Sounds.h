#ifndef SOUNDS_H_INCLUDED
#define SOUNDS_H_INCLUDED

#include <SFML/Audio.hpp>
#include "globalvars.h"
#include <memory>
#include <unordered_map>

class SoundManager
{
public:
    std::unordered_map<std::string, sf::SoundBuffer> buffers;
    std::vector<std::unique_ptr<sf::Sound>> playSounds;
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
void setMusicVolume();
void playMusic(std::string track);


#endif // SOUNDS_H_INCLUDED
