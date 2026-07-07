#include "gtpch.h"
#include "FontAtlas.h"

namespace GT
{
	FontAtlas::FontAtlas(uint32_t width, uint32_t height)
	{
	}
	void FontAtlas::Init(const std::filesystem::path& fontPath, uint32_t fontSize)
	{
	}
	void FontAtlas::AddGlyph(wchar_t glyph, const GlyphBitmap& glyphbitmap)
	{
	}
	Rect FontAtlas::GetGlyphUV(wchar_t glyph) const
	{
		return Rect();
	}
}