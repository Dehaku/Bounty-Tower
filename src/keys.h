#ifndef __KEYS_H__
#define __KEYS_H__

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

// TODO: Add a bool for Any, and have all keys make it true, Press Any Key
// TODO: Add a Holding integer for each one If Key.Qheld > 10
extern bool plat;

extern sf::RenderWindow App;

extern bool InFocus;

class Ke
{
public:
    bool AnyKey;
    bool LMB;
    int LMBTime;
    bool RMB;
    int RMBTime;
    bool left;
    int leftTime;
    bool right;
    int rightTime;
    bool up;
    int upTime;
    bool down;
    int downTime;
    bool space;
    int spaceTime;
    bool lctrl;
    int lctrlTime;
    bool rctrl;
    int rctrlTime;
    bool lshift;
    int lshiftTime;
    bool rshift;
    int rshiftTime;
    bool tab;
    int tabTime;
    bool period;
    int periodTime;
    bool comma;
    int commaTime;
    bool plus;
    int plusTime;
    bool minus;
    int minusTime;
    bool lalt;
    int laltTime;
    bool ralt;
    int raltTime;
    bool home;
    int homeTime;
    bool end;
    int endTime;
    bool pad1;
    int pad1Time;
    bool pad2;
    int pad2Time;
    bool pad3;
    int pad3Time;
    bool pad4;
    int pad4Time;
    bool pad5;
    int pad5Time;
    bool pad6;
    int pad6Time;
    bool pad7;
    int pad7Time;
    bool pad8;
    int pad8Time;
    bool pad9;
    int pad9Time;
    bool pad0;
    int pad0Time;

    bool num1;
    int num1Time;
    bool num2;
    int num2Time;
    bool num3;
    int num3Time;
    bool num4;
    int num4Time;
    bool num5;
    int num5Time;
    bool num6;
    int num6Time;
    bool num7;
    int num7Time;
    bool num8;
    int num8Time;
    bool num9;
    int num9Time;
    bool num0;
    int num0Time;

    bool a;
    bool b;
    bool c;
    bool d;
    bool e;
    bool f;
    bool g;
    bool h;
    bool i;
    bool j;
    bool k;
    bool l;
    bool m;
    bool n;
    bool o;
    bool p;
    bool q;
    bool r;
    bool s;
    bool t;
    bool u;
    bool v;
    bool w;
    bool x;
    bool y;
    bool z;
    int aTime;
    int bTime;
    int cTime;
    int dTime;
    int eTime;
    int fTime;
    int gTime;
    int hTime;
    int iTime;
    int jTime;
    int kTime;
    int lTime;
    int mTime;
    int nTime;
    int oTime;
    int pTime;
    int qTime;
    int rTime;
    int sTime;
    int tTime;
    int uTime;
    int vTime;
    int wTime;
    int xTime;
    int yTime;
    int zTime;

    void Update();

    Ke();
};
extern Ke Key;

#endif
