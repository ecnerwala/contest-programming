// Sample input 2, in CPP.
#include <cassert>
#include <cstdio>
#include "message.h"

long long GetPieceLength(long long node_index) {
  return (node_index + 1) * 200 / NumberOfNodes() - node_index * 200 / NumberOfNodes();
}

static char __S[201] = "ABAAABAAABAAABAAABAAABAAABAAABAAABAAABAAABAAABAAABAAABAAABAAABAAABAAABAAABAAABAAABAAABAAABAAABAAABAAABAAABAAABAAABAAABAAABAAABAAABAAABAAABAAABAAABAAABAAABAAABAAABAAABAAABAAABAAABAAABAAABAAABAAABAAABAA";
char GetSignalCharacter(long long position) {
  assert(MyNodeId() * 200 / NumberOfNodes() <= position);
  assert((MyNodeId() + 1) * 200 / NumberOfNodes() > position);
  return __S[position];
}
