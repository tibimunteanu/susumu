#pragma once

#include "engine/scene/entity.h"

namespace susumu
{
    class ScriptableEntity
    {
    public:
        virtual ~ScriptableEntity() = default;

        template<typename T>
        T& GetComponent()
        {
            return m_Entity.GetComponent<T>();
        }

    protected:
        virtual void OnCreate() {}
        virtual void OnDestroy() {}
        virtual void OnUpdate(Timestep dt) {}
    private:
        Entity m_Entity;
        friend class Scene;
    };
}
