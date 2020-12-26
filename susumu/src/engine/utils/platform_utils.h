#pragma once

#include <string>

namespace susumu
{
    class FileDialogs
    {
    public:
        static std::string OpenFile(const char* filter); // returns empty string if canceled
        static std::string SaveFile(const char* filter); // returns empty string if canceled
    };

}

