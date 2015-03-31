#include "Camera.h"

void cameraControls()
{
    if (inputState.key[Key::Left])
        gvars::currentx--;
    if (inputState.key[Key::Right])
        gvars::currentx++;
    if (inputState.key[Key::Up])
        gvars::currenty--;
    if (inputState.key[Key::Down])
        gvars::currenty++;
    if (inputState.key[Key::LShift] && inputState.key[Key::Left])
        for (int i = 0; i != 4; i++)
            gvars::currentx--;

    if (inputState.key[Key::LShift] && inputState.key[Key::Right])
        for (int i = 0; i != 4; i++)
            gvars::currentx++;

    if (inputState.key[Key::LShift] && inputState.key[Key::Up])
        for (int i = 0; i != 4; i++)
            gvars::currenty--;

    if (inputState.key[Key::LShift] && inputState.key[Key::Down])
        for (int i = 0; i != 4; i++)
            gvars::currenty++;


    if (inputState.key[Key::Add].time == 1)
        gvars::view1.zoom(2);
    if (inputState.key[Key::Subtract].time == 1)
    gvars::view1.zoom(0.5);

    if (inputState.key[Key::Comma].time == 1 &&
                inputState.key[Key::LShift] &&
                gvars::currentz <= CHUNK_SIZE - 1)
        gvars::currentz++;

    if (inputState.key[Key::Period].time == 1 &&
        inputState.key[Key::LShift] && gvars::currentz >= 1)
        gvars::currentz--;

    if (inputState.key[Key::Comma].time == 1 &&
        inputState.key[Key::RShift] &&
        gvars::currentz <= CHUNK_SIZE - 1)
        gvars::currentz++;

    if (inputState.key[Key::Period].time == 1 &&
        inputState.key[Key::RShift] && gvars::currentz >= 1)
        gvars::currentz--;
}
