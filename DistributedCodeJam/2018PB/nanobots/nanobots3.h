// Sample input 3, in CPP.
#include <cassert>

long long GetRange() {
  return 2;
}

long long GetNumNanobots() {
  return 4;
}

static long long sizes[] = {2,1,1,1};
static long long speeds[] = {1,2,1,1};

char Experiment(long long size, long long speed) {
  if (size < 1 || size > GetRange() || speed < 1 || speed > GetRange())
    assert(0);
  for (int i = 0; i < 4; ++i)
    if (sizes[i] > size && speeds[i] > speed) return 'T';
  return 'E';
}

