#include "Text.h"

class ChatBox ChatBox;
cTextList cText;

void cTextList::Setup()
{
    sString.setColor(Color);
    sString.setCharacterSize(Size);

    if (Bold == true)
    {
        sString.setStyle(sf::Text::Bold);
    }
    sString.setPosition(xpos, ypos);
}

void cTextList::DrawMe()
{
    if (Shadow == true)
    {

        sString.setPosition(xpos - 1, ypos - 1);
        sString.setColor(sf::Color(0, 0, 0));
        App.draw(sString);
    }
    Setup();
    App.draw(sString);
}

void cTextList::CreateText(int xpos, int ypos, int SizeMe, sf::Color color,
                           std::string stringvalue1, std::string String1,
                           float int1, std::string stringvalue2,
                           std::string String2, float int2,
                           std::string stringvalue3, std::string String3,
                           float int3, int Shadow, int NULL3)
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
    cTextList var;
    var.xpos = xpos;
    var.ypos = ypos;
    var.Color = color;
    var.sString = Textz;
    var.Size = Size;
    var.Shadow = Shadow;
    var.Bold = Bold;
    textlist.push_back(var);
}

void cTextList::DrawTextz()
{

    std::vector<cTextList>::iterator textz;
    for (textz = textlist.begin(); textz != textlist.end(); ++textz)
    {
        //if(textz->Shadow == true){textz->SetColor(textz->Color);Textz.SetPosition(textz->xpos+1,textz->ypos+1);App.Draw(Textz);}Textz.SetPosition(textz->xpos,textz->ypos);
        textz->DrawMe();
    }

    textlist.clear();
}

void cTextList::LoadFont()
{
    if (!font.loadFromFile("data/fonts/sansation.ttf"))
    {
        throw std::runtime_error("Failed to load font!");
    }
}

cTextList::cTextList()
{
    Size = 11;
}

void ChatBox::AddChat(std::string Text, sf::Color Color)
{
    ChatLine NewLine;
    NewLine.Line = Text;
    NewLine.Color = Color;
    ChatStorage.push_back(NewLine);
}

void ChatBox::DisplayChat(sf::Vector2f Position)
{
    for (size_t i = 0; i != ChatStorage.size(); i++)
    {
        cText.CreateText(Position.x,
                         (Position.y - (ChatStorage.size() * 10)) + (i * 10),
                         11, ChatStorage[i].Color, ChatStorage[i].Line);
    }
}
