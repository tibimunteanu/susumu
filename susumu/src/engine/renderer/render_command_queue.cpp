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

    void RenderCommandQueue::Submit(const RenderCommand& command)
    {
        auto ptr = m_CommandBuffer;

        memcpy(m_CommandBuffer, &command, sizeof(RenderCommand));
        m_CommandBufferPtr += sizeof(RenderCommand);
        m_RenderCommandCount++;
    }

    void RenderCommandQueue::SubmitCommand(RenderCommandFn fn, void* params, unsigned int size)
    {
        unsigned char*& buffer = m_CommandBufferPtr;
        memcpy(buffer, &fn, sizeof(RenderCommandFn));
        buffer += sizeof(RenderCommandFn);
        memcpy(buffer, params, size);
        buffer += size;

        auto totalSize = sizeof(RenderCommandFn) + size;
        auto padding = totalSize % 16; //16-byte alignment
        buffer += padding;
        m_RenderCommandCount++;
    }

    void RenderCommandQueue::Execute()
    {
        SU_RENDER_TRACE("RenderCommandQueue::Execute -- {0} commands, {1} bytes", m_RenderCommandCount, (m_CommandBufferPtr - m_CommandBuffer));

        unsigned char* buffer = m_CommandBuffer;

        for (int i = 0; i < m_RenderCommandCount; i++)
        {
            RenderCommandFn fn = *(RenderCommandFn*)buffer;
            buffer += sizeof(RenderCommandFn);
            buffer += (*fn)(buffer);

            auto padding = (int)buffer % 16;
            buffer += padding;
        }

        m_CommandBufferPtr = m_CommandBuffer;
        m_RenderCommandCount = 0;
    }
}
