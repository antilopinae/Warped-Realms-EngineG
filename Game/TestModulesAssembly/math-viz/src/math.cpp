#include <cmath>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

// Very basic and unsafe eval function (FOR DEMONSTRATION ONLY!)
double eval(const std::string& expression) {
  // This is extremely dangerous and should NEVER be used in production.
  // It's only included here for the sake of a complete minimal example.
  // Using a proper expression parser is absolutely crucial for security.
  //  THERE IS NO ERROR CHECKING HERE.

  // This is a placeholder that ONLY supports x*x, or simple arithmetic operations.
  // A complete evaluator requires a full expression parser and evaluator library.
  try {
    return std::stod(expression);  // VERY LIMITED:  Assumes the expression is a single number
  } catch (const std::invalid_argument& e) {
    if (expression == "x*x") return 0;  // VERY LIMITED
    size_t mulPos = expression.find('*');
    if (mulPos != std::string::npos) {
      double left = eval(expression.substr(0, mulPos));
      double right = eval(expression.substr(mulPos + 1));
      return left * right;
    }

    size_t plusPos = expression.find('+');
    if (plusPos != std::string::npos) {
      double left = eval(expression.substr(0, plusPos));
      double right = eval(expression.substr(plusPos + 1));
      return left + right;
    }
    // Add more operators as needed...  but SERIOUSLY, use a library!
    std::cerr << "Eval error: " << e.what() << std::endl;
    throw std::runtime_error("Unsupported expression (use a real parser!)");
  }
}

extern "C" {

// Function to allocate memory in WASM linear memory
void* allocateMemory(size_t size) {
  void* ptr = malloc(size);
  if (ptr == nullptr) {
    std::cerr << "Failed to allocate memory of size: " << size << std::endl;
  }
  return ptr;
}

// Function to free memory allocated in WASM linear memory
void freeMemory(void* ptr) { free(ptr); }

// Function to evaluate a mathematical formula
int evaluateFormula(const char* formula, double xStart, double xEnd, int points, double* results) {
  std::string formulaStr(formula);

  try {
    for (int i = 0; i < points; ++i) {
      double x = xStart + (xEnd - xStart) * static_cast<double>(i) / (points - 1);
      // Replace 'x' in the formula with its numerical value
      std::string xStr = std::to_string(x);
      std::string evaluatedFormula = formulaStr;
      size_t pos = evaluatedFormula.find("x");
      while (pos != std::string::npos) {
        evaluatedFormula.replace(pos, 1, xStr);
        pos = evaluatedFormula.find("x", pos + xStr.length());
      }
      // Naive evaluation (for demonstration only, use a proper expression parser)
      results[i] = eval(evaluatedFormula);
    }
    return 0;  // Success
  } catch (const std::exception& e) {
    std::cerr << "Error evaluating formula: " << e.what() << std::endl;
    return 1;  // Failure
  }
}
}
