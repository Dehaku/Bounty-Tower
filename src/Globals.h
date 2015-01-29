#ifndef __GLOBALS_H_INCLUDED__
#define __GLOBALS_H_INCLUDED__

#include <SFML/Graphics.hpp>
#include <set>

const float PI = 3.141;
const double G = 6.67;
const sf::Vector2f Rez(1024, 768);
const sf::Vector2f Center(500, 500);
const int gridx = 100;
const int gridy = 100;
const int gridz = 10;
const int GridSize = 20;
const sf::Color White(255, 255, 255, 255);
const sf::Color Red(255, 0, 0, 255);
const sf::Color Green(0, 255, 0, 255);
const sf::Color Blue(0, 0, 255, 255);
const sf::Color Cyan(0, 150, 255, 255);
const sf::Color Yellow(255, 255, 0, 255);
const sf::Color Black(0, 0, 0, 255);
const sf::Color Brown(100, 100, 0, 255);

extern bool Debug;
extern bool InFocus;
extern bool DrawStuffsDone;

extern std::vector<std::string> chatboxlist;

class NPC;
extern std::vector<NPC> *Selection;

extern sf::Vector2f RMBHeldPos;

extern sf::RenderTexture renderTexture;
extern sf::Image Img;
extern sf::Texture Tex;

extern sf::Sprite Sprt;
extern sf::Sprite Sprt2;

class ID
{
public:
    enum WorldTile
    {
        City,
        Spire,
        Grass,
        Road,
        River,
        Ocean
    };
};
extern ID ID;

extern sf::Vector2f HalfSize;
extern sf::View View1;

extern int AirPressure;

extern float GX;
extern float GY;

extern sf::Vector2f MousePos;
extern int MouseStagnation;

extern sf::Sprite NewTile;

extern bool AllowedToAttack; // Set this to each NPC

extern std::vector<int> Selected;

extern float testx;
extern float testy;

extern float testx2;
extern float testy2;

extern float GCtimescale;

extern int MyTarget;
extern int MyTargetid;

extern float CameraZoom;

float CZ(float Number);

extern int currentx;
extern int currenty;
extern int currentz;

extern int currentregionx;
extern int currentregiony;
extern int currentplanet;

extern std::string Zom;
extern std::string Poi;

extern int groundmap[gridz][gridy][gridx];
extern int smellmap[gridz][gridy][gridx];
extern int sunmap[gridz][gridy][gridx];

enum class skills
{
    ag,
    ch,
    in,
    de,
    st,
    pe,
    en,
    wi
};

void fSleep(float Time);
void debug(std::string Info);
void PrintToFile(std::string FileName, std::string Text, bool EndLine = true);
void Con(std::string Text, bool EndLine = true);
void Con(float Number, bool EndLine = true);
bool Booleanize(int Num);
bool Inbetween(float First, float Second, float Number);
int randz(int minValue, int maxValue);
bool AABB(int Pointx, int Pointy, int Left, int Right, int Up, int Down);
bool AABB(sf::Vector2f Point, int Left, int Right, int Up, int Down);
int CloseishS(int orix, int tarx);

class Math
{
public:
    int Closeish(int orix, int oriy, int tarx, int tary);
    int Closeisha(int orix, int oriy, int tarx, int tary);
    int DistanceTrace(int xa, int ya, int xb, int yb);
    int Closeishxx(int orix, int oriy, int tarx, int tary);
    bool Exceed(float Value, float Limit);
    int Clamp(int Value, int MinValue, int MaxValue);
    float AngleBetweenVectors(sf::Vector2f a, sf::Vector2f b);
    sf::Vector2f CircleRandz(int xpos, int ypos, int radius);
    sf::Vector2f Vec(float X, float Y);
};
extern Math Math;

/// Get the absolute of a numeric value,
/// converted to size_t, suitable for indexing
template <typename T> size_t abs_to_index(T value)
{
    return static_cast<size_t>(std::abs(value));
}

int StringFindNumber(std::string Stringy, std::string Term);
std::string StringFindString(std::string Stringy, std::string Term);
std::string StringFindStringNpos(std::string Stringy, std::string Term);
std::vector<std::string> StringFindElements(std::string Source,
                                            std::string Seperater = ":");
std::vector<float> NumericSplitter(float Received, float SplitVariable = 100);
float PercentageBuff(float Received);
std::string StringFindChaos(std::string Stringy, std::string Term,
                            std::string Ending);
std::set<std::string> StringFindSetChaos(std::string Stringy, std::string Term,
                                         std::string Ending);
std::string AddString(std::string Original, std::string Addition);
std::string AddString(std::string Original, std::string Addition,
                      std::string Addition2);
std::string AddString(std::string Original, std::string Addition,
                      std::string Addition2, std::string Addition3);
std::string NumbertoString(float Number);
float PercentIs(float Value, float Percentage);

class Globes
{
public:
    bool Following;
    bool ButtonClicked;
    int ButtonClickedTime;
    sf::Vector2f HeldClickPos;
    int glbbtn;
    float Scalex;
    float Scaley;
    unsigned long long globalid;
    sf::Vector2f TopLeft;
    sf::Vector2f TopRight;
    sf::Vector2f BottomLeft;
    sf::Vector2f BottomRight;
    std::string GenString;
    sf::Vector2i Cords;
    bool InitalZeds;
    float TimeScale;

    Globes();
};
extern Globes Globals;

std::string GenerateName(int MinLength = 2, int MaxLength = 10);
bool Toggle(bool &Boolean);

#endif // GLOBALS_H_INCLUDED
