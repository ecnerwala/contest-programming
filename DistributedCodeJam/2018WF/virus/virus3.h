// Sample input 3, in CPP.
#include <stdio.h>
#include "message.h"  // NOLINT

static int __initialized;
static int __infected;

#define NUMBER_OF_NODES 40
#define MAX_MESSAGE_SIZE 64000

char buffer[NUMBER_OF_NODES][MAX_MESSAGE_SIZE];
int buffer_size[NUMBER_OF_NODES];

void Initialize() {
  int i;
  if (!__initialized) {
    __initialized = 1;
    if ((MyNodeId() == 7) || (MyNodeId() == 8) || (MyNodeId() == 9)) __infected = 1;
    for (i = 0; i < NUMBER_OF_NODES; i++) {
      buffer_size[i] = 0;
    }
  }
}

void VirusPutChar(int target, char value) {
  Initialize();
  buffer[target][buffer_size[target]++] = value;
}

void VirusSend(int target) {
  Initialize();
  int i;
  if (__infected) {
    PutChar(target, 1);
    for (i = 0; i < buffer_size[target]; ++i) {
      PutChar(target, 0);
    }
  } else {
    PutChar(target, 0);
    for (i = 0; i < buffer_size[target]; ++i) {
      PutChar(target, buffer[target][i]);
    }
  }
  buffer_size[target] = 0;
  Send(target);
}

int VirusReceive(int source) {
  Initialize();
  int real_source = Receive(source);
  char message_infected = GetChar(real_source);
  if (message_infected) __infected = 1;
  return real_source;
}

void PrintNumber(int number) {
  Initialize();
  if (__infected) {
    printf("X\n");
  } else {
    printf("%d\n", number);
  }
}
