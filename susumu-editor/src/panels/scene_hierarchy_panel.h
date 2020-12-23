#pragma once

#include "engine/core/base.h"
#include "engine/core/log.h"
#include "engine/scene/scene.h"
#include "engine/scene/entity.h"

#include <imgui/imgui.h>

namespace susumu
{
    class SceneHierarchyPanel
    {
    public:
        SceneHierarchyPanel() = default;
        SceneHierarchyPanel(const Ref<Scene>& context);

        void SetContext(const Ref<Scene>& context);

        void OnImGuiRender();
    private:
        void DrawEntityNode(Entity entity);
        void DrawComponents(Entity entity);
    private:
        Ref<Scene> m_Context;
        Entity m_SelectionContext;
    };
}
