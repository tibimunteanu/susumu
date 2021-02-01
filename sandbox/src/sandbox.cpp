#include <susumu.h>
#include <engine/core/startup.h>

#include "game_layer.h"

class Sandbox : public susumu::App
{
public:
    Sandbox()
    {
        PushLayer(new GameLayer());
    }

    ~Sandbox()
    {

    }
};

susumu::App* susumu::CreateApp()
{
    return new Sandbox();
}
