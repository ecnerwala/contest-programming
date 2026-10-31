#include "circuit.h"

#include <cassert>
#include <cstdio>

#include <vector>

int main() {
  int N, M, Q;
  assert(3 == scanf("%d %d %d", &N, &M, &Q));
  std::vector<int> P(N + M), A(M);
  for (int i = 0; i < N + M; ++i) {
    assert(1 == scanf("%d", &P[i]));
  }
  for (int j = 0; j < M; ++j) {
    assert(1 == scanf("%d", &A[j]));
  }
  init(N, M, P, A);

  for (int i = 0; i < Q; ++i) {
    int L, R;
    assert(2 == scanf("%d %d", &L, &R));
    printf("%d\n", count_ways(L, R));
  }
  return 0;
}
