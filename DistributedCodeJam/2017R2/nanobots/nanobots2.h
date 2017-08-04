// Sample input 2, in CPP.
#include <cassert>

long long GetRange() {
  return 1000000;
}

long long GetNumNanobots() {
  return 2;
}

static long long sizes[] = {999999,500000};
static long long speeds[] = {2,999999};

char Experiment(long long size, long long speed) {
  if (size < 1 || size > GetRange() || speed < 1 || speed > GetRange())
    assert(0);
  for (int i = 0; i < 2; ++i)
    if (sizes[i] > size && speeds[i] > speed) return 'T';
  return 'E';
}

