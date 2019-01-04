// Sample input 1, in CPP.
#include <cassert>
#include <cstdio>
#include "message.h"

long long GetPieceLength(long long node_index) {
  return 9954;
}

static char __S[101] = "ABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCA";
char GetSignalCharacter(long long position) {
  return __S[position % 79];
}
