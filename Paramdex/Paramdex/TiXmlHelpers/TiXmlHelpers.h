#pragma once
#include <string>

namespace TiXmlHelpers
{
	// Tinyxml2 uses UTF-8 encoded strings, so we need to convert between std::wstring and std::string, and vice versa.
    inline std::string WToS(const std::wstring& w) 
    {
        return std::string(w.begin(), w.end());
    }

    inline std::wstring SToW(const char* s) 
    {
        if (!s) return L"";
        std::string str(s);
        return std::wstring(str.begin(), str.end());
    }
}