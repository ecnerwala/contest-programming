#include <cstdio>
#include <cstdlib>
#include <vector>
#include "game.h"

namespace {

int read_int() {
  int x;
  if (scanf("%d", &x) != 1) {
    fprintf(stderr, "Error while reading input\n");
    exit(1);
  }
  return x;
}

}  // namespace

int main() {
  int N = read_int();
  int M = read_int();
  int K = read_int();
  std::vector<int> u(M), v(M);
  for (int i = 0; i < M; ++i) {
    u[i] = read_int();
    v[i] = read_int();
  }

  init(N, K);
  int i;
  for (i = 0; i < M; ++i) {
    int answer = add_teleporter(u[i], v[i]);
    if (answer != 0 && answer != 1) {
      i = -1;
      break;
    } else if (answer == 1) {
      break;
    }
  }
  printf("%d\n", i);
  return 0;
}
