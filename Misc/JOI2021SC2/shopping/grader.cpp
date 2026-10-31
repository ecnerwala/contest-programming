#include "Anna.h"
#include "Bruno.h"
#include <cstdio>
#include <cstdlib>
#include <queue>
#include <vector>

namespace {

void WrongAnswer(int code) {
  printf("Wrong Answer [%d]\n", code);
  exit(0);
}

int num_calls_A, num_calls_B;
std::queue<bool> qx, qy;

}  // namespace

void SendA(bool y) {
  if (++num_calls_A > 18) {
    WrongAnswer(3);
  }
  qy.push(y);
}

void SendB(bool x) {
  if (++num_calls_B > 10000) {
    WrongAnswer(4);
  }
  qx.push(x);
}

int main() {
  int N, L, R;
  if (scanf("%d%d%d", &N, &L, &R) != 3) {
    fprintf(stderr, "Error while reading input\n");
    exit(1);
  }
  std::vector<int> P(N);
  for (int i = 0; i < N; ++i) {
    if (scanf("%d", &P[i]) != 1) {
      fprintf(stderr, "Error while reading input\n");
      exit(1);
    }
  }
  
  num_calls_A = num_calls_B = 0;
  InitA(N, L, R);
  InitB(N, P);
  for (; ; ) {
    if (!qx.empty()) {
      ReceiveA(qx.front());
      qx.pop();
    } else if (!qy.empty()) {
      ReceiveB(qy.front());
      qy.pop();
    } else {
      break;
    }
  }
  int A = Answer();
  if (A < L || A > R) {
    WrongAnswer(1);
  }
  for(int i = L; i <= R; i++) {
	  if(P[i] < P[A]) {
		  WrongAnswer(2);
	  }
  }
  printf("Accepted: %d %d\n", num_calls_A, num_calls_B);
  return 0;
}
