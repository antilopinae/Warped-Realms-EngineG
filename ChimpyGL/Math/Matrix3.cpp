#include "Matrix3.hpp"

namespace EngineG::Math
{

static float m3Ident[3][3] = {{1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}};

const Matrix3 Matrix3::Identity(m3Ident);

}    // namespace EngineG::Math
