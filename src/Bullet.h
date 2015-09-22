#ifndef BULLET_H_INCLUDED
#define BULLET_H_INCLUDED

#include "math.h"
#include "InputState.h"
#include "globalvars.h"
#include "Faction.h"

class Bullet
{
public:
    Vec3f pos;
    Vec3f velocity;
    std::vector<Vec3f> positions;
    double angle;
    int speed;
    int lifetime;
    int penetration;
    int health;
    int maxrichochet;
    float damage;
    bool toDelete;
    bool showPrediction;
    bool showPath;
    Npc * owner;
    Item * parent;
    npcPtrVector targets;
    npcPtrVector targetsHit;

    void moveBullet();

    Bullet();
};
extern std::vector<Bullet> bullets;

void predictBullet(Bullet bullet);

void removeBullets();

#endif // BULLET_H_INCLUDED
