#include "supch.h"
#include "render_command_queue.h"

#define SU_RENDER_TRACE(...) SU_CORE_TRACE(__VA_ARGS__)

namespace susumu
{
    RenderCommandQueue::RenderCommandQueue()
    {
        m_CommandBuffer = new uint8_t[10 * 1024 * 1024]; //10MB buffer
        m_CommandBufferPtr = m_CommandBuffer;
        memset(m_CommandBuffer, 0, 10 * 1024 * 1024);
    }

    RenderCommandQueue::~RenderCommandQueue()
    {
        delete[] m_CommandBuffer;
    }

    void* RenderCommandQueue::Allocate(RenderCommandFn fn, uint32_t size)
    {
        //TODO: aligmnent
        *(RenderCommandFn*)m_CommandBufferPtr = fn;
        m_CommandBufferPtr += sizeof(RenderCommandFn);

        *(int*)m_CommandBufferPtr = size;
        m_CommandBufferPtr += sizeof(uint32_t);

        void* memory = m_CommandBufferPtr;
        m_CommandBufferPtr += size;

        m_CommandCount++;
        return memory;
    }

    void RenderCommandQueue::Execute()
    {
        SU_RENDER_TRACE("RenderCommandQueue::Execute -- {0} commands, {1} bytes", m_CommandCount, (m_CommandBufferPtr - m_CommandBuffer));

        uint8_t* buffer = m_CommandBuffer;

        for (uint32_t i = 0; i < m_CommandCount; i++)
        {
            RenderCommandFn fn = *(RenderCommandFn*)buffer;
            buffer += sizeof(RenderCommandFn);

            uint32_t size = *(uint32_t*)buffer;
            buffer += sizeof(uint32_t);
            fn(buffer);
            buffer += size;
        }

        m_CommandBufferPtr = m_CommandBuffer;
        m_CommandCount = 0;
    }
}
