#pragma once
#include "GT/Renderer/Framebuffer.h"
#include "Font.h"
#include <cwchar>
#include "glm/glm.hpp"
namespace GT
{

	struct Glyph {
		glm::ivec2   Size;      // Size of glyph
		glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
		unsigned int Advance;   // Horizontal offset to advance to next glyph
	};
	struct Rect
	{
		glm::vec2 TL;
		glm::vec2 BR;
	};

	std::map<char, Glyph> Glyphs;

	class FontAtlas
	{
	public:
		FontAtlas() = default;
		FontAtlas(uint32_t width,uint32_t height);
		~FontAtlas() = default;

		void Init(const std::filesystem::path& fontPath, uint32_t fontSize);
		void AddGlyph(wchar_t glyph, const GlyphBitmap& glyphbitmap);

		Rect GetGlyphUV(wchar_t glyph) const;

		uint32_t GetAtlasTextureID() const { return m_Framebuffer->GetColorAttachmentRendererID(); }
		const std::unordered_map<wchar_t, Rect>& GetGlyphUVs() const { return m_GlyphUVs; }
	private:
		uint32_t m_AtlasWidth, m_AtlasHeight;
		Ref<Framebuffer> m_Framebuffer;
		Font m_Font;
		std::unordered_map<wchar_t, Rect> m_GlyphUVs;
	};
}