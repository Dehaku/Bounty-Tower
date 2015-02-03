#include "InputState.h"
#include "globalvars.h"

#include <SFML/Graphics.hpp>

extern sf::RenderWindow window;

bool plat = true;
InputState inputState;

void InputState::update()
{

    if (gvars::inFocus)
    {

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            leftTime++;
            left = true;
        }
        else
        {
            left = false;
            leftTime = 0;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            rightTime++;
            right = true;
        }
        else
        {
            right = false;
            rightTime = 0;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            upTime++;
            up = true;
        }
        else
        {
            up = false;
            upTime = 0;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            downTime++;
            down = true;
        }
        else
        {
            down = false;
            downTime = 0;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        {
            spaceTime++;
            space = true;
        }
        else
        {
            space = false;
            spaceTime = 0;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
        {
            lshiftTime++;
            lshift = true;
        }
        else
        {
            lshift = false;
            lshiftTime = 0;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
        {
            rshiftTime++;
            rshift = true;
        }
        else
        {
            rshift = false;
            rshiftTime = 0;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
        {
            lctrlTime++;
            lctrl = true;
        }
        else
        {
            lctrl = false;
            lctrlTime = 0;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))
        {
            rctrlTime++;
            rctrl = true;
        }
        else
        {
            rctrl = false;
            rctrlTime = 0;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt))
        {
            laltTime++;
            lalt = true;
        }
        else
        {
            lalt = false;
            laltTime = 0;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::RAlt))
        {
            raltTime++;
            ralt = true;
        }
        else
        {
            ralt = false;
            raltTime = 0;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Period))
        {
            periodTime++;
            period = true;
        }
        else
        {
            period = false;
            periodTime = 0;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Comma))
        {
            commaTime++;
            comma = true;
        }
        else
        {
            comma = false;
            commaTime = 0;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Add))
        {
            plusTime++;
            plus = true;
        }
        else
        {
            plus = false;
            plusTime = 0;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Subtract))
        {
            minusTime++;
            minus = true;
        }
        else
        {
            minus = false;
            minusTime = 0;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
        {
            num1Time++;
            num1 = true;
        }
        else
        {
            num1 = false;
            num1Time = 0;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
        {
            num2Time++;
            num2 = true;
        }
        else
        {
            num2 = false;
            num2Time = 0;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))
        {
            num3Time++;
            num3 = true;
        }
        else
        {
            num3 = false;
            num3Time = 0;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num4))
        {
            num4Time++;
            num4 = true;
        }
        else
        {
            num4 = false;
            num4Time = 0;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num5))
        {
            num5Time++;
            num5 = true;
        }
        else
        {
            num5 = false;
            num5Time = 0;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num6))
        {
            num6Time++;
            num6 = true;
        }
        else
        {
            num6 = false;
            num6Time = 0;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num7))
        {
            num7Time++;
            num7 = true;
        }
        else
        {
            num7 = false;
            num7Time = 0;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num8))
        {
            num8Time++;
            num8 = true;
        }
        else
        {
            num8 = false;
            num8Time = 0;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num9))
        {
            num9Time++;
            num9 = true;
        }
        else
        {
            num9 = false;
            num9Time = 0;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num0))
        {
            num0Time++;
            num0 = true;
        }
        else
        {
            num0 = false;
            num0Time = 0;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad1))
        {
            pad1Time++;
            pad1 = true;
        }
        else
        {
            pad1 = false;
            pad1Time = 0;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad2))
        {
            pad2Time++;
            pad2 = true;
        }
        else
        {
            pad2 = false;
            pad2Time = 0;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad3))
        {
            pad3Time++;
            pad3 = true;
        }
        else
        {
            pad3 = false;
            pad3Time = 0;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad4))
        {
            pad4Time++;
            pad4 = true;
        }
        else
        {
            pad4 = false;
            pad4Time = 0;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad5))
        {
            pad5Time++;
            pad5 = true;
        }
        else
        {
            pad5 = false;
            pad5Time = 0;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad6))
        {
            pad6Time++;
            pad6 = true;
        }
        else
        {
            pad6 = false;
            pad6Time = 0;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad7))
        {
            pad7Time++;
            pad7 = true;
        }
        else
        {
            pad7 = false;
            pad7Time = 0;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad8))
        {
            pad8Time++;
            pad8 = true;
        }
        else
        {
            pad8 = false;
            pad8Time = 0;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad9))
        {
            pad9Time++;
            pad9 = true;
        }
        else
        {
            pad9 = false;
            pad9Time = 0;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Numpad0))
        {
            pad0Time++;
            pad0 = true;
        }
        else
        {
            pad0 = false;
            pad0Time = 0;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            aTime++;
            a = true;
        }
        else
        {
            a = false;
            aTime = 0;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::B))
        {
            bTime++;
            b = true;
        }
        else
        {
            b = false;
            bTime = 0;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
        {
            cTime++;
            c = true;
        }
        else
        {
            c = false;
            cTime = 0;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            dTime++;
            d = true;
        }
        else
        {
            d = false;
            dTime = 0;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
        {
            eTime++;
            e = true;
        }
        else
        {
            e = false;
            eTime = 0;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::F))
        {
            fTime++;
            f = true;
        }
        else
        {
            f = false;
            fTime = 0;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::G))
        {
            gTime++;
            g = true;
        }
        else
        {
            g = false;
            gTime = 0;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::H))
        {
            hTime++;
            h = true;
        }
        else
        {
            h = false;
            hTime = 0;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::I))
        {
            iTime++;
            i = true;
        }
        else
        {
            i = false;
            iTime = 0;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::J))
        {
            jTime++;
            j = true;
        }
        else
        {
            j = false;
            jTime = 0;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::K))
        {
            kTime++;
            k = true;
        }
        else
        {
            k = false;
            kTime = 0;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::L))
        {
            lTime++;
            l = true;
        }
        else
        {
            l = false;
            lTime = 0;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::M))
        {
            mTime++;
            m = true;
        }
        else
        {
            m = false;
            mTime = 0;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::N))
        {
            nTime++;
            n = true;
        }
        else
        {
            n = false;
            nTime = 0;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::O))
        {
            oTime++;
            o = true;
        }
        else
        {
            o = false;
            oTime = 0;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
        {
            pTime++;
            p = true;
        }
        else
        {
            p = false;
            pTime = 0;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
        {
            qTime++;
            q = true;
        }
        else
        {
            q = false;
            qTime = 0;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
        {
            rTime++;
            r = true;
        }
        else
        {
            r = false;
            rTime = 0;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            sTime++;
            s = true;
        }
        else
        {
            s = false;
            sTime = 0;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::T))
        {
            tTime++;
            t = true;
        }
        else
        {
            t = false;
            tTime = 0;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::U))
        {
            uTime++;
            u = true;
        }
        else
        {
            u = false;
            uTime = 0;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::V))
        {
            vTime++;
            v = true;
        }
        else
        {
            v = false;
            vTime = 0;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            wTime++;
            w = true;
        }
        else
        {
            w = false;
            wTime = 0;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
        {
            xTime++;
            x = true;
        }
        else
        {
            x = false;
            xTime = 0;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Y))
        {
            yTime++;
            y = true;
        }
        else
        {
            y = false;
            yTime = 0;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
        {
            zTime++;
            z = true;
        }
        else
        {
            z = false;
            zTime = 0;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Home))
        {
            homeTime++;
            home = true;
        }
        else
        {
            home = false;
            homeTime = 0;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::End))
        {
            endTime++;
            end = true;
        }
        else
        {
            end = false;
            endTime = 0;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tab))
        {
            tabTime++;
            tab = true;
        }
        else
        {
            tab = false;
            tabTime = 0;
        }
        if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
        {
            rmbTime++;
            rmb = true;
        }
        else
        {
            rmb = false;
            rmbTime = 0;
        }
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            lmbTime++;
            lmb = true;
        }
        else
        {
            lmb = false;
            lmbTime = 0;
        }
    }
}

InputState::InputState()
    : num1{}, num1Time{}, num2{}, num2Time{}, num3{}, num3Time{}, num4{},
      num4Time{}, num5{}, num5Time{}, num6{}, num6Time{}, num7{}, num7Time{},
      num8{}, num8Time{}, num9{}, num9Time{}, num0{}, num0Time{}
{
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
    lmb = false;
    rmb = false;
    tab = false;
    home = false;
    end = false;
    anyKey = false;

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
    lmbTime = 0;
    rmbTime = 0;
    tabTime = 0;
    homeTime = 0;
    endTime = 0;
}
