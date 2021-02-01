#pragma once

#include "susumu.h"

class GameLayer : public susumu::Layer
{
public:
    GameLayer();
    virtual ~GameLayer() = default;

    void OnAttach() override;
    void OnDetach() override;
    void OnUpdate(susumu::Timestep dt) override;
    void OnEvent(susumu::Event& e) override;
    void OnImGuiRender() override;
};
