#include "supch.h"
#include "render_command_queue.h"

#define SU_RENDER_TRACE(...) SU_CORE_TRACE(__VA_ARGS__)

namespace susumu
{
    RenderCommandQueue::RenderCommandQueue()
    {
        m_CommandBuffer = new unsigned char[10 * 1024 * 1024]; //10MB buffer
        m_CommandBufferPtr = m_CommandBuffer;
        memset(m_CommandBuffer, 0, 10 * 1024 * 1024);
    }

    RenderCommandQueue::~RenderCommandQueue()
    {
        delete[] m_CommandBuffer;
    }

    void* RenderCommandQueue::Allocate(RenderCommandFn fn, unsigned int size)
    {
        //TODO: aligmnent
        *(RenderCommandFn*)m_CommandBufferPtr = fn;
        m_CommandBufferPtr += sizeof(RenderCommandFn);

        *(int*)m_CommandBufferPtr = size;
        m_CommandBufferPtr += sizeof(unsigned int);

        void* memory = m_CommandBufferPtr;
        m_CommandBufferPtr += size;

        m_CommandCount++;
        return memory;
    }

    void RenderCommandQueue::Execute()
    {
        SU_RENDER_TRACE("RenderCommandQueue::Execute -- {0} commands, {1} bytes", m_CommandCount, (m_CommandBufferPtr - m_CommandBuffer));

        unsigned char* buffer = m_CommandBuffer;

        for (unsigned int i = 0; i < m_CommandCount; i++)
        {
            RenderCommandFn fn = *(RenderCommandFn*)buffer;
            buffer += sizeof(RenderCommandFn);

            unsigned int size = *(unsigned int*)buffer;
            buffer += sizeof(unsigned int);
            fn(buffer);
            buffer += size;
        }

        m_CommandBufferPtr = m_CommandBuffer;
        m_CommandCount = 0;
    }
}
