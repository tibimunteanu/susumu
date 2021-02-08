#pragma once

#include "engine/renderer/renderer.h"

namespace susumu
{
    class VertexBuffer
    {
    public:
        virtual ~VertexBuffer() = default;

        virtual void SetData(void* buffer, uint32_t size, uint32_t offset = 0) = 0;
        virtual void Bind() const = 0;

        virtual uint32_t GetSize() const = 0;
        virtual RendererID GetRendererID() const = 0;

        static VertexBuffer* Create(uint32_t size = 0);
    };

    class IndexBuffer
    {
    public:
        virtual ~IndexBuffer() = default;

        virtual void SetData(void* buffer, uint32_t size, uint32_t offset = 0) = 0;
        virtual void Bind() const = 0;

        virtual uint32_t GetCount() const = 0;

        virtual uint32_t GetSize() const = 0;
        virtual RendererID GetRendererID() const = 0;

        static IndexBuffer* Create(uint32_t size = 0);
    };
}
