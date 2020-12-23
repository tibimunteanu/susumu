#pragma once

#include "engine/core/base.h"
#include "engine/core/keycodes.h"
#include "engine/core/mousecodes.h"

namespace susumu
{
    class Input
    {
    public:
        static bool IsKeyPressed(KeyCode key);

        static bool IsMouseButtonPressed(MouseCode button);
        static std::pair<float, float> GetMousePosition();
        static float GetMouseX();
        static float GetMouseY();
    };
}
