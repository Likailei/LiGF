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

CharBufferInfo Font::LoadCharBboxBitmapToBuffer(wchar_t ch)
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
		CharBufferInfo cbi
		{	ch,
			m_face->glyph->bitmap.rows,
			m_face->glyph->bitmap.width,
			m_face->glyph->bitmap.pitch,
			m_face->glyph->metrics.horiAdvance,
			m_face->glyph->metrics.vertAdvance,
			start
		};
		m_charMap.insert({ ch, cbi });

		return cbi;
	}
}

StringBufferInfo Font::GetHoriString(const std::wstring str)
{
	std::vector<CharBufferInfo> cbis;
	for (auto ch : str) {
		cbis.push_back(LoadCharBboxBitmapToBuffer(ch));
	}

	StringBufferInfo sbi{0};
	return sbi;
	//for (auto cbi : cbis) {
	//	sbi.
	//}
}

TextureInfo Font::GetTextureRGBA(wchar_t ch, std::vector<UINT8>& data)
{
	TextureInfo ti{};

	CharBufferInfo cbi = LoadCharBboxBitmapToBuffer(ch);
	for (int y = 0; y < cbi.rows; y++) {
		for (int x = 0; x < cbi.width; x++) {
			UINT8 gray = *(cbi.buffer + cbi.width*y + x);
			// 60,179,113
			UINT8 r = (UINT8)(gray / 255.0 * 60);
			UINT8 g = (UINT8)(gray / 255.0 * 179);
			UINT8 b = (UINT8)(gray / 255.0 * 113);

			data.push_back(r);
			data.push_back(g);
			data.push_back(b);
			data.push_back(gray);
		}
	}
	ti.format = DXGI_FORMAT_R8G8B8A8_UNORM;
	ti.pixelWidth = 4;
	ti.width = cbi.width;
	ti.height = cbi.rows;
	ti.rowPitch = ti.width * ti.pixelWidth;
	return ti;
}
