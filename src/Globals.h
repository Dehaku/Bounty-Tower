#ifndef __GLOBALS_H_INCLUDED__
#define __GLOBALS_H_INCLUDED__


//#include <iostream>

#include "Game.h"


bool Debug = false;
bool InFocus = true;
float PI = 3.141;

    sf::Vector2f RMBHeldPos;




    sf::RenderTexture renderTexture;
        sf::Image Img;
        sf::Texture Tex;

        sf::Sprite Sprt;
        sf::Sprite Sprt2;


class ID
{
    public:
    enum WorldTile { City, Spire, Grass, Road, River, Ocean };
};
ID ID;



sf::Vector2f Rez(1024,768);

    sf::Vector2f Center(500, 500);
    sf::Vector2f HalfSize(Rez.x/2, Rez.y/2);
    sf::View View1(Center, HalfSize);

    int AirPressure = 10;

    float GX = 0;
    float GY = 0;



    sf::Vector2f MousePos;
    int MouseStagnation = 0;


    sf::Sprite NewTile;


    bool AllowedToAttack = true; // Set this to each NPC


    std::vector<int> Selected;

    float testx = 0;
    float testy = 0;

    float testx2 = 0;
    float testy2 = 0;

    float GCtimescale = 1;

    int MyTarget = -1;
    int MyTargetid = -1;

    float CameraZoom = 1;

    //sf::Vector2i CameraSize = Rez;

    float CZ(float Number)
    {
        return Number/CameraZoom;
    }

    const int gridx = 100;const int gridy = 100;const int gridz = 10;
    int currentx = 50;int currenty = 50;int currentz = 6;

    int currentregionx = 50;
    int currentregiony = 50;
    int currentplanet = 500;

    const int GridSize = 20;



    std::string Zom("[Zombification:");
    std::string Poi("[Poison:");


    int groundmap[gridz][gridy][gridx];
    int smellmap[gridz][gridy][gridx];
    int sunmap[gridz][gridy][gridx];

    enum skills { ag,ch,in,de,st,pe,en,wi }; // I love the usability this has, But I hate the globalization, Maybe toss this into a class?







void fSleep(float Time){
    sf::sleep(sf::seconds(Time));
}


void debug(std::string Info){
    if(Debug) std::cout << Info << std::endl;
}

void PrintToFile(std::string FileName, std::string Text, bool EndLine = true)
{
    using namespace std; // Start Saving Map

    /*debug("Saving current map to file...");
    string line("");
    stringstream convert;
    convert << planet;
    convert << Cords.x;
    convert << Cords.y;
    line.append(convert.str());
    string ending(".map");
    line.append(ending);
    cout << line << endl;
    */
    ofstream outputFile(FileName.c_str());
    //outputFile << biometype << endl;
    //debug("Writting map to file...");

    outputFile << Text;

    if(EndLine) outputFile << endl;

}


void Con(std::string Text, bool EndLine = true){
    if(EndLine)
    {
        std::cout << Text << std::endl;
    }
    else
    {
        std::cout << Text;
    }
}

void Con(float Number, bool EndLine = true){
    if(EndLine)
    {
        std::cout << Number << std::endl;
    }
    else
    {
        std::cout << Number;
    }
}


sf::Color White(255,255,255,255);
sf::Color Red(255,0,0,255);
sf::Color Green(0,255,0,255);
sf::Color Blue(0,0,255,255);
sf::Color Cyan(0, 150, 255,255);
sf::Color Yellow(255,255,0,255);
sf::Color Black(0,0,0,255);
sf::Color Brown(100, 100, 0,255);

bool Booleanize(int Num){
    if(Num <= 0){ return false;}
    else{ return true;}
}


bool Inbetween(float First,float Second, float Number)
{
    if(Number > First && Number < Second)
    {
        return true;
    }
    if(Number > Second && Number < First)
    {
        return true;
    }
    return false;
}


int randz( int minValue, int maxValue ){

    maxValue = maxValue+1; // Apparently this method of randomization falls short one number.
    //if(Debug){ std::cout << "randz(" << minValue << "," << maxValue << ") \n";}
    if(minValue == maxValue){ return minValue; }
    int range = (maxValue - minValue);
    int rawRand = (rand() % range);
    return (minValue + rawRand);
}

bool AABB(int Pointx, int Pointy, int Left, int Right, int Up, int Down)
{
    sf::Vector2f Point(Pointx,Pointy); // Point(100,100), Up 80, Down 120, Left, 80, Right 120
    if(Point.x >= Left && Point.x <= Right && Point.y >= Up && Point.y <= Down ) return true;
    return false;
}

bool AABB(sf::Vector2f Point, int Left, int Right, int Up, int Down)
{

    if(Point.x >= Left && Point.x <= Right && Point.y >= Up && Point.y <= Down) return true;
    return false;
}

int CloseishS(int orix,int tarx){
    int xnum;
    xnum = orix-tarx;
    int num = xnum;
    if(num < 0){ num -= num*2;}
    return num;
};

class Math
{
    public:
    int Closeish(int orix,int oriy,int tarx,int tary){
        int Powa = sqrt( pow( (tarx-orix),2 ) + pow( (tary-oriy),2) );
        return Powa;
    }

    int Closeisha(int orix,int oriy,int tarx,int tary){ // Original
        int xnum;
        int ynum;
        xnum = orix-tarx;
        if(xnum < 0){xnum -= xnum*2;}
        ynum = oriy-tary;
        if(ynum < 0){ynum -= ynum*2;}
        int num = xnum+ynum;
        return num;
    };



    int DistanceTrace(int xa, int ya, int xb, int yb){// Highly inaccurate
        int dx = xb - xa, dy = yb - ya, steps, k;
        float xIncrement, yIncrement, x = xa, y = ya;
        if(abs(dx) > abs(dy)) steps = abs(dx);
        else steps = abs(dy);
        xIncrement = dx / (float) steps;
        yIncrement = dy / (float) steps;
        int distance = 0;

        for(int k = 0; k < steps; k++){
            x += xIncrement;
            y += yIncrement;
            distance++;
        }
    return distance;
    }


    int Closeishxx(int orix,int oriy,int tarx,int tary){ // Returns an octogonish type ring of detection, 10% overshooting.
        int Sqr = DistanceTrace(orix,oriy,tarx,tary);
        int Dia = Closeisha(orix,oriy,tarx,tary);
        int Powa = (Sqr+Dia)/2;
        return Powa;
    }

    bool Exceed(float Value, float Limit){
        if(Value > Limit)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    int Clamp(int Value, int MinValue, int MaxValue){
        if(Value > MaxValue){return MaxValue;}
        if(Value < MinValue){return MinValue;}
        else{return Value;}
    }


    float AngleBetweenVectors(sf::Vector2f a, sf::Vector2f b){
        //  (180 / PI = 57.3065)
        //return 114.5915f * atan2(b.y - a.y, b.x - a.x);
        return 57.3065f * atan2(b.y - a.y, b.x - a.x);
    }

    sf::Vector2f CircleRandz(int xpos, int ypos, int radius){
        float Xxx = 0; // global
        float Yyy = 0; // global
        float speeds = 1; // global
        int xanchor = xpos; // global
        int yanchor = ypos; // global
        float Degrees = 0;
        int Ran = randz(0,360);
        bool Found = false;
        for( int Turns = -1; Turns != Ran; Turns++)
        {
            Xxx = xanchor + cosf(Degrees * PI/180) * radius;
    		Yyy = yanchor + sinf(Degrees * PI/180) * radius;
    		Degrees += speeds; // Should probably leave speed at 1, If not less.
        }
        return sf::Vector2f(Xxx,Yyy);
    }

    sf::Vector2f Vec(float X, float Y){// This may seem pointless, But I like my stuff compressed.
        sf::Vector2f Returns(X,Y);
        return Returns;
    }
};
Math Math;


int StringFindNumber(std::string Stringy,std::string Term){
    int Returns;
    size_t tStart;
    tStart = Stringy.find(Term);
    if (tStart != std::string::npos)
    {
        size_t tEnd;
        std::string Output;
        tEnd = Stringy.find("]",tStart+1);
        if(tEnd != std::string::npos)
        {
            int iLength = Term.length();
            Output.assign(Stringy,tStart+iLength,tEnd-(tStart+iLength));
            Returns = atof(Output.c_str());
            if(Output == "true"){ Returns = 1;}
            if(Output == "false"){ Returns = 0;}
            return Returns;
        }
    }
    return 0;
}

std::string StringFindString(std::string Stringy,std::string Term){
    std::string Returns;
    size_t tStart;
    tStart = Stringy.find(Term);
    if (tStart != std::string::npos)
    {
        size_t tEnd;
        std::string Output;
        tEnd = Stringy.find("]",tStart+1);
        if(tEnd != std::string::npos)
        {
            int iLength = Term.length();
            Output.assign(Stringy,tStart+iLength,tEnd-(tStart+iLength));
            Returns = Output;
            return Returns;
        }
    }
    return "";
}

std::string StringFindStringNpos(std::string Stringy,std::string Term){
    std::string Returns;
    size_t tStart;
    tStart = Stringy.find(Term);
    if (tStart != std::string::npos)
    {
        size_t tEnd;
        std::string Output;
        tEnd = Stringy.find("]",tStart+1);
        if(true == true)
        {
            int iLength = Term.length();
            Output.assign(Stringy,tStart+iLength,tEnd-(tStart+iLength));
            Returns = Output;
            return Returns;
        }
    }
    return "";
}




std::vector<std::string> StringFindElements(std::string Source, std::string Seperater = ":")
{
    debug("Start StringFindElements");

    std::vector<std::string> Returns;
    size_t tStart = 0;
    size_t tEnd = 0;
    bool FirstRun = true;

    bool WhileBreaker = false;
    while(WhileBreaker == false)
    {
        debug("Starting Search");
        tEnd = Source.find(Seperater,tStart+1);

        std::string Element;

        if(FirstRun)
        {
            Element.append(Source,tStart,tEnd-tStart); // Ignoring the later +1, since we don't want it ignoring the first character if it's not a seperater
            FirstRun = false;
        }
        else Element.append(Source,tStart+1,tEnd-tStart-1); // +1 so it ignores the : that was used to find it, -1 since the +1 pushed it over, causing it to grab the 'next' :

        Returns.push_back(Element);
        tStart = tEnd;
        debug("Finished and added");

        if(tEnd >= std::string::npos)
        {
            debug("tEnd == Source.npos");
            WhileBreaker = true;
        }


    }
    debug("Returning StringFindElements");
    return Returns;
}


std::vector<float> NumericSplitter(float Received, float SplitVariable = 100)
{
    Con("Beginning That Function, Recieved: ", false);
    Con(Received);

    std::vector<float> Returns;
    for(int i = 0; i < Received; i += SplitVariable)
    {
        Con("Running That Function");
        if(Received-i > SplitVariable) Returns.push_back(SplitVariable);
        if(Received-i <= SplitVariable) Returns.push_back(Received-i);
    }
    Con("Returning That Function");
    return Returns;
}

float PercentageBuff(float Received)
{
    return Received/100;
}






std::string StringFindChaos(std::string Stringy,std::string Term,std::string Ending){
    std::string Returns;
    size_t tStart;
    tStart = Stringy.find(Term);
    if (tStart != std::string::npos)
    {
        size_t tEnd;
        std::string Output;
        tEnd = Stringy.find(Ending,tStart+1);
        if(tEnd != std::string::npos)
        {
            int iLength = Term.length();
            Output.assign(Stringy,tStart+iLength,tEnd-(tStart+iLength));
            Returns = Output;
            return Returns;
        }
    }

    // If we couldn't find chaos, we just return an empty string
    return "";
}

std::set<std::string> StringFindSetChaos(std::string Stringy,std::string Term,std::string Ending){
    std::set<std::string> Returns;
    size_t tStart;
    bool NotDone = true;
    // std::cout <<"Working With:" << Stringy << std::endl;
    int Goal = 0;
    while(NotDone)
    {
        NotDone = false;
        tStart = Stringy.find(Term);
        size_t tEnd;
        std::string Output;
        tEnd = Stringy.find(Ending,tStart+1);
        if(tEnd != std::string::npos)
        {
            std::cout << tEnd << std::endl;
            sf::sleep(sf::seconds(0.2));
            int iLength = Term.length();
            Output.assign(Stringy,tStart+iLength,tEnd-(tStart+iLength));
            Returns.insert(Output);
            Stringy.replace((tStart+iLength)-1,(tEnd-((tStart)+iLength))+2,"");
            std::cout << Stringy << std::endl;
            if(Goal < 500){NotDone = true;}
            Goal++;
            //tStart = tEnd;
        }
        else
        {
        }
    }
    for(const auto & Return : Returns)
    {
        std::cout << Return << std::endl;
    }
    return Returns;
}

std::string AddString(std::string Original, std::string Addition){
    Original.append(Addition);

    return Original;
}
std::string AddString(std::string Original, std::string Addition, std::string Addition2){
    Original.append(Addition);
    Original.append(Addition2);

    return Original;
}
std::string AddString(std::string Original, std::string Addition, std::string Addition2, std::string Addition3){
    Original.append(Addition);
    Original.append(Addition2);
    Original.append(Addition3);

    return Original;
}

std::string NumbertoString(float Number)
{
    std::stringstream ReturnLine;
    ReturnLine << Number;
    std::string str = ReturnLine.str();
    return str;

}

/*
float stof(std::string String) // String to Float.
{
    std::string Result;

    std::ostringstream convert;

    convert << Number;

    Result = convert.str();
}
*/

float PercentIs(float Value, float Percentage){ // Divide the return by 100 for maths.
        if(Value == 0) Value = 0.000000001;
        if(Percentage == 0) Percentage = 0.000000001;

        float Percent = Percentage/Value*100;
        if(Percent == 0) return 0.00000000001;
        return Percent;
    }



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


    Globes()
    {
        globalid = 100;
        glbbtn = 100;
        Scalex = 1;
        Scaley = 1;
        ButtonClicked = false;
        ButtonClickedTime = 0;
        Following = false;
        GenString = "";
        InitalZeds = false;
        TimeScale = 1;
    }
};
Globes Globals;

std::string GenerateName(int MinLength = 2, int MaxLength = 10){
    std::string Inserter; // TODO: Add a rememberance for the last added letter, and add a 50% chance to NOT do the same letter again.
    std::string Name;
    int Length = randz(MinLength, MaxLength);
    bool Vowel = true;
    bool DoubleLetter = false;
    if(randz(0,1) == 1) Vowel = false;

    for(int i = 0; i != Length; i++)
    {
        if(Vowel)
        {
            int VowelGen = randz(0,5);
            if(VowelGen == 0) Inserter = "A";
            if(VowelGen == 1) Inserter = "E";
            if(VowelGen == 2) Inserter = "I";
            if(VowelGen == 3) Inserter = "O";
            if(VowelGen == 4) Inserter = "U";
            if(VowelGen == 5) Inserter = "Y";

            Name.append(Inserter);

            if(DoubleLetter)
            {
                DoubleLetter = false;
                Vowel = false;
            }
            else if(randz(0,2) > 0)
            {
                Vowel = false;
            }
            else
            {
                DoubleLetter = true;
            }
        }
        else
        {
            int ConsenantGen = randz(0,24);

            if(ConsenantGen == 0) Inserter = "B";
            if(ConsenantGen == 1) Inserter = "C";
            if(ConsenantGen == 2) Inserter = "D";
            if(ConsenantGen == 3) Inserter = "F";
            if(ConsenantGen == 4) Inserter = "G";
            if(ConsenantGen == 5) Inserter = "H";
            if(ConsenantGen == 6) Inserter = "J";
            if(ConsenantGen == 7) Inserter = "K";
            if(ConsenantGen == 8) Inserter = "L";
            if(ConsenantGen == 9) Inserter = "M";
            if(ConsenantGen == 10) Inserter = "N";
            if(ConsenantGen == 11) Inserter = "P";
            if(ConsenantGen == 12) Inserter = "QU";
            //if(ConsenantGen == 12) Inserter = "Q";
            if(ConsenantGen == 13) Inserter = "R";
            if(ConsenantGen == 14) Inserter = "S";
            if(ConsenantGen == 15) Inserter = "T";
            if(ConsenantGen == 16) Inserter = "V";
            if(ConsenantGen == 17) Inserter = "W";
            if(ConsenantGen == 18) Inserter = "X";
            if(ConsenantGen == 19) Inserter = "Y";
            if(ConsenantGen == 20) Inserter = "Z";

            if(ConsenantGen == 21) Inserter = "CH";
            if(ConsenantGen == 22) Inserter = "SH";
            if(ConsenantGen == 23) Inserter = "TH";
            if(ConsenantGen == 24) Inserter = "LL";

            Name.append(Inserter);

            if(DoubleLetter)
            {
                DoubleLetter = false;
                Vowel = true;
            }
            else if(randz(0,2) > 0) Vowel = true;
            else DoubleLetter = true;
        }
    }
    return Name;
}

bool Toggle(bool &Boolean)
{
    if(Boolean == true) Boolean = false;
    else if(Boolean == false) Boolean = true;
    return Boolean;
}

#endif // GLOBALS_H_INCLUDED
