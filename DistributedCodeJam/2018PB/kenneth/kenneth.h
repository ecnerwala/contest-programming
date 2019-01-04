// Sample input 1, in CPP.
#include "message.h"
#include <cassert>
#include <cstdio>

long long GetPieceLength(long long node_index) {
  return 10;
}

static char __S[101] = "ABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCA";
char GetSignalCharacter(long long position) {
  assert(MyNodeId() * 100 / NumberOfNodes() <= position);
  assert((MyNodeId() + 1) * 100 / NumberOfNodes() > position);
  return __S[position];
}
