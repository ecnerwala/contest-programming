#include "message.h"
#include "benchmark.h"
#include <stdio.h>

// Node 0 takes the role of the "primary node". One wave consists of the primary
// sending a message to each of the worker nodes, and the workers bouncing the
// message back at the primary.
// For a hundred nodes and NumberOfWaves() == 1, this program will execute
// around 170ms for SizeOfMessage() == 1, and around 200ms for SizeOfMessage()
// == 10000.
int main() {
  int number_of_waves = NumberOfWaves();
  long long size_of_message = SizeOfMessage();
  int wave, target, source;
  long long i;
  char res = MyNodeId();
  for (wave = 0; wave < number_of_waves; ++wave) {
    if (MyNodeId() == 0) {
      for (target = 1; target < NumberOfNodes(); ++target) {
        for (i = 0; i < size_of_message; ++i) PutChar(target, res);
        Send(target);
      }
      for (target = 1; target < NumberOfNodes(); ++target) {
        source = Receive(-1);
        for (i = 0; i < size_of_message; ++i) res += GetChar(source);
      }
    } else {
      Receive(0);
      for (i = 0; i < size_of_message; ++i) res += GetChar(0);
      for (i = 0; i < size_of_message; ++i) PutChar(0, res);
      Send(0);
    }
  }
  if (MyNodeId() == 0) printf("SUCCESS\n");
  return 0;
}
