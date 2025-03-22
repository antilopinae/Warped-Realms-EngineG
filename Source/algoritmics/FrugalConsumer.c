#include <stdio.h>
#include <stdlib.h>

// UV под номером 10980.

double min(double v1, double v2) {
  return (v1 < v2) ? v1 : v2;
}

double solve_k(int num[], double price[], int num_shemes,
  double unit_price, int num_items) {
  double best, result;
  int i;
  if (num_items == 0) {
    return 0;
  } else {
    result = solve_k(num, price, num_shemes, unit_price, num_items - 1);
    best = result + unit_price;
    for (i = 0; i < num_shemes; i++) {
      result = solve_k(num, price, num_shemes, unit_price, num_items - num[i]);
      best = min(best, result + price[i]);
    }
    return best;
  }
}

double solve(int num[], double price[], int num_shemes, double unit_price, int num_items) {

}


int main(void) {

  return 0;
}