#pragma once

#ifndef _FONT_H_
#define _FONT_H_


#include "ft2build.h"
#include FT_FREETYPE_H
#include "stdafx.h"

class Font
{
public:
	Font();
	Font(const char* fileName);
	~Font();
	
	void SetCharSize(long fontWidth, UINT dpi);
	void SetPixelSize(long fontWidth, long fontHeight);
	CharBufferInfo GetCharGlyph(wchar_t ch);

	void LoadFontFile(const char* fileName);
private:
	static const size_t DefaultButterSize = 4 * 1024 * 1024;
	FT_Library m_library;
	FT_Face m_face;
	UINT8* m_buffer;
	UINT8* m_currentPtr;
	std::unordered_map<wchar_t, CharBufferInfo> m_charMap{};
};

#endif // !_FONT_H_