#pragma once

namespace EngineG::Math {

typedef struct vec2 {
    union {
        struct {
            float x;
            float y;
        };
        float asArray[2];
    };

    float& operator[](int index) { return asArray[index]; }
} vec2_t;

typedef struct vec3 {
    union {
        struct {
            float x;
            float y;
            float z;
        };
        float asArray[3];
    };

    float& operator[](int index) { return asArray[index]; }

} vec3_t;

}  // namespace EngineG::Math
