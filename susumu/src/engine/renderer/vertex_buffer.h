#pragma once

#include "engine/renderer/renderer.h"

namespace susumu
{
    class VertexBuffer
    {
    public:
        virtual ~VertexBuffer() = default;

        virtual void SetData(void* buffer, unsigned int size, unsigned int offset = 0) = 0;
        virtual void Bind() const = 0;

        virtual unsigned int GetSize() const = 0;
        virtual RendererID GetRendererID() const = 0;

        static VertexBuffer* Create(unsigned int size = 0);
    };
}
