#pragma once

#include <fstream>
#include <vector>
#include <cstdint>
#include <stdexcept>
#include <string>

namespace ez {
namespace img {

/*
Bitmap picture file saver

ez::img::Bmp()
    .setSize(10,10) // width, height 10
    .setPixel(0,0,255,100,200) // byte form
    .setPixel(5,5,0.5,0.2,0.8) // linear float form
    .save("test.bmp"); // save to file test.bmp
*/

class Bmp {
    friend class TestBmp;

private:
    uint32_t m_width{};
    uint32_t m_height{};
    std::vector<uint8_t> m_pixels;

public:
    Bmp() = default;
    ~Bmp() = default;

    Bmp& setSize(uint32_t vWidth, uint32_t vHeight) {
        m_width = std::move(vWidth);
        m_height = std::move(vHeight);
        m_pixels.resize(m_width * m_height * 3U);
        return *this;
    }

    Bmp& clear() {
        m_width = 0U;
        m_height = 0U;
        m_pixels.clear();
        return *this;
    }

    Bmp& setPixel(int32_t vX, int32_t vY, int32_t vRed, int32_t vGreen, int32_t vBlue) {
        auto x = static_cast<uint32_t>(vX);
        auto y = static_cast<uint32_t>(vY);
        if (x >= 0 && y < m_width && y >= 0U && x < m_height) {
            size_t index = static_cast<size_t>((vY * m_width + vX) * 3);
            // BMP save color in BGR
            m_pixels[index++] = m_getByteFromInt32(vBlue);
            m_pixels[index++] = m_getByteFromInt32(vGreen);
            m_pixels[index] = m_getByteFromInt32(vRed);
        }
        return *this;
    }

    Bmp& setPixel(int32_t vX, int32_t vY, float vRed, float vGreen, float vBlue) {
        return setPixel(vX,  //
                        vY, 
                        m_getByteFromLinearFloat(vRed),
                        m_getByteFromLinearFloat(vGreen),
                        m_getByteFromLinearFloat(vBlue));
    }

    // Sauvegarde l'image en tant que fichier BMP
    Bmp& save(const std::string& filename) {
        std::ofstream file(filename, std::ios::binary);
        if (!file) {
            throw std::runtime_error("Impossible d'ouvrir le fichier.");
        }

        int paddingSize = (4 - (m_width * 3) % 4) % 4;
        int fileSize = 54 + (m_width * m_height * 3) + (m_height * paddingSize);

        // En-t�te de fichier BMP
        uint8_t fileHeader[14] = {
            'B',
            'M',  // Signature
            0,
            0,
            0,
            0,  // Taille du fichier
            0,
            0,
            0,
            0,  // R�serv�
            54,
            0,
            0,
            0  // Offset vers les donn�es d'image
        };

        // Ins�rer la taille du fichier dans l'en-t�te
        fileHeader[2] = fileSize;
        fileHeader[3] = fileSize >> 8;
        fileHeader[4] = fileSize >> 16;
        fileHeader[5] = fileSize >> 24;

        // En-t�te d'information BMP
        uint8_t infoHeader[40] = {
            40, 0, 0, 0,  // Taille de l'en-t�te d'information
            0,  0, 0, 0,  // Largeur
            0,  0, 0, 0,  // Hauteur
            1,  0,        // Nombre de plans (1)
            24, 0,        // Bits par pixel (24 bits)
            0,  0, 0, 0,  // Compression (aucune)
            0,  0, 0, 0,  // Taille de l'image (non compress�e)
            0,  0, 0, 0,  // R�solution horizontale (pixels par m�tre)
            0,  0, 0, 0,  // R�solution verticale (pixels par m�tre)
            0,  0, 0, 0,  // Couleurs dans la palette
            0,  0, 0, 0   // Couleurs importantes
        };

        // Ins�rer la largeur et la hauteur dans l'en-t�te d'information
        infoHeader[4] = m_width;
        infoHeader[5] = m_width >> 8;
        infoHeader[6] = m_width >> 16;
        infoHeader[7] = m_width >> 24;
        infoHeader[8] = m_height;
        infoHeader[9] = m_height >> 8;
        infoHeader[10] = m_height >> 16;
        infoHeader[11] = m_height >> 24;

        // �crire les en-t�tes dans le fichier
        file.write(reinterpret_cast<char*>(fileHeader), sizeof(fileHeader));
        file.write(reinterpret_cast<char*>(infoHeader), sizeof(infoHeader));

        // �crire les donn�es des pixels
        for (int32_t y = static_cast<int32_t>(m_height - 1); y >= 0; --y) {  // BMP commence du bas vers le haut
            for (int32_t x = 0; x < static_cast<int32_t>(m_width); ++x) {
                size_t index = static_cast<size_t>((y * m_width + x) * 3);
                file.write(reinterpret_cast<char*>(&m_pixels[index]), 3);
            }
            // Ajout de padding si n�cessaire
            uint8_t padding[3] = {0, 0, 0};
            file.write(reinterpret_cast<char*>(padding), paddingSize);
        }

        file.close();
        return *this;
    }

private:
    uint8_t m_getByteFromInt32(int32_t vValue) {
        if (vValue < 0) {
            vValue = 0;
        }
        if (vValue > 255) {
            vValue = 255;
        }
        return static_cast<uint8_t>(vValue);
    }
    uint8_t m_getByteFromLinearFloat(float vValue) {
        if (vValue < 0.0f) {
            vValue = 0.0f;
        }
        if (vValue > 1.0f) {
            vValue = 1.0f;
        }
        return static_cast<uint8_t>(vValue * 255.0f);
    }
};

}  // namespace img
}  // namespace ez
