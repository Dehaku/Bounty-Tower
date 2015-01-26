#ifndef __EFFECTS_H_INCLUDED__
#define __EFFECTS_H_INCLUDED__

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
// *#include <math.h>

#include "Game.h"

extern sf::RenderWindow App;

/* Need to Make
Kamehame styled beam
*/

class Effects
{
    public:
    float AngleBetweenVectors(sf::Vector2f a, sf::Vector2f b){
        //  (180 / PI = 57.3065)
        //return 114.5915f * atan2(b.y - a.y, b.x - a.x);
        return 57.3065f * atan2(b.y - a.y, b.x - a.x);
    }
    int Closeish(int orix,int oriy,int tarx,int tary){
        int Powa = sqrt( pow( (tarx-orix),2 ) + pow( (tary-oriy),2) );
        return Powa;
    }
    class cLine
    {
        public:
        sf::Vector2f StartPos;
        sf::Vector2f EndPos;
        int size;
        int duration;
        bool fading;
        int faderate;
        int outline;
        sf::Color maincolor;
        sf::Color seccolor;

        bool DrawMe(){
            /*//sf::Shape Line = sf::Shape::Line(StartPos.x,StartPos.y,EndPos.x,EndPos.y, size, maincolor, outline, seccolor);
            sf::Vertex Line[2] = {StartPos,EndPos};
            //Line.setFillColor(maincolor);
            //maincolor.a -= faderate;
            //if(maincolor.a < 0){maincolor.a = 0;}
            App.draw(Line,2,sf::Lines);*/

            sf::RectangleShape rectangle;

            int Length = sqrt( pow( (EndPos.x-StartPos.x),2 ) + pow( (EndPos.y-StartPos.y),2) );
            rectangle.setSize(sf::Vector2f(Length, size));
            //rectangle.setSize(sf::Vector2f(StartPos.x-EndPos.x,StartPos.y-EndPos.y));
            float Angle = 57.3065f * atan2(EndPos.y-StartPos.y, EndPos.x-StartPos.x);
            rectangle.setRotation(Angle);

            rectangle.setOrigin(0,size/2);

            rectangle.setFillColor(maincolor);
            rectangle.setOutlineColor(seccolor);
            rectangle.setOutlineThickness(outline);
            rectangle.setPosition(StartPos.x, StartPos.y);
            App.draw(rectangle);

        }
    };
    class cCircle
    {
        public:
        sf::Vector2f Pos;
        int size;
        bool outline;
        sf::Color maincolor;
        sf::Color seccolor;

        bool DrawMe(){
        //sf::Shape Circle = sf::Shape::Circle(Pos.x,Pos.y,size, maincolor, outline, seccolor);
        sf::CircleShape Circle;
        Circle.setRadius(size);
        Circle.setFillColor(maincolor);
        Circle.setOutlineThickness(outline);
        Circle.setOutlineColor(seccolor);
        Circle.setPosition(Pos.x,Pos.y);
        Circle.setOrigin(size,size);
        App.draw(Circle);
        }


    };
    class cSquare
    {
        public:
        sf::Vector2f StartPos;
        sf::Vector2f EndPos;
        bool outline;
        sf::Color maincolor;
        sf::Color seccolor;

        bool DrawMe(){
            //sf::Shape Square = sf::Shape::Rectangle(StartPos.x,StartPos.y,EndPos.x,EndPos.y, maincolor, outline, seccolor);
            sf::RectangleShape rectangle;
            rectangle.setSize(sf::Vector2f(100, 50));
            rectangle.setSize(sf::Vector2f(StartPos.x-EndPos.x,StartPos.y-EndPos.y));
            rectangle.setFillColor(maincolor);
            rectangle.setOutlineColor(seccolor);
            rectangle.setOutlineThickness(outline);
            rectangle.setPosition(EndPos.x, EndPos.y);
            App.draw(rectangle);
        }


    };

    class cBeam
    {
        public:
        sf::Vector2f StartPos;
        sf::Vector2f EndPos;
        int size;
        int duration;
        bool fading;
        int faderate;
        int outline;
        sf::Color maincolor;
        sf::Color seccolor;

        bool DrawMe(){
            //sf::Shape Line = sf::Shape::Line(StartPos.x,StartPos.y,EndPos.x,EndPos.y, size, maincolor, outline, seccolor);
            sf::Vertex Line[2] = {StartPos,EndPos};
            //Line.setFillColor(maincolor);
            //maincolor.a -= faderate;
            //if(maincolor.a < 0){maincolor.a = 0;}
            App.draw(Line,2,sf::Lines);

            sf::RectangleShape rectangle;

            int Length = sqrt( pow( (EndPos.x-StartPos.x),2 ) + pow( (EndPos.y-StartPos.y),2) );
            rectangle.setSize(sf::Vector2f(Length, size));
            //rectangle.setSize(sf::Vector2f(StartPos.x-EndPos.x,StartPos.y-EndPos.y));
            float Angle = 57.3065f * atan2(EndPos.y-StartPos.y, EndPos.x-StartPos.x);
            rectangle.setRotation(Angle);

            rectangle.setOrigin(0,size/2);

            rectangle.setFillColor(maincolor);
            rectangle.setOutlineColor(seccolor);
            rectangle.setOutlineThickness(outline);
            rectangle.setPosition(StartPos.x, StartPos.y);
            App.draw(rectangle);


        }
    };
    std::vector<cCircle> Circles;
    std::vector<cLine> Lines;
    std::vector<cBeam> Beams;
    std::vector<cSquare> Squares;

    bool CreateLine(int Sxpos, int Sypos, int Expos, int Eypos, int Size, sf::Color MainColor, float Outline = 0, sf::Color SecondaryColor = sf::Color(0,0,0), bool Fades = false, int Fadesrate = 255)
    {
        cLine evar;
        evar.StartPos = sf::Vector2f(Sxpos,Sypos);
        evar.EndPos = sf::Vector2f(Expos,Eypos);
        evar.size = Size;
        evar.fading = Fades;
        evar.faderate = Fadesrate;
        evar.outline = Outline;
        evar.maincolor = MainColor;
        evar.seccolor = SecondaryColor;
        Lines.push_back(evar);
    }

    bool CreateBeam(int Sxpos, int Sypos, int Expos, int Eypos, int Size, sf::Color MainColor, float Outline = 0, sf::Color SecondaryColor = sf::Color(0,0,0), bool Fades = false, int Fadesrate = 255)
    {
        cBeam evar;
        evar.StartPos = sf::Vector2f(Sxpos,Sypos);
        evar.EndPos = sf::Vector2f(Expos,Eypos);
        evar.size = Size;
        evar.fading = Fades;
        evar.faderate = Fadesrate;
        evar.outline = Outline;
        evar.maincolor = MainColor;
        evar.seccolor = SecondaryColor;
        Beams.push_back(evar);
    }

    bool CreateSquare(int Sxpos, int Sypos, int Expos, int Eypos, sf::Color MainColor, float Outline = 0, sf::Color SecondaryColor = sf::Color(0,0,0))
    {
        cSquare evar;
        evar.StartPos = sf::Vector2f(Sxpos,Sypos);
        evar.EndPos = sf::Vector2f(Expos,Eypos);
        evar.outline = Outline;
        evar.maincolor = MainColor;
        evar.seccolor = SecondaryColor;
        Squares.push_back(evar);
    }

    bool CreateCircle(int xpos, int ypos, int Size, sf::Color MainColor, float Outline = 0, sf::Color SecondaryColor = sf::Color(0,0,0))
    {
     cCircle evar; evar.Pos = sf::Vector2f(xpos,ypos); evar.size = Size;
     evar.outline = Outline; evar.maincolor = MainColor; evar.seccolor = SecondaryColor;
     Circles.push_back(evar);
    }


    bool DrawEffects()
    {
        if(true == true){
        std::vector<cCircle>::iterator effectz;
        std::vector<cCircle>::iterator remova;
        int remover = 0;
        int removerz = 0;
        for( effectz = Circles.begin(); effectz != Circles.end(); ++effectz ){

            effectz->DrawMe();
            remover++;
            //if(effectz->fading == true && effectz->maincolor.a <= 0){removerz = remover;}
        } Circles.clear(); }

        if(true == true){
        std::vector<cSquare>::iterator effectz;
        for( effectz = Squares.begin(); effectz != Squares.end(); ++effectz ){

            effectz->DrawMe();

        } Squares.clear();}

        if(true == true){
        std::vector<cLine>::iterator effectz;
        for( effectz = Lines.begin(); effectz != Lines.end(); ++effectz ){
            effectz->DrawMe();
        } Lines.clear();}

        if(true == true){
        std::vector<cBeam>::iterator effectz;
        for( effectz = Beams.begin(); effectz != Beams.end(); ++effectz ){
            effectz->DrawMe();
        } Beams.clear();}

    }

};
Effects Effectz;

#endif // EFFECTS_H_INCLUDED
