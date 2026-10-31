#include "hieroglyphs.h"
#include <cassert>
#include <cstdio>
#include <vector>

int main() {
  int N, M;
  assert(2 == scanf("%d%d", &N, &M));
  std::vector<int> A(N), B(M);
  for (int i = 0; i < N; i++)
    assert(1 == scanf("%d", &A[i]));
  for (int j = 0; j < M; j++)
    assert(1 == scanf("%d", &B[j]));
  fclose(stdin);

  std::vector<int> R = ucs(A, B);

  int T = (int)R.size();
  printf("%d\n", T);
  for (int i = 0; i < T; i++)
    printf("%s%d", (i == 0 ? "" : " "), R[i]);
  printf("\n");
  fclose(stdout);

  return 0;
}
