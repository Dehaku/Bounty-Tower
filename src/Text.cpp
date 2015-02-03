#include "Text.h"

#include <sstream>

class ChatBox chatBox;
TextList textList;

extern sf::RenderWindow window;

void TextList::setup()
{
    sString.setColor(color);
    sString.setCharacterSize(size);

    if (bold == true)
    {
        sString.setStyle(sf::Text::Bold);
    }
    sString.setPosition(xpos, ypos);
}

void TextList::drawMe()
{
    if (shadow == true)
    {

        sString.setPosition(xpos - 1, ypos - 1);
        sString.setColor(sf::Color(0, 0, 0));
        window.draw(sString);
    }
    setup();
    window.draw(sString);
}

void TextList::createText(int xpos, int ypos, int SizeMe, sf::Color color,
                          std::string stringvalue1, std::string String1,
                          float int1, std::string stringvalue2,
                          std::string String2, float int2,
                          std::string stringvalue3, std::string String3,
                          float int3, int Shadow, int /*NULL3*/)
{
    int Size = SizeMe;
    bool Bold = true;

    using namespace std;
    string line1("");
    stringstream convert1;
    stringstream convert2;
    stringstream convert3;

    line1.append(stringvalue1);

    line1.append(String1);

    if (int1 != -6698)
    {
        convert1 << int1;
        line1.append(convert1.str());
    }

    line1.append(stringvalue2);

    line1.append(String2);

    if (int2 != -6698)
    {
        convert2 << int2;
        line1.append(convert2.str());
    }

    line1.append(stringvalue3);

    line1.append(String3);

    if (int3 != -6698)
    {
        convert3 << int3;
        line1.append(convert3.str());
    }

    sf::Text Textz(line1, font);

    Textz.setCharacterSize(Size);
    if (Bold == true)
    {
        Textz.setStyle(sf::Text::Bold);
    }

    //if(shadow == true){Textz.SetText(line1);Textz.SetColor(sf::Color(0,0,0));Textz.SetPosition(xpos+1,ypos+1);App.Draw(Textz);}
    Textz.setColor(color);
    Textz.setPosition(xpos, ypos); //App.Draw(Textz);
    TextList var;
    var.xpos = xpos;
    var.ypos = ypos;
    var.color = color;
    var.sString = Textz;
    var.size = Size;
    var.shadow = Shadow;
    var.bold = Bold;
    textlist.push_back(var);
}

void TextList::drawTextz()
{

    std::vector<TextList>::iterator textz;
    for (textz = textlist.begin(); textz != textlist.end(); ++textz)
    {
        //if(textz->Shadow == true){textz->SetColor(textz->Color);Textz.SetPosition(textz->xpos+1,textz->ypos+1);App.Draw(Textz);}Textz.SetPosition(textz->xpos,textz->ypos);
        textz->drawMe();
    }

    textlist.clear();
}

void TextList::loadFont()
{
    if (!font.loadFromFile("data/fonts/sansation.ttf"))
    {
        throw std::runtime_error("Failed to load font!");
    }
}

TextList::TextList() : xpos{}, ypos{}, shadow{}, bold{}
{
    size = 11;
}

void ChatBox::addChat(std::string Text, sf::Color Color)
{
    ChatLine NewLine;
    NewLine.line = Text;
    NewLine.color = Color;
    chatStorage.push_back(NewLine);
}

void ChatBox::displayChat(sf::Vector2f Position)
{
    for (size_t i = 0; i != chatStorage.size(); i++)
    {
        textList.createText(Position.x,
                            (Position.y - (chatStorage.size() * 10)) + (i * 10),
                            11, chatStorage[i].color, chatStorage[i].line);
    }
}
