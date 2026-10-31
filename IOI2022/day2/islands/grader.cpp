#include "islands.h"

#include <cassert>
#include <cstdio>

#include <variant>
#include <vector>

int main() {
  int N, M;
  assert(2 == scanf("%d %d", &N, &M));

  std::vector<int> U(M), V(M);
  for (int i = 0; i < M; ++i) {
    assert(2 == scanf("%d %d", &U[i], &V[i]));
  }

  std::variant<bool, std::vector<int>> result = find_journey(N, M, U, V);
  if (result.index() == 0) {
    printf("0\n");
    if (std::get<bool>(result)) {
      printf("1\n");
    } else {
      printf("0\n");
    }
  } else {
    printf("1\n");
    std::vector<int> &canoes = std::get<std::vector<int>>(result);
    printf("%d\n", static_cast<int>(canoes.size()));
    for (int i = 0; i < static_cast<int>(canoes.size()); ++i) {
      if (i > 0) {
        printf(" ");
      }
      printf("%d", canoes[i]);
    }
    printf("\n");
  }
  return 0;
}
