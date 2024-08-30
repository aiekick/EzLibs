#pragma once
#include <vector>

#include <EzMath/EzMath.hpp>

namespace ez {

class Gui {
public:

    struct Vertex {
        fvec2 pos;
        fvec2 uv;
        fvec4 color;
        Vertex() = default;
        Vertex(fvec2 vPos, fvec2 vUV, fvec4 vColor) : pos(vPos), uv(vUV), color(vColor) {
        }
    };

public:
    bool Begin(const std::string& vLabel, const bool* vOpened = nullptr, const fvec2& vSize = fvec2(0.0f, 0.0f)) {
    
    }

    void End() {
    
    }
};

}  // namespace ez
