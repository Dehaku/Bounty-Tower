#include "Camera.h"


void cameraControls()
{
    {
        if (inputState.key[Key::Left] || inputState.key[Key::A])
            gvars::currentx--;
        if (inputState.key[Key::Right] || inputState.key[Key::D])
            gvars::currentx++;
        if (inputState.key[Key::Up] || inputState.key[Key::W])
            gvars::currenty--;
        if (inputState.key[Key::Down] || inputState.key[Key::S])
            gvars::currenty++;
        if (inputState.key[Key::LShift] && (inputState.key[Key::Left] || inputState.key[Key::A]))
            for (int i = 0; i != 4; i++)
                gvars::currentx--;

        if (inputState.key[Key::LShift] && (inputState.key[Key::Right] || inputState.key[Key::D]))
            for (int i = 0; i != 4; i++)
                gvars::currentx++;

        if (inputState.key[Key::LShift] && (inputState.key[Key::Up] || inputState.key[Key::W]))
            for (int i = 0; i != 4; i++)
                gvars::currenty--;

        if (inputState.key[Key::LShift] && (inputState.key[Key::Down] || inputState.key[Key::S]))
            for (int i = 0; i != 4; i++)
                gvars::currenty++;


        if (inputState.key[Key::Add].time == 1)
            gvars::view1.zoom(2);
        if (inputState.key[Key::Subtract].time == 1)
            gvars::view1.zoom(0.5);

        if ( (inputState.key[Key::Comma].time == 1 || inputState.key[Key::Comma].time >= 20)
            &&
                    inputState.key[Key::LShift] &&
                    gvars::currentz < CHUNK_SIZE-1)
            gvars::currentz++;

        if ( (inputState.key[Key::Period].time == 1 || inputState.key[Key::Period].time >= 20) &&
            inputState.key[Key::LShift] && gvars::currentz >= 1)
            gvars::currentz--;

        if ((inputState.key[Key::Comma].time == 1 || inputState.key[Key::Comma].time >= 20) &&
            inputState.key[Key::RShift] &&
            gvars::currentz < CHUNK_SIZE-1)
            gvars::currentz++;

        if ( (inputState.key[Key::Period].time == 1 || inputState.key[Key::Period].time >= 20) &&
            inputState.key[Key::RShift] && gvars::currentz >= 1)
            gvars::currentz--;
    }

}
