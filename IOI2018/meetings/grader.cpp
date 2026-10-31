#include <cstdio>
#include <cstdlib>
#include <vector>
#include "meetings.h"

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
  int Q = read_int();
  std::vector<int> H(N);
  for (int i = 0; i < N; ++i) {
    H[i] = read_int();
  }
  std::vector<int> L(Q), R(Q);
  for (int j = 0; j < Q; ++j) {
    L[j] = read_int();
    R[j] = read_int();
  }

  std::vector<long long> C = minimum_costs(H, L, R);
  for (size_t j = 0; j < C.size(); ++j) {
    printf("%lld\n", C[j]);
  }
  return 0;
}
