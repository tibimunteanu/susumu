#include <susumu.h>

class ExampleLayer : public susumu::Layer
{
public:
    ExampleLayer()
        : Layer("Example")
    {
    }

    void OnUpdate() override
    {
        //SU_INFO("ExampleLayer::OnUpdate");
    }

    void OnEvent(susumu::Event& event) override
    {
        //SU_TRACE("ExampleLayer::OnEvent {0}", event);
    }
};

class Sandbox : public susumu::App
{
public:
    Sandbox()
    {
        PushLayer(new ExampleLayer());
    }

    ~Sandbox()
    {

    }
};

susumu::App* susumu::CreateApp()
{
    return new Sandbox();
}
