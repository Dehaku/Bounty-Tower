#include "util.h"

void fSleep(float Time)
{
    sf::sleep(sf::seconds(Time));
}

bool Booleanize(int Num)
{
    if (Num <= 0)
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool Inbetween(float First, float Second, float Number)
{
    if (Number > First && Number < Second)
    {
        return true;
    }
    if (Number > Second && Number < First)
    {
        return true;
    }
    return false;
}

int randz(int minValue, int maxValue)
{
    // Apparently this method of randomization falls short one number.
    maxValue = maxValue + 1;
    //if(Debug){ std::cout << "randz(" << minValue << "," << maxValue << ") \n";}
    if (minValue == maxValue)
    {
        return minValue;
    }
    int range = (maxValue - minValue);
    int rawRand = (rand() % range);
    return (minValue + rawRand);
}

bool AABB(int Pointx, int Pointy, int Left, int Right, int Up, int Down)
{
    // Point(100,100), Up 80, Down 120, Left, 80, Right 120
    sf::Vector2f Point(Pointx, Pointy);
    if (Point.x >= Left && Point.x <= Right && Point.y >= Up && Point.y <= Down)
        return true;
    return false;
}

bool AABB(sf::Vector2f Point, int Left, int Right, int Up, int Down)
{

    if (Point.x >= Left && Point.x <= Right && Point.y >= Up && Point.y <= Down)
        return true;
    return false;
}

bool AABB(sf::Vector2i Point, int Left, int Right, int Up, int Down)
{

    if (Point.x >= Left && Point.x <= Right && Point.y >= Up && Point.y <= Down)
        return true;
    return false;
}

int CloseishS(int orix, int tarx)
{
    int xnum;
    xnum = orix - tarx;
    int num = xnum;
    if (num < 0)
    {
        num -= num * 2;
    }
    return num;
}

int StringFindNumber(std::string Stringy, std::string Term)
{
    size_t tStart;
    tStart = Stringy.find(Term);
    if (tStart != std::string::npos)
    {
        size_t tEnd;
        std::string Output;
        tEnd = Stringy.find("]", tStart + 1);
        if (tEnd != std::string::npos)
        {
            int iLength = Term.length();
            Output.assign(Stringy, tStart + iLength, tEnd - (tStart + iLength));
            auto Returns = atof(Output.c_str());
            if (Output == "true")
            {
                Returns = 1;
            }
            if (Output == "false")
            {
                Returns = 0;
            }
            return Returns;
        }
    }
    return 0;
}

std::string StringFindString(std::string Stringy, std::string Term)
{
    std::string Returns;
    size_t tStart;
    tStart = Stringy.find(Term);
    if (tStart != std::string::npos)
    {
        size_t tEnd;
        std::string Output;
        tEnd = Stringy.find("]", tStart + 1);
        if (tEnd != std::string::npos)
        {
            int iLength = Term.length();
            Output.assign(Stringy, tStart + iLength, tEnd - (tStart + iLength));
            Returns = Output;
            return Returns;
        }
    }
    return "";
}

std::string StringFindStringNpos(std::string Stringy, std::string Term)
{
    std::string Returns;
    size_t tStart;
    tStart = Stringy.find(Term);
    if (tStart != std::string::npos)
    {
        size_t tEnd;
        std::string Output;
        tEnd = Stringy.find("]", tStart + 1);

        {
            int iLength = Term.length();
            Output.assign(Stringy, tStart + iLength, tEnd - (tStart + iLength));
            Returns = Output;
            return Returns;
        }
    }
    return "";
}

std::vector<std::string> StringFindElements(std::string Source,
                                            std::string Seperater)
{

    std::vector<std::string> Returns;
    bool FirstRun = true;

    bool WhileBreaker = false;
    while (WhileBreaker == false)
    {
        size_t tStart = 0;
        size_t tEnd = 0;
        tEnd = Source.find(Seperater, tStart + 1);

        std::string Element;

        if (FirstRun)
        {
            // Ignoring the later +1, since we don't want it ignoring the
            // first character if it's not a seperater
            Element.append(Source, tStart, tEnd - tStart);
            FirstRun = false;
        }
        else
            // +1 so it ignores the : that was used to find it,
            // -1 since the +1 pushed it over, causing it to grab the 'next' :
            Element.append(Source, tStart + 1, tEnd - tStart - 1);

        Returns.push_back(Element);
        tStart = tEnd;

        if (tEnd >= std::string::npos)
        {
            WhileBreaker = true;
        }
    }
    return Returns;
}

std::vector<float> NumericSplitter(float Received, float SplitVariable)
{
    Con("Beginning That Function, Recieved: ", false);
    Con(Received);

    std::vector<float> Returns;
    for (int i = 0; i < Received; i += SplitVariable)
    {
        Con("Running That Function");
        if (Received - i > SplitVariable)
            Returns.push_back(SplitVariable);
        if (Received - i <= SplitVariable)
            Returns.push_back(Received - i);
    }
    Con("Returning That Function");
    return Returns;
}

float PercentageBuff(float Received)
{
    return Received / 100;
}

std::string StringFindChaos(std::string Stringy, std::string Term,
                            std::string Ending)
{
    std::string Returns;
    size_t tStart;
    tStart = Stringy.find(Term);
    if (tStart != std::string::npos)
    {
        size_t tEnd;
        std::string Output;
        tEnd = Stringy.find(Ending, tStart + 1);
        if (tEnd != std::string::npos)
        {
            int iLength = Term.length();
            Output.assign(Stringy, tStart + iLength, tEnd - (tStart + iLength));
            Returns = Output;
            return Returns;
        }
    }

    // If we couldn't find chaos, we just return an empty string
    return "";
}

std::set<std::string> StringFindSetChaos(std::string Stringy, std::string Term,
                                         std::string Ending)
{
    std::set<std::string> Returns;
    bool NotDone = true;
    // std::cout <<"Working With:" << Stringy << std::endl;
    int Goal = 0;
    while (NotDone)
    {
        NotDone = false;
        size_t tStart = Stringy.find(Term);
        size_t tEnd;
        std::string Output;
        tEnd = Stringy.find(Ending, tStart + 1);
        if (tEnd != std::string::npos)
        {
            std::cout << tEnd << std::endl;
            sf::sleep(sf::seconds(0.2));
            int iLength = Term.length();
            Output.assign(Stringy, tStart + iLength, tEnd - (tStart + iLength));
            Returns.insert(Output);
            Stringy.replace((tStart + iLength) - 1,
                            (tEnd - ((tStart)+iLength)) + 2, "");
            std::cout << Stringy << std::endl;
            if (Goal < 500)
            {
                NotDone = true;
            }
            Goal++;
            //tStart = tEnd;
        }
        else
        {
        }
    }
    for (const auto &Return : Returns)
    {
        std::cout << Return << std::endl;
    }
    return Returns;
}

float PercentIs(float Value, float Percentage)
{
    // Divide the return by 100 for maths.
    if (Value == 0)
        Value = 0.000000001;
    if (Percentage == 0)
        Percentage = 0.000000001;

    float Percent = Percentage / Value * 100;
    if (Percent == 0)
        return 0.00000000001;
    return Percent;
}

bool Toggle(bool &Boolean)
{
    if (Boolean == true)
        Boolean = false;
    else if (Boolean == false)
        Boolean = true;
    return Boolean;
}

std::string GenerateName(int MinLength, int MaxLength)
{
    // TODO: Add a rememberance for the last added letter,
    // and add a 50% chance to NOT do the same letter again.
    std::string Inserter;
    std::string Name;
    int Length = randz(MinLength, MaxLength);
    bool Vowel = true;
    bool DoubleLetter = false;
    if (randz(0, 1) == 1)
        Vowel = false;

    for (int i = 0; i != Length; i++)
    {
        if (Vowel)
        {
            int VowelGen = randz(0, 5);
            if (VowelGen == 0)
                Inserter = "A";
            if (VowelGen == 1)
                Inserter = "E";
            if (VowelGen == 2)
                Inserter = "I";
            if (VowelGen == 3)
                Inserter = "O";
            if (VowelGen == 4)
                Inserter = "U";
            if (VowelGen == 5)
                Inserter = "Y";

            Name.append(Inserter);

            if (DoubleLetter)
            {
                DoubleLetter = false;
                Vowel = false;
            }
            else if (randz(0, 2) > 0)
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
            int ConsenantGen = randz(0, 24);

            if (ConsenantGen == 0)
                Inserter = "B";
            if (ConsenantGen == 1)
                Inserter = "C";
            if (ConsenantGen == 2)
                Inserter = "D";
            if (ConsenantGen == 3)
                Inserter = "F";
            if (ConsenantGen == 4)
                Inserter = "G";
            if (ConsenantGen == 5)
                Inserter = "H";
            if (ConsenantGen == 6)
                Inserter = "J";
            if (ConsenantGen == 7)
                Inserter = "K";
            if (ConsenantGen == 8)
                Inserter = "L";
            if (ConsenantGen == 9)
                Inserter = "M";
            if (ConsenantGen == 10)
                Inserter = "N";
            if (ConsenantGen == 11)
                Inserter = "P";
            if (ConsenantGen == 12)
                Inserter = "QU";
            //if(ConsenantGen == 12) Inserter = "Q";
            if (ConsenantGen == 13)
                Inserter = "R";
            if (ConsenantGen == 14)
                Inserter = "S";
            if (ConsenantGen == 15)
                Inserter = "T";
            if (ConsenantGen == 16)
                Inserter = "V";
            if (ConsenantGen == 17)
                Inserter = "W";
            if (ConsenantGen == 18)
                Inserter = "X";
            if (ConsenantGen == 19)
                Inserter = "Y";
            if (ConsenantGen == 20)
                Inserter = "Z";

            if (ConsenantGen == 21)
                Inserter = "CH";
            if (ConsenantGen == 22)
                Inserter = "SH";
            if (ConsenantGen == 23)
                Inserter = "TH";
            if (ConsenantGen == 24)
                Inserter = "LL";

            Name.append(Inserter);

            if (DoubleLetter)
            {
                DoubleLetter = false;
                Vowel = true;
            }
            else if (randz(0, 2) > 0)
                Vowel = true;
            else
                DoubleLetter = true;
        }
    }
    return Name;
}
