#ifndef __KEYS_H__
#define __KEYS_H__

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "Game.h"




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

    bool Update(){

        if(InFocus)
        {



        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){leftTime++;left = true;} else { /*Key.*/left = false;leftTime = 0;}
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){ /*Key.*/rightTime++;right = true;} else { /*Key.*/right = false;rightTime = 0;}
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){ /*Key.*/upTime++;up = true;} else { /*Key.*/up = false;upTime = 0;}
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){ /*Key.*/downTime++;down = true;} else { /*Key.*/down = false;downTime = 0;}
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)){ /*Key.*/spaceTime++;space = true;} else { /*Key.*/space = false;spaceTime = 0;}
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)){ /*Key.*/lshiftTime++;lshift = true;} else { /*Key.*/lshift = false;lshiftTime = 0;}
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::RShift)){ /*Key.*/rshiftTime++;rshift = true;} else { /*Key.*/rshift = false;rshiftTime = 0;}
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)){ /*Key.*/lctrlTime++;lctrl = true;} else { /*Key.*/lctrl = false;lctrlTime = 0;}
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::RControl)){ /*Key.*/rctrlTime++;rctrl = true;} else { /*Key.*/rctrl = false;rctrlTime = 0;}
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt)){ /*Key.*/laltTime++;lalt = true;} else { /*Key.*/lalt = false;laltTime = 0;}
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::RAlt)){ /*Key.*/raltTime++;ralt = true;} else { /*Key.*/ralt = false;raltTime = 0;}
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Period)){  /*Key.*/periodTime++;period = true;} else { /*Key.*/period = false;periodTime = 0;}
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Comma)){ /*Key.*/commaTime++;comma = true;} else { /*Key.*/comma = false;commaTime = 0;}
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Add)){ /*Key.*/plusTime++;plus = true;} else { /*Key.*/plus = false;plusTime = 0;}
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Subtract)){ /*Key.*/minusTime++;minus  = true;} else { /*Key.*/minus = false;minusTime = 0;}

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1)){ /*Key.*/num1Time++; num1 = true;} else { /*Key.*/ num1 = false; num1Time = 0;}
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2)){ /*Key.*/num2Time++; num2 = true;} else { /*Key.*/ num2 = false; num2Time = 0;}
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3)){ /*Key.*/num3Time++; num3 = true;} else { /*Key.*/ num3 = false; num3Time = 0;}
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4)){ /*Key.*/num4Time++; num4 = true;} else { /*Key.*/ num4 = false; num4Time = 0;}
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num5)){ /*Key.*/num5Time++; num5 = true;} else { /*Key.*/ num5 = false; num5Time = 0;}
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num6)){ /*Key.*/num6Time++; num6 = true;} else { /*Key.*/ num6 = false; num6Time = 0;}
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num7)){ /*Key.*/num7Time++; num7 = true;} else { /*Key.*/ num7 = false; num7Time = 0;}
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num8)){ /*Key.*/num8Time++; num8 = true;} else { /*Key.*/ num8 = false; num8Time = 0;}
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num9)){ /*Key.*/num9Time++; num9 = true;} else { /*Key.*/ num9 = false; num9Time = 0;}
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num0)){ /*Key.*/num0Time++; num0 = true;} else { /*Key.*/ num0 = false; num0Time = 0;}

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad1)){ /*Key.*/pad1Time++;pad1 = true;} else { /*Key.*/pad1 = false;pad1Time = 0;}
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad2)){ /*Key.*/pad2Time++;pad2 = true;} else { /*Key.*/pad2 = false;pad2Time = 0;}
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad3)){ /*Key.*/pad3Time++;pad3 = true;} else { /*Key.*/pad3 = false;pad3Time = 0;}
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad4)){ /*Key.*/pad4Time++;pad4 = true;} else { /*Key.*/pad4 = false;pad4Time = 0;}
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad5)){ /*Key.*/pad5Time++;pad5 = true;} else { /*Key.*/pad5 = false;pad5Time = 0;}
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad6)){ /*Key.*/pad6Time++;pad6 = true;} else { /*Key.*/pad6 = false;pad6Time = 0;}
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad7)){ /*Key.*/pad7Time++;pad7 = true;} else { /*Key.*/pad7 = false;pad7Time = 0;}
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad8)){ /*Key.*/pad8Time++;pad8 = true;} else { /*Key.*/pad8 = false;pad8Time = 0;}
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad9)){ /*Key.*/pad9Time++;pad9 = true;} else { /*Key.*/pad9 = false;pad9Time = 0;}
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad0)){ /*Key.*/pad0Time++;pad0 = true;} else { /*Key.*/pad0 = false;pad0Time = 0;}

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)){ /*Key.*/aTime++;a = true;} else { /*Key.*/a = false;aTime = 0;}
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::B)){ /*Key.*/bTime++;b = true;} else { /*Key.*/b = false;bTime = 0;}
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::C)){ /*Key.*/cTime++;c = true;} else { /*Key.*/c = false;cTime = 0;}
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)){ /*Key.*/dTime++;d = true;} else { /*Key.*/d = false;dTime = 0;}
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)){ /*Key.*/eTime++;e = true;} else { /*Key.*/e = false;eTime = 0;}
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::F)){ /*Key.*/fTime++;f = true;} else { /*Key.*/f = false;fTime = 0;}
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::G)){ /*Key.*/gTime++;g = true;} else { /*Key.*/g = false;gTime = 0;}
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::H)){ /*Key.*/hTime++;h = true;} else { /*Key.*/h = false;hTime = 0;}
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::I)){ /*Key.*/iTime++;i = true;} else { /*Key.*/i = false;iTime = 0;}
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::J)){ /*Key.*/jTime++;j = true;} else { /*Key.*/j = false;jTime = 0;}
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::K)){ /*Key.*/kTime++;k = true;} else { /*Key.*/k = false;kTime = 0;}
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::L)){ /*Key.*/lTime++;l = true;} else { /*Key.*/l = false;lTime = 0;}
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::M)){ /*Key.*/mTime++;m = true;} else { /*Key.*/m = false;mTime = 0;}
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::N)){ /*Key.*/nTime++;n = true;} else { /*Key.*/n = false;nTime = 0;}
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::O)){ /*Key.*/oTime++;o = true;} else { /*Key.*/o = false;oTime = 0;}
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::P)){ /*Key.*/pTime++;p = true;} else { /*Key.*/p = false;pTime = 0;}
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)){ /*Key.*/qTime++;q = true;} else { /*Key.*/q = false;qTime = 0;}
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)){ /*Key.*/rTime++;r = true;} else { /*Key.*/r = false;rTime = 0;}
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)){ /*Key.*/sTime++;s = true;} else { /*Key.*/s = false;sTime = 0;}
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::T)){ /*Key.*/tTime++;t = true;} else { /*Key.*/t = false;tTime = 0;}
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::U)){ /*Key.*/uTime++;u = true;} else { /*Key.*/u = false;uTime = 0;}
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::V)){ /*Key.*/vTime++;v = true;} else { /*Key.*/v = false;vTime = 0;}
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)){ /*Key.*/wTime++;w = true;} else { /*Key.*/w = false;wTime = 0;}
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)){ /*Key.*/xTime++;x = true;} else { /*Key.*/x = false;xTime = 0;}
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Y)){ /*Key.*/yTime++;y = true;} else { /*Key.*/y = false;yTime = 0;}
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)){ /*Key.*/zTime++;z = true;} else { /*Key.*/z = false;zTime = 0;}
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Home)){ /*Key.*/homeTime++;home = true;} else { /*Key.*/home = false;homeTime = 0;}
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::End)){ /*Key.*/endTime++;end = true;} else { /*Key.*/end = false;endTime = 0;}
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tab)){ tabTime++;tab = true;} else {tab = false; tabTime = 0;}
        if (sf::Mouse::isButtonPressed(sf::Mouse::Right)){RMBTime++;RMB = true;} else{RMB = false; RMBTime = 0;}
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){LMBTime++;LMB = true;} else{LMB = false; LMBTime = 0;}



        }
    }

    Ke(){
        left = false;
        up = false;
        down = false;
        right = false;
        lshift = false;
        rshift = false;
        period = false;
        comma = false;
        lctrl = false;
        rctrl = false;
        lalt = false;
        ralt = false;
        plus = false;
        minus = false;
        pad1 = false;
        pad2 = false;
        pad3 = false;
        pad4 = false;
        pad5 = false;
        pad6 = false;
        pad7 = false;
        pad8 = false;
        pad9 = false;
        pad0 = false;
        space = false;
        a = false;
        b = false;
        c = false;
        d = false;
        e = false;
        f = false;
        g = false;
        h = false;
        i = false;
        j = false;
        k = false;
        l = false;
        m = false;
        n = false;
        o = false;
        p = false;
        q = false;
        r = false;
        s = false;
        t = false;
        u = false;
        v = false;
        w = false;
        x = false;
        y = false;
        z = false;
        LMB = false;
        RMB = false;
        tab = false;
        home = false;
        end = false;
        AnyKey = false;

        leftTime = 0;
        upTime = 0;
        downTime = 0;
        rightTime = 0;
        lshiftTime = 0;
        rshiftTime = 0;
        periodTime = 0;
        commaTime = 0;
        lctrlTime = 0;
        rctrlTime = 0;
        laltTime = 0;
        raltTime = 0;
        plusTime = 0;
        minusTime = 0;
        pad1Time = 0;
        pad2Time = 0;
        pad3Time = 0;
        pad4Time = 0;
        pad5Time = 0;
        pad6Time = 0;
        pad7Time = 0;
        pad8Time = 0;
        pad9Time = 0;
        pad0Time = 0;
        spaceTime = 0;
        aTime = 0;
        bTime = 0;
        cTime = 0;
        dTime = 0;
        eTime = 0;
        fTime = 0;
        gTime = 0;
        hTime = 0;
        iTime = 0;
        jTime = 0;
        kTime = 0;
        lTime = 0;
        mTime = 0;
        nTime = 0;
        oTime = 0;
        pTime = 0;
        qTime = 0;
        rTime = 0;
        sTime = 0;
        tTime = 0;
        uTime = 0;
        vTime = 0;
        wTime = 0;
        xTime = 0;
        yTime = 0;
        zTime = 0;
        LMBTime = 0;
        RMBTime = 0;
        tabTime = 0;
        homeTime = 0;
        endTime = 0;
    }
};
Ke Key;



#endif
