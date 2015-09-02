#include "Text.h"

#include <sstream>

class ChatBox chatBox;
TextList textList;

extern sf::RenderWindow window;


sf::Text drawText(sf::Vector2f vPos, std::string text, sf::Color color, int size)
{
    sf::Text returnText;
    {
        returnText.setString(text);
        returnText.setColor(color);
        returnText.setPosition(vPos);
        returnText.setCharacterSize(size);

        returnText.setFont(gvars::defaultFont);
    }
    return returnText;
}


std::string randomWindowName()
{
    std::vector<std::string> const names{
        "Zombificational Settlementation Experimentation", "Galaxy",
        "Ye Old Settlement, Ye New Galaxy",
        "The first Galaxy had Joey, Just saying",
        "First comes the universe, then comes Galaxy"};

    return names[randz(0, names.size() - 1)];
}

sf::Text createText(std::string Text)
{
    sf::Text outwardText;
    outwardText.setString(Text);
    outwardText.setColor(sf::Color::White);
    outwardText.setCharacterSize(15);
    outwardText.setFont(gvars::defaultFont);

    return outwardText;
}



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
    sf::View oldView = window.getView();
    window.setView(drawView);

    setup();
    if (shadow == true)
    {

        sString.setPosition(xpos - 1, ypos - 1);
        sString.setColor(sf::Color(0, 0, 0));
        window.draw(sString);
    }
    setup();


    window.draw(sString);

    window.setView(oldView);
}

void TextList::createText(int xpos, int ypos, int sizeMe, sf::Color color,
                          std::string stringvalue1, std::string string1,
                          float int1, std::string stringvalue2,
                          std::string string2, float int2,
                          std::string stringvalue3, std::string string3,
                          float int3, int shadow, int /*NULL3*/)
{
    int size = sizeMe;
    bool bold = true;

    using namespace std;
    string line1("");
    stringstream convert1;
    stringstream convert2;
    stringstream convert3;

    line1.append(stringvalue1);

    line1.append(string1);

    if (int1 != -6698)
    {
        convert1 << int1;
        line1.append(convert1.str());
    }

    line1.append(stringvalue2);

    line1.append(string2);

    if (int2 != -6698)
    {
        convert2 << int2;
        line1.append(convert2.str());
    }

    line1.append(stringvalue3);

    line1.append(string3);

    if (int3 != -6698)
    {
        convert3 << int3;
        line1.append(convert3.str());
    }

    sf::Text textz(line1, font);

    textz.setCharacterSize(size);
    if (bold == true)
    {
        textz.setStyle(sf::Text::Bold);
    }

    //if(shadow == true){Textz.SetText(line1);Textz.SetColor(sf::Color(0,0,0));Textz.SetPosition(xpos+1,ypos+1);App.Draw(Textz);}
    textz.setColor(color);
    textz.setPosition(xpos, ypos); //App.Draw(Textz);
    TextList var;
    var.xpos = xpos;
    var.ypos = ypos;
    var.color = color;
    var.sString = textz;
    var.size = size;
    var.shadow = shadow;
    var.bold = bold;
    textlist.push_back(var);
}

void TextList::createText(sf::Vector2f vPos, int sizeMe, sf::Color color,
                    std::string stringvalue, sf::View drawView)
{
    using namespace std;
    string line1("");
    stringstream convert1;

    line1.append(stringvalue);

    sf::Text textz(line1, font);
    textz.setStyle(sf::Text::Bold);
    textz.setCharacterSize(sizeMe);
    textz.setColor(color);
    textz.setPosition(vPos.x, vPos.y);

    TextList var;
    var.drawView = drawView;
    var.xpos = vPos.x;
    var.ypos = vPos.y;
    var.color = color;
    var.sString = textz;
    var.size = size;
    var.shadow = 1;
    var.bold = bold;
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
    drawView = gvars::view1;
}

void ChatBox::addChat(std::string text, sf::Color color)
{
    ChatLine newLine;
    newLine.line = text;
    newLine.color = color;
    chatStorage.push_back(newLine);
}

void ChatBox::displayChat(sf::Vector2f position)
{
    /*
    for (size_t i = 0; i != chatStorage.size(); i++)
    {
        textList.createText(position.x,
                            (position.y - (chatStorage.size() * 10)) + (i * 10),
                            11, chatStorage[i].color, chatStorage[i].line);
    }
    */

    for (size_t i = chatStorage.size(); i != (chatStorage.size()-5); i--)
    {
        std::cout << "Chatbox (i):" << i << std::endl;
        textList.createText(position.x,
                            (position.y - (chatStorage.size() * 10)) + (i * 10),
                            11, chatStorage[i].color, chatStorage[i].line);
    }


}
