#pragma once

#include "engine/core/base.h"

namespace susumu
{
    enum class TextureFormat
    {
        None = 0,
        RGB = 1,
        RGBA = 2
    };

    class Texture
    {
    public:
        virtual ~Texture() = default;
    };

    class Texture2D : public Texture
    {
    public:
        static Texture2D* Create(TextureFormat format, uint32_t width, uint32_t height);

        virtual TextureFormat GetFormat() const = 0;
        virtual uint32_t GetWidth() const = 0;
        virtual uint32_t GetHeight() const = 0;
    };
}
