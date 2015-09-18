#ifndef SKILLS_H_INCLUDED
#define SKILLS_H_INCLUDED

#include <string>
#include <vector>
#include <list>

class Skill
{
public:
    std::string name;
    std::string tree;
    std::string desc;
    bool autouse;
    bool active;
    bool on;
    int level;
    int durationint;
    int duration;
    int cooldownint;
    int cooldown;
    int ranks;
    int ranksmax;
    int cost;
};

class Skills
{
public:
    std::list<Skill> list;
    int getRanks(std::string skillName);
    Skill * getSkill(std::string skillName);
    Skills();
};

#endif // SKILLS_H_INCLUDED
