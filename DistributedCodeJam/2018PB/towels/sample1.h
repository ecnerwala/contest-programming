// Sample input 1, in CPP.

#include <cassert>

#include <cstdio>

long long GetNumberOfStacks() {
  return 2LL;
}

long long GetNumberOfTowelsInStack() {
  return 3LL;
}

long long GetNumberOfGymUsersBeforeYou() {
  return 5LL;
}

long long GetTowelCleanlinessRank(long long stack, long long position) {
  if (stack == 0 && position == 0) return 1LL;
  if (stack == 0 && position == 1) return 4LL;
  if (stack == 0 && position == 2) return 6LL;
  if (stack == 1 && position == 0) return 3LL;
  if (stack == 1 && position == 1) return 2LL;
  if (stack == 1 && position == 2) return 5LL;
  fprintf(stderr, "Incorrect value:\n");
  fprintf(stderr, "stack = %lld\n", stack);
  fprintf(stderr, "position = %lld\n", position);
  assert(0);
}
