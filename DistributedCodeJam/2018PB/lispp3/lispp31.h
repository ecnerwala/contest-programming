// Sample input 1, in CPP.

#include <cassert>

int GetLength() {
  return 9;
}

char GetCharacter(long long index) {
  switch ((int)index) {
    case 0: return '(';
    case 1: return '(';
    case 2: return ')';
    case 3: return '+';
    case 4: return '(';
    case 5: return ')';
    case 6: return ')';
    case 7: return '(';
    case 8: return ')';
    default: assert(0);
  }
}
