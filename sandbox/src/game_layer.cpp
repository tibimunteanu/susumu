#include "game_layer.h"

#include <imgui/imgui.h>

GameLayer::GameLayer()
    : Layer("GameLayer")
{
}

void GameLayer::OnAttach()
{
}

void GameLayer::OnDetach()
{
}

void GameLayer::OnUpdate(susumu::Timestep dt)
{
    susumu::Renderer::Clear(0.2f, 0.3f, 0.8f, 1.0f);
}

void GameLayer::OnEvent(susumu::Event& e)
{
}

void GameLayer::OnImGuiRender()
{
}

