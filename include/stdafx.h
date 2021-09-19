#pragma once

#ifndef _STDAFX_H_
#define _STDAFX_H_

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#define SAFE_RELEASE(p) if (p) (p)->Release()

#include <Windows.h>
#include <iostream>
#include <unordered_map>

struct CharBufferInfo {
    wchar_t ch;
    unsigned int rows;
    unsigned int width;
    int pitch;
    unsigned char* buffer;
};

inline void ThrowIfFailed(HRESULT hr)
{
    if (FAILED(hr))
    {
        char str[64];
        sprintf_s(str, "HRESULT OF 0x%08X", static_cast<UINT>(hr));
        //OutputDebugStringA(str);
        throw std::runtime_error(str);
    }
}

#endif // !_STDAFX_H_
