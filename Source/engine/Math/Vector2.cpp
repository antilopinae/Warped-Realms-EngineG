#include "Vector2.hpp"
#include "Matrix3.hpp"

namespace EngineG::Math{

const Vector2 Vector2::Zero(0.0f, 0.0f);
const Vector2 Vector2::UnitX(1.0f, 0.0f);
const Vector2 Vector2::UnitY(0.0f, 1.0f);
const Vector2 Vector2::NegUnitX(-1.0f, 0.0f);
const Vector2 Vector2::NegUnitY(0.0f, -1.0f);

Vector2 Vector2::Transform(const Vector2& vec, const class Matrix3& mat, float w /*= 1.0f*/)
{
    Vector2 retVal;
    retVal.x = vec.x * mat.mat[0][0] + vec.y * mat.mat[1][0] + w * mat.mat[2][0];
    retVal.y = vec.x * mat.mat[0][1] + vec.y * mat.mat[1][1] + w * mat.mat[2][1];
    //ignore w since we aren't returning a new value for it...
    return retVal;
}

}
