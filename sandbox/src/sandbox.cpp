#include <susumu.h>
#include <engine/core/startup.h>

#include "sandbox_2d.h"

class Sandbox : public susumu::App
{
public:
    Sandbox()
    {
        PushLayer(new Sandbox2D());
    }

    ~Sandbox()
    {

    }
};

susumu::App* susumu::CreateApp()
{
    return new Sandbox();
}
