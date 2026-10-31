#include "mosaic.h"
#include <cassert>
#include <cstdio>
#include <vector>

int main() {
  int N;
  assert(1 == scanf("%d", &N));
  std::vector<int> X(N), Y(N);
  for (int i = 0; i < N; i++)
    assert(1 == scanf("%d", &X[i]));
  for (int i = 0; i < N; i++)
    assert(1 == scanf("%d", &Y[i]));
  int Q;
  assert(1 == scanf("%d", &Q));
  std::vector<int> T(Q), B(Q), L(Q), R(Q);
  for (int k = 0; k < Q; k++)
    assert(4 == scanf("%d%d%d%d", &T[k], &B[k], &L[k], &R[k]));
  fclose(stdin);

  std::vector<long long> C = mosaic(X, Y, T, B, L, R);

  int S = (int)C.size();
  for (int k = 0; k < S; k++)
    printf("%lld\n", C[k]);
  fclose(stdout);

  return 0;
}
