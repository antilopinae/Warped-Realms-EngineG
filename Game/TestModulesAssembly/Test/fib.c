#include <emscripten/emscripten.h>
#include <stdio.h>

int EMSCRIPTEN_KEEPALIVE fib(int n) {
  if (n == 0) {
    return 0;
  } else {
    if ((n == -1) || (n == 1)) {
      return 1;
    } else {
      if (n > 0) {
        return fib(n - 1) + fib(n - 2);
      } else {
        return fib(n + 2) - fib(n + 1);
      }
    }
  }
}

int main() {
  printf("fib(10) = %d\n", fib(10));
  return 0;
}
