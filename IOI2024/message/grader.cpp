#include "message.h"
#include <cassert>
#include <cstdio>
#include <cstdlib>

namespace {
const int PACKET_SIZE = 31;
const int CALLS_CNT_LIMIT = 100;

int calls_cnt;
std::vector<bool> C(PACKET_SIZE);
std::vector<std::vector<bool>> R;

void quit(const char* message) {
  printf("%s\n", message);
  exit(0);
}

void run_scenario() {
  R.clear();
  calls_cnt = 0;

  int S;
  assert(1 == scanf("%d", &S));
  std::vector<bool> M(S);
  for (int i = 0; i < S; i++) {
    int bit;
    assert(1 == scanf("%d", &bit));
    assert((bit == 0) || (bit == 1));
    M[i] = bit;
  }

  for (int i = 0; i < PACKET_SIZE; i++) {
    int bit;
    assert(1 == scanf("%d", &bit));
    assert((bit == 0) || (bit == 1));
    C[i] = bit;
  }

  send_message(M, C);
  std::vector<bool> D = receive_message(R);

  int K = (int)R.size();
  int L = (int)D.size();
  printf("%d %d\n", K, L);
  for (int i = 0; i < L; i++)
    printf("%s%d", (i == 0 ? "" : " "), (D[i] ? 1 : 0));
  printf("\n");
}

std::vector<bool> taint(const std::vector<bool>& A) {
  std::vector<bool> B = A;
  bool bit = 0;
  for (int i = 0; i < PACKET_SIZE; i++)
    if (C[i] == 1) {
      B[i] = bit;
      bit = !bit;
    }
  return B;
}

} // namespace

std::vector<bool> send_packet(std::vector<bool> A) {
  calls_cnt++;
  if (calls_cnt > CALLS_CNT_LIMIT)
    quit("Too many calls");
  if ((int)A.size() != PACKET_SIZE)
    quit("Invalid argument");

  std::vector<bool> B = taint(A);
  R.push_back(B);
  return B;
}

int main() {
  int T;
  assert(1 == scanf("%d", &T));
  for (int i = 1; i <= T; i++)
    run_scenario();
}
