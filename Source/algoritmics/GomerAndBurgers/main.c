#include <stdio.h>
#include <stdlib.h>

int max(int v1, int v2){
  if(v1>v2){
    return v1;
  } else
    return v2;
}

unsigned long long total_calls = 0;

int solve_t(int m, int n, int t, int memo[]) {
  int first, second;
  if(memo[t] != -2) {
    return memo[t];
  }
  total_calls++;
  if (t == 0){
    memo[t] = 0;
    return 0;
  }
  if (t >= m)
    first = solve_t(m, n, t-m, memo);
  else
    first = -1;
  if (t >= n)
    second = solve_t(n, m, t-n, memo);
  else
    second = -1;
  if (first == -1 && second == -1){
    memo[t] = -1;
    return -1;
  }
  else {
    memo[t] = max(first, second) + 1;
    return memo[t];
  }
}


#define SIZE 10000

void solve(int m, int n, int t) {
  int result, i;
  int memo[SIZE];
  for (i = 0; i <= t; i++) {
    memo[i] = -2;
  }
  result = solve_t(m, n, t, memo);
  if (result >= 0)
    printf("%d\n", result);
  else {
    i = t - 1;
    result = solve_t(m, n, i, memo);
    while (result == -1) {
      i--;
      result = solve_t(m, n, i, memo);
    }
    printf("%d %d\n", result, t - i);
  }
}

void solve2(int m, int n, int t)
{
  int result, i, first, second;
  int dp[SIZE];
  dp[0] = 0;
  for (i = 1; i <= t; i++)
  {
    if (i >= m)
      first = dp[i-m];
    else
      first = -1;
    if (i >= n)
      second = dp[i-n];
    else
      second = -1;
    if (first == -1 && second == -1)
      dp[i] = -1;
    else
      dp[i] = max(first, second) + 1;
  }
  result = dp[t];
  if (result >= 0)
    printf("%d\n", result);
  else
  {
    i = t - 1;
    result = dp[i];
    while (result == -1)
    {
      i--;
      result = dp[i];
    }
    printf("%d %d\n", result, t-i);
  }
}


int main(void) {
  int m, n, t;
  while (scanf("%d%d%d", &m, &n, &t) != -1)
    // solve(m, n, t); // Memoizing
      solve2(m, n, t); // Dynamic programming
  return 0;
}
