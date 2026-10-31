#include <unistd.h>

#include <cassert>
#include <cstring>

#include "escape_route.h"

namespace {
const int N_MAX = 90;
const int M_MAX = N_MAX * (N_MAX - 1) / 2;
const long long S_MAX = 1000000000000000;
const int Q_MAX = 3000000;

int digits(long long x) {
  if (x < 10) {
    return 1;
  }
  return digits(x / 10) + 1;
}

const int N_digits = digits(N_MAX);
const int M_digits = digits(M_MAX);
const int S_digits = digits(S_MAX);
const int Q_digits = digits(Q_MAX);
const int ABUV_digits = digits(N_MAX - 1);
const int LCT_digits = digits(S_MAX - 1);
const int answer_digits = 19;

int N, M, Q;
long long S;
std::vector<int> A, B, U, V;
std::vector<long long> L, C, T;

class Input {
 private:
  std::vector<char> buffer;
  int pos;

  int read_NABUV() {
    if (buffer[pos + 1] == ' ') {
      int x = buffer[pos] - '0';
      pos += 2;
      return x;
    } else {
      int x = (buffer[pos] - '0') * 10 + (buffer[pos + 1] - '0');
      pos += 3;
      return x;
    }
  }
  long long read_number(char sep) {
    long long x = 0;
    while (true) {
      if (buffer[pos] == sep) {
        pos++;
        break;
      } else if (buffer[pos + 1] == sep) {
        x = x * 10 + (long long)(buffer[pos] - '0');
        pos += 2;
        break;
      } else {
        x = x * 100 +
            (long long)((buffer[pos] - '0') * 10 + (buffer[pos + 1] - '0'));
        pos += 2;
      }
    }
    return x;
  }

 public:
  void read_all() {
    int l1 = N_digits + 1 + M_digits + 1 + S_digits + 1 + Q_digits + 1;
    int l2 =
        (ABUV_digits + 1 + ABUV_digits + 1 + LCT_digits + 1 + LCT_digits + 1) *
        M_MAX;
    int l3 = (ABUV_digits + 1 + ABUV_digits + 1 + LCT_digits + 1) * Q_MAX;
    int total_length = l1 + l2 + l3;
    buffer.resize(total_length + 1);
    size_t read_result =
        read(0, buffer.data(), sizeof(char) * (total_length + 1));
    assert(read_result <= sizeof(char) * (total_length + 1));
    pos = 0;

    N = read_NABUV();
    M = read_number(' ');
    S = read_number(' ');
    Q = read_number('\n');

    A.resize(M);
    B.resize(M);
    L.resize(M);
    C.resize(M);
    for (int i = 0; i < M; i++) {
      A[i] = read_NABUV();
      B[i] = read_NABUV();
      L[i] = read_number(' ');
      C[i] = read_number('\n');
    }

    U.resize(Q);
    V.resize(Q);
    T.resize(Q);
    for (int j = 0; j < Q; j++) {
      U[j] = read_NABUV();
      V[j] = read_NABUV();
      T[j] = read_number('\n');
    }
  }
};

void output(std::vector<long long>& answer) {
  int total_length = (answer_digits + 1) * answer.size();
  std::vector<char> buffer(total_length);
  int pos = 0;

  std::vector<char> tmp(answer_digits);
  for (long long ans : answer) {
    int p2 = answer_digits - 1;
    while (true) {
      if (ans < 10) {
        tmp[p2] = char(ans) + '0';
        break;
      }
      tmp[p2] = char(ans % 10) + '0';
      ans /= 10;
      p2--;
    }
    memcpy(buffer.data() + pos, tmp.data() + p2, answer_digits - p2);
    pos += answer_digits - p2;
    buffer[pos] = '\n';
    pos++;
  }

  int write_result = write(1, buffer.data(), sizeof(char) * pos);
  assert(write_result == pos);
}
}  // namespace

int main() {
  Input().read_all();

  std::vector<long long> answer = calculate_necessary_time(
      N, M, S, Q, std::move(A), std::move(B), std::move(L), std::move(C),
      std::move(U), std::move(V), std::move(T));
  assert(answer.size() == (size_t)Q);

  output(answer);
}
