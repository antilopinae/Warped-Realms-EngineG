#include <stdio.h>
#include <stdlib.h>

#define SIZE 1000

int max(int v1, int v2){
  return (v1 > v2) ? v1 : v2;
}

// Memoize
int solve(char outcome1[], char outcome2[], int goals1[], int goals2[], int i, int j, int memo[SIZE+1][SIZE+1]) {
  int first, second, third, fourth;
  if(memo[i][j] != -1) {
    return memo[i][j];
  }
  if(i == 0 || j == 0) {
    memo[i][j] = 0;
    return 0;
  }
  if ((
      outcome1[i] == 'W' && outcome2[j] == 'L'
      && goals1[i] > goals2[j]
    ) ||
    (
      outcome1[i] == 'L' && outcome2[j] == 'W'
      && goals1[i] < goals2[j]
    ))
  {
    first = solve(outcome1, outcome2, goals1, goals2, i-1, j-1, memo) + goals1[i] + goals2[j];
  }
  else
  {
    first = 0;
  }

  second = solve(outcome1, outcome2, goals1, goals2, i-1, j-1, memo);
  third = solve(outcome1, outcome2, goals1, goals2, i-1, j, memo);
  fourth = solve(outcome1, outcome2, goals1, goals2, i, j-1, memo);
  memo[i][j] = max(first, max(second, max(third, fourth)));
  return memo[i][j];
}

// Dynamic
int solve2(char outcome1[], char outcome2[], int goals1[], int goals2[], int n) {
  int i, j, k;
  int first, second, third, fourth;
  static int previous[SIZE+1], current[SIZE+1];
  for (i = 0; i <= n; i++) {
    previous[i] = 0;
  }
  for(i = 0; i <= n; i++) {
    for(j = 0; j <= n; j++) {
      if ((
          outcome1[i] == 'W' && outcome2[j] == 'L'
          && goals1[i] > goals2[j]
        ) ||
        (
          outcome1[i] == 'L' && outcome2[j] == 'W'
          && goals1[i] < goals2[j]
        ))
      {
        first = previous[j-1]+goals1[i]+goals2[j];
      }
      else
      {
        first = 0;
      }
      second = previous[j-1];
      third = previous[j];
      fourth = previous[j];
      current[j] = max(first, max(second, max(third, fourth)));
    }
    for (k=0; k <= SIZE; k++) {
      previous[k] = current[k];
    }
  }
  return current[n];
}


int main() {
  int i, j, n, result;
  char outcome1[SIZE+1], outcome2[SIZE+1];
  int goals1[SIZE+1], goals2[SIZE+1];
  static int memo[SIZE+1][SIZE+1];
  scanf("%d", &n);
  for(i = 1; i <= n; i++) {
    scanf("%c", &outcome1[i]);
  }
  for(i = 1; i <= n; i++) {
    scanf("%d", &goals1[i]);
  }
  for(i = 1; i <= n; i++) {
    scanf("%c", &outcome2[i]);
  }
  for(i = 1; i <= n; i++) {
    scanf("%d", &goals2[i]);
  }
  for(i = 1; i <= SIZE; i++) {
    for(j=0; j<SIZE; j++) {
      memo[i][j] = 0;
    }
  }
//  result = solve(outcome1, outcome2, goals1, goals2, n, n, memo);
  result = solve2(outcome1, outcome2, goals1, goals2, n);
  printf("%d\n", result);
  return 0;
}