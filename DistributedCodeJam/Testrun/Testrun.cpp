#include "message.h"
#ifndef INPUT_HEADER
#define INPUT_HEADER "Testrun.h"
#endif
#include INPUT_HEADER
#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const int NODES = NumberOfNodes();
const int ID = MyNodeId();

int main() {
  long long sum = 0LL;
  for (long long pos = MyNodeId(); pos < GetN(); pos += NumberOfNodes()) {
    sum += GetNumber(pos);
  }
  if (MyNodeId() > 0) {
    Receive(MyNodeId() - 1);
    sum += GetLL(MyNodeId() - 1);
  }
  if (MyNodeId() < NumberOfNodes() - 1) {
    PutLL(MyNodeId() + 1, sum);
    Send(MyNodeId() + 1);
  } else {
    printf("%lld\n", sum);
  }
	return 0;
}
