#pragma once
#pragma once

namespace GT
{

    struct GlyphBitmap {
        uint32_t codepoint = 0;

        int width = 0;
        int height = 0;

        int bearingX = 0; // left offset
        int bearingY = 0; // top offset (positive = up)
        int advanceX = 0; // pixel advance

        uint8_t* data = nullptr; // 8-bit grayscale, row-major
    };

    enum class FontError {
        OK = 0,
        LibraryInitFailed,
        FaceLoadFailed,
        SetPixelSizeFailed,
        GlyphNotFound,
        RasterizeFailed,
    };

    class Font {
    public:
        Font();
        ~Font();

        // 禁止拷贝（FT 资源不可拷贝）
        Font(const Font&) = delete;
        Font& operator=(const Font&) = delete;

        // 移动允许
        Font(Font&&) noexcept;
        Font& operator=(Font&&) noexcept;

        // ---- 对外接口 ----
        bool Load(const std::filesystem::path& path);
        void Unload();

        bool SetPixelHeight(int height);
        int  GetPixelHeight() const { return m_pixelHeight; }

        GlyphBitmap Rasterize(uint32_t codepoint);

        bool IsValid() const;

    private:
        void Destroy();

    private:
        struct Impl;          // PIMPL（彻底隔离 FT 头文件）
        Impl* d = nullptr;

        int m_pixelHeight = 0;
    };
}