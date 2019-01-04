// Sample input 3, in CPP.

#include <cassert>

#include <cstdio>

long long GetNumberOfStacks() {
  return 4LL;
}

long long GetNumberOfTowelsInStack() {
  return 1LL;
}

long long GetNumberOfGymUsersBeforeYou() {
  return 2LL;
}

long long GetTowelCleanlinessRank(long long stack, long long position) {
  if (stack == 0 && position == 0) return 1LL;
  if (stack == 1 && position == 0) return 2LL;
  if (stack == 2 && position == 0) return 3LL;
  if (stack == 3 && position == 0) return 4LL;
  fprintf(stderr, "Incorrect value:\n");
  fprintf(stderr, "stack = %lld\n", stack);
  fprintf(stderr, "position = %lld\n", position);
  assert(0);
}
