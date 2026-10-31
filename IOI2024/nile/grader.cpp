#include "nile.h"
#include <cassert>
#include <cstdio>
#include <vector>

int main() {
  int N;
  assert(1 == scanf("%d", &N));
  std::vector<int> W(N), A(N), B(N);
  for (int i = 0; i < N; i++)
    assert(3 == scanf("%d%d%d", &W[i], &A[i], &B[i]));
  int Q;
  assert(1 == scanf("%d", &Q));
  std::vector<int> E(Q);
  for (int j = 0; j < Q; j++)
    assert(1 == scanf("%d", &E[j]));
  fclose(stdin);

  std::vector<long long> R = calculate_costs(W, A, B, E);

  int S = (int)R.size();
  for (int j = 0; j < S; j++)
    printf("%lld\n", R[j]);
  fclose(stdout);

  return 0;
}
