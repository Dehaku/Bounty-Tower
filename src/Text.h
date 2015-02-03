#ifndef __TEXT_H_INCLUDED__
#define __TEXT_H_INCLUDED__

#include <SFML/Graphics.hpp>

class TextList
{
public:
    int xpos;
    int ypos;
    sf::Color color;
    sf::Text sString;
    sf::Font font;
    int size;
    bool shadow;
    bool bold;

    void setup();
    void drawMe();
    std::vector<TextList> textlist;

    void createText(int xpos, int ypos, int SizeMe, sf::Color color,
                    std::string stringvalue1, std::string String1 = "",
                    float int1 = -6698, std::string stringvalue2 = "",
                    std::string String2 = "", float int2 = -6698,
                    std::string stringvalue3 = "", std::string String3 = "",
                    float int3 = -6698, int shadow = 1, int NULL3 = 0);

    void drawTextz();

    void loadFont();

    TextList();
};
extern TextList textList;

class ChatBox
{
public:
    class ChatLine
    {
    public:
        std::string line;
        sf::Color color;
    };

    std::vector<ChatLine> chatStorage;

    void addChat(std::string Text, sf::Color Color);

    void displayChat(sf::Vector2f Position);
};
extern ChatBox chatBox;

#endif // TEXT_H_INCLUDED
