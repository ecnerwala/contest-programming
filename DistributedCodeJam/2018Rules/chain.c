#include "message.h"
#include "benchmark.h"
#include <stdio.h>

// Nodes send messages in a chain pattern: 0->1->2->3->..., each node waiting to
// receive the previous message before beginning the next one.
// For a hundred nodes and NumberOfWaves == 1, this program will execute
// 400-500ms.
int main() {
  long long size_of_message = SizeOfMessage();
  int number_of_waves = NumberOfWaves();
  int wave, target, source;
  long long i;
  char res = 1;
  // Start of the chain.
  if (MyNodeId() == 0) {
    for (i = 0; i < size_of_message; ++i) PutChar(1, res);
    Send(1);
  }
  target = (MyNodeId() + 1) % NumberOfNodes();
  for (wave = 0; wave < number_of_waves; ++wave) {
    source = Receive(-1);
    for (i = 0; i < size_of_message; ++i) res += GetChar(source);
    for (i = 0; i < size_of_message; ++i) PutChar(target, res);
    Send(target);
  }
  if (MyNodeId() == 0) printf("SUCCESS\n");
  return 0;
}
