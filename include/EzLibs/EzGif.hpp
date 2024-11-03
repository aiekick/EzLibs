#pragma once

#include <fstream>
#include <vector>
#include <cstdint>
#include <algorithm>

namespace ez {
namespace img {

class Gif {
    friend class TestGif;

private:
    uint32_t m_width{};
    uint32_t m_height{};
    std::vector<uint8_t> m_pixels;
    std::vector<uint8_t> m_palette; // RGB

public:
    Gif() : m_palette(256 * 3, 255) {
    }
    ~Gif() = default;

    Gif& setSize(uint32_t vWidth, uint32_t vHeight) {
        m_width = vWidth;
        m_height = vHeight;
        m_pixels.resize(m_width * m_height);
        return *this;
    }

    Gif& clear() {
        m_width = 0U;
        m_height = 0U;
        m_pixels.clear();
        m_palette.assign(256 * 3, 255);
        return *this;
    }

    Gif& setColor(uint8_t index, uint8_t red, uint8_t green, uint8_t blue) {
        if (index < 256) {
            m_palette[index * 3] = red;
            m_palette[index * 3 + 1] = green;
            m_palette[index * 3 + 2] = blue;
        }
        return *this;
    }

    Gif& setPixel(int32_t vX, int32_t vY, uint8_t colorIndex) {
        if (vX >= 0 && vX < static_cast<int32_t>(m_width) && //
            vY >= 0 && vY < static_cast<int32_t>(m_height)) {
            m_pixels[vY * m_width + vX] = colorIndex;
        }
        return *this;
    }

    Gif& save(const std::string& filename) {
        std::ofstream file(filename, std::ios::binary);

        // En-tête GIF et bloc de description logique
        file << "GIF89a";
        m_writeShort(file, m_width);
        m_writeShort(file, m_height);
        file.put(0xF7);  // GCT flag set + 256 colors + sorted
        file.put(0);     // Background color index
        file.put(0);     // Aspect ratio

        // Écrire la palette de couleurs
        file.write(reinterpret_cast<const char*>(m_palette.data()), m_palette.size());

        // Bloc d'image
        file.put(0x2C);                // Image separator
        m_writeShort(file, 0);         // Image left
        m_writeShort(file, 0);         // Image top
        m_writeShort(file, m_width);   // Image width
        m_writeShort(file, m_height);  // Image height
        file.put(0x00);                // No interlace, no sort, no local color table

        // Image data (non compressée pour simplification)
        file.put(8);  // LZW minimum code size (8 bits)
        int dataSize = m_width * m_height + m_height;
        file.put(dataSize);  // Block size (naïve approach without compression)

        // Ajouter des lignes de pixels
        for (uint32_t y = 0; y < m_height; ++y) {
            file.put(m_width);  // Taille de la ligne
            file.write(reinterpret_cast<const char*>(&m_pixels[y * m_width]), m_width);
        }
        file.put(0);  // Fin du bloc d'image

        // Bloc de fin GIF
        file.put(0x3B);

        file.close();
        return *this;
    }

private:
    uint8_t m_getByteFromInt32(int32_t vValue) {
        return static_cast<uint8_t>(std::max(0, std::min(255, vValue)));
    }

    uint8_t m_getByteFromLinearFloat(float vValue) {
        return static_cast<uint8_t>(std::max(0.0f, std::min(1.0f, vValue)) * 255.0f);
    }

    void m_writeShort(std::ofstream& file, uint16_t value) {
        file.put(value & 0xFF);
        file.put((value >> 8) & 0xFF);
    }
};

}  // namespace img
}  // namespace ez
