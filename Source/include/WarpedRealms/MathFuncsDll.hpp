#pragma once

#include "Export.hpp"

namespace MathFuncs {
// Returns a + b
extern "C" EXPORT double Add(double a, double b);

// Returns a - b
extern "C" EXPORT double Subtract(double a, double b);

// Returns a * b
extern "C" EXPORT double Multiply(double a, double b);

// Returns a / b
// Throws DivideByZeroException if b is 0
extern "C" EXPORT double Divide(double a, double b);
}
