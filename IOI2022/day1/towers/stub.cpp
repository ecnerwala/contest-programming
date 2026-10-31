#include "towers.h"

#include <cassert>
#include <cstdio>

#include <vector>

int main() {
  int N, Q;
  assert(2 == scanf("%d %d", &N, &Q));
  std::vector<int> H(N);
  for (int i = 0; i < N; ++i) {
    assert(1 == scanf("%d", &H[i]));
  }
  init(N, H);

  for (int i = 0; i < Q; ++i) {
    int L, R, D;
    assert(3 == scanf("%d %d %d", &L, &R, &D));
    printf("%d\n", max_towers(L, R, D));
  }
  return 0;
}
