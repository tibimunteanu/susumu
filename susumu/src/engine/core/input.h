#pragma once

#include "engine/core/keycodes.h"
#include "engine/core/mousecodes.h"

#include <glm/glm.hpp>

namespace susumu
{
    class Input
    {
    public:
        static bool IsKeyPressed(KeyCode key);

        static bool IsMouseButtonPressed(MouseCode button);
        static glm::vec2 GetMousePosition();
        static float GetMouseX();
        static float GetMouseY();
    };
}
