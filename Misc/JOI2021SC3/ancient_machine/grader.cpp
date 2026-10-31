#include "Anna.h"
#include "Bruno.h"
#include <cstdio>
#include <cstdlib>
#include <set>
#include <vector>

namespace {

void WrongAnswer(int code) {
  printf("Wrong Answer [%d]\n", code);
  exit(0);
}

int N;
std::vector<int> send_array, remove_array;
std::set<int> remove_set;

int CountGood(std::vector<char> S) {
  std::vector<int> l(N), r(N);
  for (int i = 0; i < N; i++) {
    l[i] = i - 1;
    r[i] = i + 1;
  }
  int good_count = 0;
  for (int i = 0; i < N; i++) {
    int d = remove_array[i];
    if (l[d] >= 0 && r[d] < N && S[l[d]] == 'X' && S[d] == 'Y' &&
        S[r[d]] == 'Z')
      good_count++;
    if (l[d] >= 0) r[l[d]] = r[d];
    if (r[d] < N) l[r[d]] = l[d];
  }
  return good_count;
}

}  // namespace

void Send(int a) {
  if (!(a == 0 || a == 1)) WrongAnswer(1);
  if (send_array.size() >= 200000) WrongAnswer(2);
  send_array.push_back(a);
}

void Remove(int d) {
  if (!(0 <= d && d <= N - 1)) WrongAnswer(3);
  if (remove_set.find(d) != remove_set.end()) WrongAnswer(4);
  remove_array.push_back(d);
  remove_set.insert(d);
}

int main() {
  if (scanf("%d", &N) != 1) {
    fprintf(stderr, "Error while reading input\n");
    exit(1);
  }
  std::vector<char> S(N);
  for (int i = 0; i < N; i++) {
    if (scanf(" %c", &S[i]) != 1) {
      fprintf(stderr, "Error while reading input\n");
      exit(1);
    }
  }
  Anna(N, S);
  int send_count = send_array.size();
  Bruno(N, send_count, send_array);
  if (int(remove_array.size()) != N) WrongAnswer(5);
  int good_count = CountGood(S);
  printf("Accepted: %d %d\n", send_count, good_count);
  return 0;
}
