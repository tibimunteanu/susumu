#pragma once

#include "engine/renderer/render_command_queue.h"
#include "engine/renderer/renderer_api.h"

namespace susumu
{
    class Renderer
    {
    public:
        typedef void(*RenderCommandFn)(void*);

        //commands
        static void Clear();
        static void Clear(float r, float g, float b, float a = 1.0f);
        static void SetClearColor(float r, float g, float b, float a);

        static void DrawIndexed(unsigned int count);

        static void ClearMagenta();

        static void Init();

        static void* Submit(RenderCommandFn fn, unsigned int size)
        {
            return s_Instance->m_CommandQueue.Allocate(fn, size);
        }

        void WaitAndRender();

        inline static Renderer& Get() { return *s_Instance; }
    private:
        static Renderer* s_Instance;

        RenderCommandQueue m_CommandQueue;
    };
}

#define SU_RENDER_PASTE2(a, b) a ## b
#define SU_RENDER_PASTE(a, b) SU_RENDER_PASTE2(a, b)
#define SU_RENDER_UNIQUE(x) SU_RENDER_PASTE(x, __LINE__)

#define SU_RENDER(code) \
    struct SU_RENDER_UNIQUE(SURenderCommand) \
    {\
        static void Execute(void*)\
        {\
            code\
        }\
    };\
	{\
		auto mem = ::susumu::Renderer::Submit(SU_RENDER_UNIQUE(SURenderCommand)::Execute, sizeof(SU_RENDER_UNIQUE(SURenderCommand)));\
		new (mem) SU_RENDER_UNIQUE(SURenderCommand)();\
	}\

#define SU_RENDER_1(arg0, code) \
	do {\
    struct SU_RENDER_UNIQUE(SURenderCommand) \
    {\
		SU_RENDER_UNIQUE(SURenderCommand)(typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0) \
		: arg0(arg0) {}\
		\
        static void Execute(void* argBuffer)\
        {\
			auto& arg0 = ((SU_RENDER_UNIQUE(SURenderCommand)*)argBuffer)->arg0;\
            code\
        }\
		\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0;\
    };\
	{\
		auto mem = ::susumu::Renderer::Submit(SU_RENDER_UNIQUE(SURenderCommand)::Execute, sizeof(SU_RENDER_UNIQUE(SURenderCommand)));\
		new (mem) SU_RENDER_UNIQUE(SURenderCommand)(arg0);\
	} } while(0)

#define SU_RENDER_2(arg0, arg1, code) \
    struct SU_RENDER_UNIQUE(SURenderCommand) \
    {\
		SU_RENDER_UNIQUE(SURenderCommand)(typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0,\
											typename ::std::remove_const<typename ::std::remove_reference<decltype(arg1)>::type>::type arg1) \
		: arg0(arg0), arg1(arg1) {}\
		\
        static void Execute(void* argBuffer)\
        {\
			auto& arg0 = ((SU_RENDER_UNIQUE(SURenderCommand)*)argBuffer)->arg0;\
			auto& arg1 = ((SU_RENDER_UNIQUE(SURenderCommand)*)argBuffer)->arg1;\
            code\
        }\
		\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0;\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg1)>::type>::type arg1;\
    };\
	{\
		auto mem = ::susumu::Renderer::Submit(SU_RENDER_UNIQUE(SURenderCommand)::Execute, sizeof(SU_RENDER_UNIQUE(SURenderCommand)));\
		new (mem) SU_RENDER_UNIQUE(SURenderCommand)(arg0, arg1);\
	}\

#define SU_RENDER_3(arg0, arg1, arg2, code) \
    struct SU_RENDER_UNIQUE(SURenderCommand) \
    {\
		SU_RENDER_UNIQUE(SURenderCommand)(typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0,\
											typename ::std::remove_const<typename ::std::remove_reference<decltype(arg1)>::type>::type arg1,\
											typename ::std::remove_const<typename ::std::remove_reference<decltype(arg2)>::type>::type arg2) \
		: arg0(arg0), arg1(arg1), arg2(arg2) {}\
		\
        static void Execute(void* argBuffer)\
        {\
			auto& arg0 = ((SU_RENDER_UNIQUE(SURenderCommand)*)argBuffer)->arg0;\
			auto& arg1 = ((SU_RENDER_UNIQUE(SURenderCommand)*)argBuffer)->arg1;\
			auto& arg2 = ((SU_RENDER_UNIQUE(SURenderCommand)*)argBuffer)->arg2;\
            code\
        }\
		\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0;\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg1)>::type>::type arg1;\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg2)>::type>::type arg2;\
    };\
	{\
		auto mem = ::susumu::Renderer::Submit(SU_RENDER_UNIQUE(SURenderCommand)::Execute, sizeof(SU_RENDER_UNIQUE(SURenderCommand)));\
		new (mem) SU_RENDER_UNIQUE(SURenderCommand)(arg0, arg1, arg2);\
	}\

#define SU_RENDER_4(arg0, arg1, arg2, arg3, code) \
    struct SU_RENDER_UNIQUE(SURenderCommand) \
    {\
		SU_RENDER_UNIQUE(SURenderCommand)(typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0,\
											typename ::std::remove_const<typename ::std::remove_reference<decltype(arg1)>::type>::type arg1,\
											typename ::std::remove_const<typename ::std::remove_reference<decltype(arg2)>::type>::type arg2,\
											typename ::std::remove_const<typename ::std::remove_reference<decltype(arg3)>::type>::type arg3)\
		: arg0(arg0), arg1(arg1), arg2(arg2), arg3(arg3) {}\
		\
        static void Execute(void* argBuffer)\
        {\
			auto& arg0 = ((SU_RENDER_UNIQUE(SURenderCommand)*)argBuffer)->arg0;\
			auto& arg1 = ((SU_RENDER_UNIQUE(SURenderCommand)*)argBuffer)->arg1;\
			auto& arg2 = ((SU_RENDER_UNIQUE(SURenderCommand)*)argBuffer)->arg2;\
			auto& arg3 = ((SU_RENDER_UNIQUE(SURenderCommand)*)argBuffer)->arg3;\
            code\
        }\
		\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0;\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg1)>::type>::type arg1;\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg2)>::type>::type arg2;\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg3)>::type>::type arg3;\
    };\
	{\
		auto mem = Renderer::Submit(SU_RENDER_UNIQUE(SURenderCommand)::Execute, sizeof(SU_RENDER_UNIQUE(SURenderCommand)));\
		new (mem) SU_RENDER_UNIQUE(SURenderCommand)(arg0, arg1, arg2, arg3);\
	}

#define SU_RENDER_S(code) auto self = this;\
	SU_RENDER_1(self, code)

#define SU_RENDER_S1(arg0, code) auto self = this;\
	SU_RENDER_2(self, arg0, code)

#define SU_RENDER_S2(arg0, arg1, code) auto self = this;\
	SU_RENDER_3(self, arg0, arg1, code)

#define SU_RENDER_S3(arg0, arg1, arg2, code) auto self = this;\
	SU_RENDER_4(self, arg0, arg1, arg2, code)
