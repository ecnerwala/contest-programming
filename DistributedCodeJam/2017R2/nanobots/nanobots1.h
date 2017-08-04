// Sample input 1, in CPP.
#include <cassert>

long long GetRange() {
  return 10;
}

long long GetNumNanobots() {
  return 4;
}

static long long sizes[] = {8,6,2,5};
static long long speeds[] = {2,4,10,5};

char Experiment(long long size, long long speed) {
  if (size < 1 || size > GetRange() || speed < 1 || speed > GetRange())
    assert(0);
  for (int i = 0; i < 4; ++i)
    if (sizes[i] > size && speeds[i] > speed) return 'T';
  return 'E';
}

