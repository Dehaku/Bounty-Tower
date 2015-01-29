#ifndef __TEXT_H_INCLUDED__
#define __TEXT_H_INCLUDED__

#include <SFML/Graphics.hpp>

class cTextList
{
public:
    int xpos;
    int ypos;
    sf::Color Color;
    sf::Text sString;
    sf::Font font;
    int Size;
    bool Shadow;
    bool Bold;

    void Setup();
    void DrawMe();
    std::vector<cTextList> textlist;

    void CreateText(int xpos, int ypos, int SizeMe, sf::Color color,
                    std::string stringvalue1, std::string String1 = "",
                    float int1 = -6698, std::string stringvalue2 = "",
                    std::string String2 = "", float int2 = -6698,
                    std::string stringvalue3 = "", std::string String3 = "",
                    float int3 = -6698, int Shadow = 1, int NULL3 = 0);

    void DrawTextz();

    void LoadFont();

    cTextList();
};
extern cTextList cText;

class ChatBox
{
public:
    class ChatLine
    {
    public:
        std::string Line;
        sf::Color Color;
    };

    std::vector<ChatLine> ChatStorage;

    void AddChat(std::string Text, sf::Color Color);

    void DisplayChat(sf::Vector2f Position);
};
extern ChatBox ChatBox;

#endif // TEXT_H_INCLUDED
