#pragma once

/*
MIT License

Copyright (c) 2014-2024 Stephane Cuillerdier (aka aiekick)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <vector>
#include <cstdio>
#include <vector>
#include <cstdint>
#include <fstream>
#include <algorithm>
#include <unordered_map>

#include "EzLzw.hpp"

namespace ez {
namespace img {

// Gif file format :
// https://www.loc.gov/preservation/digital/formats/fdd/fdd000133.shtml
// https://www.matthewflickinger.com/lab/whatsinagif/index.html
// https://www.matthewflickinger.com/lab/whatsinagif/gif_explorer.asp

class Gif {
    friend class TestGif;

private:
    uint32_t m_width{};
    uint32_t m_height{};
    std::vector<uint8_t> m_pixels;
    std::vector<uint8_t> m_palette;  // RGB
    std::unordered_map<std::string, uint16_t> m_dicoComp;  // Dictionnaire de compression


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
        if (vX >= 0 && vX < static_cast<int32_t>(m_width) && vY >= 0 && vY < static_cast<int32_t>(m_height)) {
            m_pixels[vY * m_width + vX] = colorIndex;
        }
        return *this;
    }

    Gif& save(const std::string& filename) {
        FILE* file = fopen_s(filename.c_str(), "wb");
        if (!file) return *this;

        // En-tête GIF et bloc de description logique
        fwrite("GIF89a", 1, 6, file);
        m_writeShort(file, m_width);
        m_writeShort(file, m_height);
        fputc(0xF7, file);  // GCT flag set + 256 colors + sorted
        fputc(0, file);     // Background color index
        fputc(0, file);     // Aspect ratio

        // Écrire la palette de couleurs
        fwrite(m_palette.data(), sizeof(uint8_t), m_palette.size(), file);

        // Bloc d'image
        m_writeImageBlock(file);
#if 0
        m_writeUnComrpessedDataBlock(file);
#else
        m_writeCompressedDataBlock(file);
#endif

        // Bloc de fin GIF
        fputc(0x3B, file);

        fclose(file);
        return *this;
    }

private:
    uint8_t m_getByteFromInt32(int32_t vValue) {
        return static_cast<uint8_t>(std::max(0, std::min(255, vValue)));
    }

    uint8_t m_getByteFromLinearFloat(float vValue) {
        return static_cast<uint8_t>(std::max(0.0f, std::min(1.0f, vValue)) * 255.0f);
    }

    void m_writeShort(FILE* file, uint16_t value) {
        fputc(value & 0xFF, file);
        fputc((value >> 8) & 0xFF, file);
    }

    void m_writeImageBlock(FILE* vpFile) {
        fputc(0x2C, vpFile);           // Image separator
        m_writeShort(vpFile, 0);       // Image left
        m_writeShort(vpFile, 0);       // Image top
        m_writeShort(vpFile, m_width);  // Image width
        m_writeShort(vpFile, m_height);  // Image height
        fputc(0x00, vpFile);             // No interlace, no sort, no local color table
    }

    void m_writeUnComrpessedDataBlock(FILE* vpFile) {
        fputc(8, vpFile);                        // LZW minimum code size (8 bits)
        uint32_t dataSize = m_width * m_height;  // Naïve approach without compression
        fputc(dataSize, vpFile);                 // Block size
        for (uint32_t y = 0; y < m_height; ++y) {
            fputc(m_width, vpFile);  // Taille de la ligne
            fwrite(&m_pixels[y * m_width], 1, m_width, vpFile);
        }
        fputc(0, vpFile);  // Fin du bloc d'image
    }

    void m_writeCompressedDataBlock(FILE* vpFile) {
        int minCodeSize = 8;
        fputc(minCodeSize, vpFile);  // LZW minimum code size (8 bits)
        auto compressedData = packToGifBlocks(m_compress(m_pixels, minCodeSize), minCodeSize);
        fputc(0xFF, vpFile);  // clear code
        fputc(compressedData.size(), vpFile);  // Taille du bloc compressé
        fwrite(compressedData.data(), 1, compressedData.size(), vpFile);
        fputc(0, vpFile);  // Fin du bloc d'image
    }

    std::vector<uint16_t> m_compress(const std::vector<uint8_t>& vDatas, int minCodeSize) {
        std::vector<uint16_t> result;
        uint16_t code = 1 << minCodeSize;  // Commence après le clear code et end code
        uint16_t clearCode = 1 << minCodeSize;
        uint16_t endOfInfoCode = clearCode + 1;

        // Initialiser le dictionnaire avec les codes de base (valeurs de 0 à 255)
        m_dicoComp.clear();
        for (int i = 0; i < 256; ++i) {
            m_dicoComp[std::string(1, static_cast<char>(i))] = i;
        }

        // Ajouter le clear code au début des données
        result.push_back(clearCode);

        std::string p;
        p += static_cast<char>(vDatas.at(0));

        for (size_t idx = 1; idx < vDatas.size(); ++idx) {
            char c = static_cast<char>(vDatas.at(idx));
            std::string pc = p + c;

            if (m_dicoComp.find(pc) != m_dicoComp.end()) {
                p = pc;
            } else {
                result.push_back(m_dicoComp[p]);
                if (code < 4096) {  // Limite de la table de codes LZW
                    m_dicoComp[pc] = code++;
                }
                p = c;
            }
        }

        // Ajouter le dernier code
        result.push_back(m_dicoComp[p]);

        // Ajouter le code de fin
        result.push_back(endOfInfoCode);

        return result;
    }

    std::vector<uint8_t> packToGifBlocks(const std::vector<uint16_t>& compressedData, int minCodeSize) {
        std::vector<uint8_t> result;
        int bitPos = 0;
        uint32_t buffer = 0;
        int codeSize = minCodeSize + 1;

        std::vector<uint8_t> block;
        block.reserve(255);  // Pré-allocation pour les blocs GIF (taille max de 255)

        for (auto code : compressedData) {
            buffer |= (code << bitPos);
            bitPos += codeSize;

            while (bitPos >= 8) {
                block.push_back(static_cast<uint8_t>(buffer & 0xFF));
                buffer >>= 8;
                bitPos -= 8;

                // Lorsque le bloc atteint 255 octets, on l'ajoute au résultat
                if (block.size() == 255) {
                    result.push_back(static_cast<uint8_t>(block.size()));
                    result.insert(result.end(), block.begin(), block.end());
                    block.clear();
                }
            }

            // Augmenter la taille des codes si nécessaire
            if (code == (1 << codeSize) - 1 && codeSize < 12) {
                ++codeSize;
            }
        }

        // Ajouter les bits restants dans le buffer
        if (bitPos > 0) {
            block.push_back(static_cast<uint8_t>(buffer & 0xFF));
        }

        // Ajouter le dernier bloc si non vide
        if (!block.empty()) {
            result.push_back(static_cast<uint8_t>(block.size()));
            result.insert(result.end(), block.begin(), block.end());
        }

        // Bloc de fin GIF avec une taille de zéro
        result.push_back(0x00);

        return result;
    }
};


}  // namespace img
}  // namespace ez
