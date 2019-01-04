// Sample input 1, in CPP.
#include <cassert>
#include <cstdio>
#include "message.h"

long long GetPieceLength(long long node_index) {
  return (node_index + 1) * 100 / NumberOfNodes() - node_index * 100 / NumberOfNodes();
}

static char __S[101] = "ABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCABCA";
char GetSignalCharacter(long long position) {
  assert(MyNodeId() * 100 / NumberOfNodes() <= position);
  assert((MyNodeId() + 1) * 100 / NumberOfNodes() > position);
  return __S[position];
}
