// Sample input 1, in CPP.
#include <cassert>

long long GetRange() {
  return 1e12;
}

long long GetNumNanobots() {
  return 1e7;
}


char Experiment(long long size, long long speed) {
  if (size < 1 || size > GetRange() || speed < 1 || speed > GetRange())
    assert(0);
  return (speed + size <= (long long)(1e7) + 1) ? 'T' : 'E';
}

