#include "insects.h"

#include <cassert>
#include <cstdio>

#include <algorithm>
#include <map>
#include <set>
#include <string>
#include <vector>

static inline constexpr int kMaxQueries = 40000;

static int N;
// Insect types are compressed to colors in the range [0, N).
static std::vector<int> color;
static std::vector<bool> in_box;

static std::vector<int> color_occurrences;
static std::multiset<int> max_occurrences;

static std::vector<int> op_counter(3, 0);

static inline void protocol_violation(std::string message) {
  printf("Protocol Violation: %s\n", message.c_str());
  exit(0);
}

void move_inside(int i) {
  if (i < 0 || i >= N) {
    protocol_violation("invalid parameter");
  }
  ++op_counter[0];
  if (op_counter[0] > kMaxQueries) {
    protocol_violation("too many calls");
  }
  if (!in_box[i]) {
    in_box[i] = true;
    max_occurrences.erase(max_occurrences.find(color_occurrences[color[i]]));
    ++color_occurrences[color[i]];
    max_occurrences.insert(color_occurrences[color[i]]);
  }
}

void move_outside(int i) {
  if (i < 0 || i >= N) {
    protocol_violation("invalid parameter");
  }
  ++op_counter[1];
  if (op_counter[1] > kMaxQueries) {
    protocol_violation("too many calls");
  }
  if (in_box[i]) {
    in_box[i] = false;
    max_occurrences.erase(max_occurrences.find(color_occurrences[color[i]]));
    --color_occurrences[color[i]];
    max_occurrences.insert(color_occurrences[color[i]]);
  }
}

int press_button() {
  ++op_counter[2];
  if (op_counter[2] > kMaxQueries) {
    protocol_violation("too many calls");
  }
  return *(max_occurrences.rbegin());
}

int main() {
  assert(1 == scanf("%d", &N));
  color.resize(N);
  in_box.assign(N, false);

  std::map<int, int> type_to_color;
  for (int i = 0; i < N; ++i) {
    int Ti;
    assert(1 == scanf("%d", &Ti));
    if (type_to_color.find(Ti) == type_to_color.end()) {
      int new_color = type_to_color.size();
      type_to_color[Ti] = new_color;
      max_occurrences.insert(0);
    }
    color[i] = type_to_color[Ti];
  }

  color_occurrences.assign(type_to_color.size(), 0);

  int answer = min_cardinality(N);
  int Q = *std::max_element(op_counter.begin(), op_counter.end());
  printf("%d\n", answer);
  printf("%d\n", Q);
  return 0;
}
