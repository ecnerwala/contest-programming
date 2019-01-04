#include "message.h"
#include "benchmark.h"
#include <stdio.h>

// This is a full fan-out - one wave is everyone sending a message to everyone.
// For a hundred nodes and NumberOfWaves() == 1, this program will execute
// around 250ms for SizeOfMessage() == 1, and around 300ms for SizeOfMessage()
// == 10000.
int main() {
  int number_of_waves = NumberOfWaves();
  long long size_of_message = SizeOfMessage();
  int wave, target, source, received;
  long long i;
  char res = MyNodeId();
  for (wave = 0; wave < number_of_waves; ++wave) {
    for (target = 0; target < NumberOfNodes(); ++target) {
      for (i = 0; i < size_of_message; ++i) PutChar(target, res);
      Send(target);
    }
    for (source = 0; source < NumberOfNodes(); ++source) {
      received = Receive(-1);
      for (i = 0; i < size_of_message; ++i) res += GetChar(received);
    }
  }
  if (MyNodeId() == 0) printf("SUCCESS\n");
  return 0;
}
