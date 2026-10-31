#include "prison.h"

#include <cassert>
#include <cstdio>

#include <string>
#include <vector>

static constexpr int kNumPrisoners = 500;

static void invalid_strategy(std::string message) {
  printf("%s\n", message.c_str());
  exit(0);
}

int main() {
  int N;
  assert(1 == scanf("%d", &N));

  std::vector<std::vector<int>> strategy = devise_strategy(N);
  if (strategy.size() == 0) {
    invalid_strategy("s is an empty array");
  }
  int x = strategy.size() - 1;
  for (int i = 0; i <= x; ++i) {
    if (static_cast<int>(strategy[i].size()) != N + 1) {
      invalid_strategy("s[i] contains incorrect length");
    }
    if (strategy[i][0] < 0 || strategy[i][0] > 1) {
      invalid_strategy("First element of s[i] is non-binary"); 
    }
    for (int j = 1; j <= N; ++j) {
      if (strategy[i][j] < -2 || strategy[i][j] > x) {
        invalid_strategy("s[i][j] contains incorrect value");
      }
    }
  }

  FILE *log_file = fopen("log.txt","w");

  int A, B;
  while (1 == scanf("%d", &A) && A != -1) {
    assert(1 == scanf("%d", &B));
    bool answer = false;
    int whiteboard = 0;
    for (int i = 0; i < kNumPrisoners && !answer; ++i) {
      int check = strategy[whiteboard][0];
      whiteboard = strategy[whiteboard][check == 0 ? A : B];
      if (whiteboard < 0) {
        answer = true;
        printf("%c\n", "BA"[whiteboard + 2]);
      } else {
        if (i > 0) {
          fprintf(log_file, " ");
        }
        fprintf(log_file, "%d", whiteboard);
      }
    }
    if (!answer) {
      printf("X\n");
    }
    fprintf(log_file, "\n");
    fflush(log_file);
  }
}
