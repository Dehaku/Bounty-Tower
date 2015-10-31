
#include "util.h"

std::string str(int num)
{
    return std::to_string(num);
}
std::string str(float num)
{
    return std::to_string(num);
}

RandomWeightList::WeightEntry::WeightEntry()
{
    weight = 0;
}

void RandomWeightList::clear()
{
    entries.clear();
}

void RandomWeightList::addEntry(std::string entryName, int entryWeight)
{
    WeightEntry wE;
    wE.name = entryName;
    wE.weight = entryWeight;
    if(entryWeight > 0)
        entries.push_back(wE);
}

int RandomWeightList::getRandomSlot()
{
    int sum_of_weight = 0;
    for(int i = 0; i < entries.size(); i++)
    {
        sum_of_weight += entries[i].weight;
    }

    int rnd = random(1, sum_of_weight);
    for(int i = 0; i < entries.size(); i++)
    {
        if(rnd <= entries[i].weight)
            return i;
        rnd -= entries[i].weight;
    }
    return 0;
}

std::string RandomWeightList::getRandomName()
{
    int sum_of_weight = 0;
    for(int i = 0; i < entries.size(); i++)
    {
        sum_of_weight += entries[i].weight;
    }

    int rnd = random(1, sum_of_weight);
    for(int i = 0; i < entries.size(); i++)
    {
        if(rnd <= entries[i].weight)
            return entries[i].name;
        rnd -= entries[i].weight;
    }
    return "";
}

void RandomWeightList::printEntries()
{
    int sum_of_weight = 0;
    for(int i = 0; i < entries.size(); i++)
    {
        std::cout << entries[i].name << ": " << sum_of_weight+1 << " - " << sum_of_weight+entries[i].weight << std::endl;
        sum_of_weight += entries[i].weight;
    }
}

float RandomWeightList::getChance(std::string entryName)
{
    float sum_of_weight = 0;
    for(int i = 0; i < entries.size(); i++)
    {
        sum_of_weight += entries[i].weight;
    }
    for(int i = 0; i < entries.size(); i++)
    {
        if(entries[i].name == entryName)
        {
            return (entries[i].weight / sum_of_weight) * 100;
        }
    }

    return -1; // Nothing found, Return impossible.
}
float RandomWeightList::getChance(int entryPosition)
{
    float sum_of_weight = 0;
    for(int i = 0; i < entries.size(); i++)
    {
        sum_of_weight += entries[i].weight;
    }
    for(int i = 0; i < entries.size(); i++)
    {
        if(i == entryPosition)
        {
            return (entries[i].weight / sum_of_weight) * 100;
        }
    }

    return -1; // Nothing found, Return impossible.
}

void RandomWeightList::printChances()
{
    float sum_of_weight = 0;
    for(int i = 0; i < entries.size(); i++)
    {
        sum_of_weight += entries[i].weight;
    }

    for(int i = 0; i < entries.size(); i++)
    {
        std::cout << entries[i].name << ": %" << (entries[i].weight / sum_of_weight) * 100 << std::endl;
    }
}

RandomWeightList randomWeightList;


void fSleep(float time)
{
    sf::sleep(sf::seconds(time));
}

bool booleanize(int num)
{
    if (num <= 0)
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool inbetween(float first, float second, float number)
{
    if (number > first && number < second)
    {
        return true;
    }
    if (number > second && number < first)
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






int random(int minValue, int maxValue)
{
    static std::minstd_rand0 generator (std::chrono::system_clock::now().time_since_epoch().count()); // minstd_rand0 is a standard linear_congruential_engine
    maxValue = maxValue + 1;
    if (minValue == maxValue)
    {
        return minValue;
    }
    int range = (maxValue - minValue);

    int rawRand = (generator() % range);

    return (minValue + rawRand);

}

bool aabb(int pointx, int pointy, int left, int right, int up, int down)
{
    if (pointx >= left && pointx <= right && pointy >= up && pointy <= down)
        return true;
    return false;
}

bool aabb(sf::Vector2f point, int left, int right, int up, int down)
{
    if (point.x >= left && point.x <= right && point.y >= up && point.y <= down)
        return true;
    return false;
}

bool aabb(sf::Vector2i point, int left, int right, int up, int down)
{
    if (point.x >= left && point.x <= right && point.y >= up && point.y <= down)
        return true;
    return false;
}

int stringFindNumber(std::string stringy, std::string term)
{
    size_t tStart;
    tStart = stringy.find(term);
    if (tStart != std::string::npos)
    {
        size_t tEnd;
        std::string output;
        tEnd = stringy.find("]", tStart + 1);
        if (tEnd != std::string::npos)
        {
            int iLength = term.length();
            output.assign(stringy, tStart + iLength, tEnd - (tStart + iLength));
            auto returns = atof(output.c_str());
            if (output == "true")
            {
                returns = 1;
            }
            if (output == "false")
            {
                returns = 0;
            }
            return returns;
        }
    }
    return 0;
}

std::string stringFindString(std::string stringy, std::string term)
{
    std::string returns;
    size_t tStart;
    tStart = stringy.find(term);
    if (tStart != std::string::npos)
    {
        size_t tEnd;
        std::string output;
        tEnd = stringy.find("]", tStart + 1);
        if (tEnd != std::string::npos)
        {
            int iLength = term.length();
            output.assign(stringy, tStart + iLength, tEnd - (tStart + iLength));
            returns = output;
            return returns;
        }
    }
    return "";
}

std::string stringFindStringNpos(std::string stringy, std::string term)
{
    std::string returns;
    size_t tStart;
    tStart = stringy.find(term);
    if (tStart != std::string::npos)
    {
        size_t tEnd;
        std::string output;
        tEnd = stringy.find("]", tStart + 1);

        {
            int iLength = term.length();
            output.assign(stringy, tStart + iLength, tEnd - (tStart + iLength));
            returns = output;
            return returns;
        }
    }
    return "";
}

std::vector<std::string> stringFindElements(std::string source,
                                            std::string seperater)
{

    std::vector<std::string> returns;
    bool firstRun = true;

    bool whileBreaker = false;
    while (whileBreaker == false)
    {
        size_t tStart = 0;
        size_t tEnd = 0;
        tEnd = source.find(seperater, tStart + 1);

        std::string element;

        if (firstRun)
        {
            // Ignoring the later +1, since we don't want it ignoring the
            // first character if it's not a seperater
            element.append(source, tStart, tEnd - tStart);
            firstRun = false;
        }
        else
            // +1 so it ignores the : that was used to find it,
            // -1 since the +1 pushed it over, causing it to grab the 'next' :
            element.append(source, tStart + 1, tEnd - tStart - 1);

        returns.push_back(element);

        if (tEnd >= std::string::npos)
        {
            whileBreaker = true;
        }
    }
    return returns;
}

std::vector<float> numericSplitter(float received, float splitVariable)
{
    con("Beginning That Function, Recieved: ", false);
    con(received);

    std::vector<float> returns;
    for (int i = 0; i < received; i += splitVariable)
    {
        con("Running That Function");
        if (received - i > splitVariable)
            returns.push_back(splitVariable);
        if (received - i <= splitVariable)
            returns.push_back(received - i);
    }
    con("Returning That Function");
    return returns;
}

float percentageBuff(float received)
{
    return received / 100;
}

std::string stringFindChaos(std::string stringy, std::string term,
                            std::string ending)
{
    std::string returns;
    size_t tStart;
    tStart = stringy.find(term);
    if (tStart != std::string::npos)
    {
        size_t tEnd;
        std::string output;
        tEnd = stringy.find(ending, tStart + 1);
        if (tEnd != std::string::npos)
        {
            int iLength = term.length();
            output.assign(stringy, tStart + iLength, tEnd - (tStart + iLength));
            returns = output;
            return returns;
        }
    }

    // If we couldn't find chaos, we just return an empty string
    return "";
}

std::set<std::string> stringFindSetChaos(std::string stringy, std::string term,
                                         std::string ending)
{
    std::set<std::string> returns;
    bool notDone = true;
    // std::cout <<"Working With:" << Stringy << std::endl;
    int goal = 0;
    while (notDone)
    {
        notDone = false;
        size_t tStart = stringy.find(term);
        size_t tEnd;
        std::string output;
        tEnd = stringy.find(ending, tStart + 1);
        if (tEnd != std::string::npos)
        {
            std::cout << tEnd << std::endl;
            sf::sleep(sf::seconds(0.2));
            int iLength = term.length();
            output.assign(stringy, tStart + iLength, tEnd - (tStart + iLength));
            returns.insert(output);
            stringy.replace((tStart + iLength) - 1,
                            (tEnd - ((tStart)+iLength)) + 2, "");
            std::cout << stringy << std::endl;
            if (goal < 500)
            {
                notDone = true;
            }
            goal++;
            //tStart = tEnd;
        }
        else
        {
        }
    }
    for (const auto &Return : returns)
    {
        std::cout << Return << std::endl;
    }
    return returns;
}

std::vector<std::string> stringFindVectorChaos(std::string stringy, std::string term,
                                         std::string ending)
{
    std::vector<std::string> returns;
    bool notDone = true;
    // std::cout <<"Working With:" << Stringy << std::endl;
    int goal = 0;
    while (notDone)
    {
        notDone = false;
        size_t tStart = stringy.find(term);
        size_t tEnd;
        std::string output;
        tEnd = stringy.find(ending, tStart + 1);
        if (tEnd != std::string::npos)
        {
            std::cout << tEnd << std::endl;
            sf::sleep(sf::seconds(0.2));
            int iLength = term.length();
            output.assign(stringy, tStart + iLength, tEnd - (tStart + iLength));
            returns.push_back(output);
            stringy.replace((tStart + iLength) - 1,
                            (tEnd - ((tStart)+iLength)) + 2, "");
            std::cout << stringy << std::endl;
            if (goal < 500)
            {
                notDone = true;
            }
            goal++;
            //tStart = tEnd;
        }
        else
        {
        }
    }
    for (const auto &Return : returns)
    {
        std::cout << Return << std::endl;
    }
    return returns;
}

float percentIs(float value, float percentage)
{
    // Divide the return by 100 for maths.
    if (value == 0)
        value = 0.000000001;
    if (percentage == 0)
        percentage = 0.000000001;

    float percent = percentage / value * 100;
    if (percent == 0)
        return 0.00000000001;
    return percent;
}

bool toggle(bool &boolean)
{
    if (boolean == true)
        boolean = false;
    else if (boolean == false)
        boolean = true;
    return boolean;
}

std::string generateName(int minLength, int maxLength)
{
    // TODO: Add a rememberance for the last added letter,
    // and add a 50% chance to NOT do the same letter again.
    std::string inserter;
    std::string name;
    int length = randz(minLength, maxLength);
    bool vowel = true;
    bool doubleLetter = false;
    if (randz(0, 1) == 1)
        vowel = false;

    for (int i = 0; i != length; i++)
    {
        if (vowel)
        {
            int vowelGen = randz(0, 5);
            if (vowelGen == 0)
                inserter = "A";
            if (vowelGen == 1)
                inserter = "E";
            if (vowelGen == 2)
                inserter = "I";
            if (vowelGen == 3)
                inserter = "O";
            if (vowelGen == 4)
                inserter = "U";
            if (vowelGen == 5)
                inserter = "Y";

            name.append(inserter);

            if (doubleLetter)
            {
                doubleLetter = false;
                vowel = false;
            }
            else if (randz(0, 2) > 0)
            {
                vowel = false;
            }
            else
            {
                doubleLetter = true;
            }
        }
        else
        {
            int consenantGen = randz(0, 24);

            if (consenantGen == 0)
                inserter = "B";
            if (consenantGen == 1)
                inserter = "C";
            if (consenantGen == 2)
                inserter = "D";
            if (consenantGen == 3)
                inserter = "F";
            if (consenantGen == 4)
                inserter = "G";
            if (consenantGen == 5)
                inserter = "H";
            if (consenantGen == 6)
                inserter = "J";
            if (consenantGen == 7)
                inserter = "K";
            if (consenantGen == 8)
                inserter = "L";
            if (consenantGen == 9)
                inserter = "M";
            if (consenantGen == 10)
                inserter = "N";
            if (consenantGen == 11)
                inserter = "P";
            if (consenantGen == 12)
                inserter = "QU";
            //if(ConsenantGen == 12) Inserter = "Q";
            if (consenantGen == 13)
                inserter = "R";
            if (consenantGen == 14)
                inserter = "S";
            if (consenantGen == 15)
                inserter = "T";
            if (consenantGen == 16)
                inserter = "V";
            if (consenantGen == 17)
                inserter = "W";
            if (consenantGen == 18)
                inserter = "X";
            if (consenantGen == 19)
                inserter = "Y";
            if (consenantGen == 20)
                inserter = "Z";

            if (consenantGen == 21)
                inserter = "CH";
            if (consenantGen == 22)
                inserter = "SH";
            if (consenantGen == 23)
                inserter = "TH";
            if (consenantGen == 24)
                inserter = "LL";

            name.append(inserter);

            if (doubleLetter)
            {
                doubleLetter = false;
                vowel = true;
            }
            else if (randz(0, 2) > 0)
                vowel = true;
            else
                doubleLetter = true;
        }
    }
    return name;
}
