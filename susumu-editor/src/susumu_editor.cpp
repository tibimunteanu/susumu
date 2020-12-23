#include <susumu.h>
#include <engine/core/startup.h>

#include "editor_layer.h"

namespace susumu
{
    class SusumuEditor : public App
    {
    public:
        SusumuEditor()
            : App("Susumu Editor")
        {
            PushLayer(new EditorLayer());
        }

        ~SusumuEditor()
        {

        }
    };

    App* CreateApp()
    {
        return new SusumuEditor();
    }
}
