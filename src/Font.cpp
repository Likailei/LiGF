#include "Font.h"

Font::Font()
{
	ThrowIfFailed(FT_Init_FreeType(&m_library));
	m_buffer = new UINT8[DefaultButterSize];
	m_currentPtr = m_buffer;
}

Font::Font(const char* fileName)
{
	ThrowIfFailed(FT_Init_FreeType(&m_library));
	LoadFontFile(fileName);
	m_buffer = new UINT8[DefaultButterSize];
	m_currentPtr = m_buffer;
}

void Font::LoadFontFile(const char* fileName)
{
	ThrowIfFailed(FT_New_Face(m_library, fileName, 0, &m_face));
}

Font::~Font()
{
	FT_Done_Face(m_face);
	FT_Done_FreeType(m_library);
	if(m_buffer) delete[] m_buffer;
}

void Font::SetCharSize(long charPointSize, UINT dpi)
{
	ThrowIfFailed(FT_Set_Char_Size(m_face, charPointSize*64, 0, dpi, 0));
}

void Font::SetPixelSize(long charPixelWidth, long charPixelHeight)
{
	ThrowIfFailed(FT_Set_Pixel_Sizes(m_face, charPixelWidth, charPixelHeight));
}

CharBufferInfo Font::GetCharGlyph(wchar_t ch)
{
	auto search = m_charMap.find(ch);
	if (search != m_charMap.end()) {
		return search->second;
	}
	else {
		ThrowIfFailed(FT_Load_Char(m_face, ch, FT_LOAD_RENDER));
		size_t needSize = static_cast<size_t>(m_face->glyph->bitmap.width * m_face->glyph->bitmap.rows);
		size_t freeSize = m_buffer + DefaultButterSize - m_currentPtr;

		if (needSize > freeSize) {
			memset(m_buffer, 0, DefaultButterSize);
			m_currentPtr = m_buffer;
			m_charMap.clear();
		}
		UINT8* start = m_currentPtr;
		memcpy(m_currentPtr, m_face->glyph->bitmap.buffer, needSize);
		m_currentPtr += needSize;
		CharBufferInfo cbi{ ch, m_face->glyph->bitmap.rows, m_face->glyph->bitmap.width, m_face->glyph->bitmap.pitch, start };
		m_charMap.insert({ ch, cbi });
		
		return cbi;
	}
}