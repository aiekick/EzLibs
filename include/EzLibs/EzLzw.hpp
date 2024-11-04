#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <fstream>
#include <unordered_map>

namespace ez {
namespace comp {

// LZW algo :
// https://fr.wikipedia.org/wiki/Lempel-Ziv-Welch

/*
FONCTION LZW_Compresser(Texte, dictionnaire)
    w ← ""
    TANT QUE (il reste des caractères à lire dans Texte) FAIRE
    c ← Lire(Texte)
    p ← Concaténer(w, c)
    SI Existe(p, dictionnaire) ALORS
        w ← p
    SINON
        Ajouter(p, dictionnaire)
        Écrire dictionnaire[w]
        w ← c
    FIN TANT QUE
    Écrire dictionnaire[w]
*/

/*
FONCTION LZW_Décompresser(Code, dictionnaire)
    n ← |Code|
    v ← Lire(Code)
    Écrire dictionnaire[v]
    w ← chr(v)
    POUR i ALLANT DE 2 à n FAIRE
        v ← Lire(Code)
        SI Existe(v, dictionnaire) ALORS
            entrée ← dictionnaire[v]
        SINON entrée ← Concaténer(w, w[0])
        Écrire entrée
        Ajouter(Concaténer(w,entrée[0]), dictionnaire)
        w ← entrée
    FIN POUR
*/

class Lzw {
    friend class TestLzw;

private:
    std::vector<uint8_t> m_datas;

public:
    Lzw() = default;
    ~Lzw() = default;

    Lzw& compressFile(const std::string& vFilePathName) {
        std::ifstream inputFile(vFilePathName, std::ios::binary);
        if (inputFile.is_open()) {
            std::vector<uint8_t> buffer((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
            inputFile.close();
            m_datas = m_compress(buffer);
        }
        return *this;
    }

    Lzw& compresss(void* vBuffer, size_t vBufferLen) {
        std::vector<uint8_t> buffer(static_cast<uint8_t*>(vBuffer), static_cast<uint8_t*>(vBuffer) + vBufferLen);
        m_datas = m_compress(buffer);
        return *this;
    }

    Lzw& compresss(const std::string& vBuffer) {
        m_datas = m_compress(m_stringToVector(vBuffer));
        return *this;
    }

    Lzw& extractFile(const std::string& vFilePathName) {
        std::ifstream inputFile(vFilePathName, std::ios::binary);
        if (inputFile.is_open()) {
            std::vector<uint8_t> buffer((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
            inputFile.close();
            m_datas = m_extract(buffer);
        }
        return *this;
    }

    Lzw& extract(void* vBuffer, size_t vBufferLen) {
        std::vector<uint8_t> buffer(static_cast<uint8_t*>(vBuffer), static_cast<uint8_t*>(vBuffer) + vBufferLen);
        m_datas = m_extract(buffer);
        return *this;
    }

    Lzw& extract(const std::string& vBuffer) {
        std::vector<uint8_t> buffer(vBuffer.begin(), vBuffer.end());
        m_datas = m_extract(buffer);
        return *this;
    }

    std::vector<uint8_t> getDatas() {
        return m_datas;
    }

    std::string getDatasToString() {
        return m_vectorToString(m_datas);
    }

    Lzw& save(const std::string& vFilePathName) {
        std::ofstream file(vFilePathName, std::ios::binary);
        if (!file) {
            throw std::runtime_error("Impossible d'ouvrir le fichier.");
        }
        auto dataSize = static_cast<uint32_t>(m_datas.size());
        file.write(reinterpret_cast<const char*>(&dataSize), sizeof(dataSize));
        file.write(reinterpret_cast<const char*>(m_datas.data()), m_datas.size());
        file.close();
        return *this;
    }

private:
    static std::vector<uint8_t> m_stringToVector(const std::string& str) {
        return {str.begin(), str.end()};
    }

    static std::string m_vectorToString(const std::vector<uint8_t>& vec) {
        return {vec.begin(), vec.end()};
    }

    static std::vector<uint8_t> m_compress(const std::vector<uint8_t>& data) {
        std::unordered_map<std::string, uint16_t> dictionary;
        for (uint16_t i = 0; i < 256; ++i) {
            dictionary[std::string(1, i)] = i;
        }
        std::string w;
        std::vector<uint8_t> result;
        uint16_t code = 256;
        for (uint8_t c : data) {
            std::string wc = w + static_cast<char>(c);
            if (dictionary.count(wc)) {
                w = wc;
            } else {
                result.push_back(dictionary[w]);
                if (code < 4096) {
                    dictionary[wc] = code++;
                }
                w = std::string(1, c);
            }
        }
        if (!w.empty()) result.push_back(dictionary[w]);
        return result;
    }

    static std::vector<uint8_t> m_extract(const std::vector<uint8_t>& data) {
        std::unordered_map<uint16_t, std::string> dictionary;
        for (uint16_t i = 0; i < 256; ++i) {
            dictionary[i] = std::string(1, i);
        }
        uint16_t oldCode = data[0];
        std::string s = dictionary[oldCode];
        std::vector<uint8_t> result(s.begin(), s.end());
        uint16_t code = 256;
        for (size_t i = 1; i < data.size(); ++i) {
            uint16_t newCode = data[i];
            std::string entry;
            if (dictionary.count(newCode)) {
                entry = dictionary[newCode];
            } else if (newCode == code) {
                entry = dictionary[oldCode] + dictionary[oldCode][0];
            }
            result.insert(result.end(), entry.begin(), entry.end());
            if (code < 4096) {
                dictionary[code++] = dictionary[oldCode] + entry[0];
            }
            oldCode = newCode;
        }
        return result;
    }
};

}  // namespace comp
}  // namespace ez
