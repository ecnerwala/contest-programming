// Sample input 3, in CPP.

#include <cassert>

int GetLength() {
  return 1;
}

char GetCharacter(long long index) {
  switch ((int)index) {
    case 0: return ')';
    default: assert(0);
  }
}
