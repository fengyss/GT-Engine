#include "gtpch.h"
#include "Font.h"

#include <ft2build.h>
#include FT_FREETYPE_H

namespace GT
{

    struct Font::Impl {
        FT_Library library = nullptr;
        FT_Face    face = nullptr;
    };

    Font::Font()
        : d(new Impl)
    {
        if (FT_Init_FreeType(&d->library)) {
            d->library = nullptr;
			GT_CORE_CRITICAL("Failed to initialize FreeType Library");
        }
    }

    Font::~Font() {
        Destroy();
    }

    void Font::Destroy() {
        if (d) {
            if (d->face) {
                FT_Done_Face(d->face);
                d->face = nullptr;
            }
            if (d->library) {
                FT_Done_FreeType(d->library);
                d->library = nullptr;
            }
            delete d;
            d = nullptr;
        }
    }

    // ---- Move ----
    Font::Font(Font&& other) noexcept
        : d(other.d), m_pixelHeight(other.m_pixelHeight)
    {
        other.d = nullptr;
    }

    Font& Font::operator=(Font&& other) noexcept {
        if (this != &other) {
            Destroy();
            d = other.d;
            m_pixelHeight = other.m_pixelHeight;
            other.d = nullptr;
        }
        return *this;
    }

    // ---- Load ----
    bool Font::Load(const std::filesystem::path& path) {
        if (!d || !d->library)
            return false;

        if (d->face) {
            FT_Done_Face(d->face);
            d->face = nullptr;
        }

        if (FT_New_Face(d->library, path.string().c_str(), 0, &d->face))
            return false;

        FT_Select_Charmap(d->face, FT_ENCODING_UNICODE);
        return true;
    }

    // ---- Size ----
    bool Font::SetPixelHeight(int height) {
        if (!d || !d->face)
            return false;

        if (FT_Set_Pixel_Sizes(d->face, 0, height))
            return false;

        m_pixelHeight = height;
        return true;
    }

    // ---- Rasterize ----
    GlyphBitmap Font::Rasterize(uint32_t codepoint) {
        GlyphBitmap result{};
        result.codepoint = codepoint;

        if (!d || !d->face)
            return result;

        FT_UInt glyphIndex = FT_Get_Char_Index(d->face, codepoint);
        if (!glyphIndex)
            return result;

        if (FT_Load_Glyph(d->face, glyphIndex, FT_LOAD_DEFAULT))
            return result;

        if (FT_Render_Glyph(d->face->glyph, FT_RENDER_MODE_NORMAL))
            return result;

        FT_Bitmap& bmp = d->face->glyph->bitmap;

        result.width = bmp.width;
        result.height = bmp.rows;
        result.bearingX = d->face->glyph->bitmap_left;
        result.bearingY = d->face->glyph->bitmap_top;
        result.advanceX = int(d->face->glyph->advance.x >> 6);

        if (result.width == 0 || result.height == 0)
            return result;

        size_t bytes = size_t(result.width) * result.height;
        result.data = new uint8_t[bytes];
        std::memcpy(result.data, bmp.buffer, bytes);

        return result;
    }
}