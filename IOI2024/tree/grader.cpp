#include "tree.h"
#include <cassert>
#include <cstdio>

int main() {
  int N;
  assert(1 == scanf("%d", &N));
  std::vector<int> P(N);
  P[0] = -1;
  for (int i = 1; i < N; i++)
    assert(1 == scanf("%d", &P[i]));
  std::vector<int> W(N);
  for (int i = 0; i < N; i++)
    assert(1 == scanf("%d", &W[i]));
  int Q;
  assert(1 == scanf("%d", &Q));
  std::vector<int> L(Q), R(Q);
  for (int j = 0; j < Q; j++)
    assert(2 == scanf("%d%d", &L[j], &R[j]));
  fclose(stdin);

  init(P, W);
  std::vector<long long> A(Q);
  for (int j = 0; j < Q; j++)
    A[j] = query(L[j], R[j]);

  for (int j = 0; j < Q; j++)
    printf("%lld\n", A[j]);
  fclose(stdout);

  return 0;
}
