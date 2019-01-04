#include "message.h"
#ifndef INPUT_HEADER
#define INPUT_HEADER "virus.h"
#endif
#include INPUT_HEADER
#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const int NODES = NumberOfNodes();
const int ID = MyNodeId();

bool infected = false;

void MyVirusSend(int target) {
	VirusPutChar(target, 'a');
	VirusSend(target);
}
// returns true if infected
int MyVirusReceive(int source) {
	source = VirusReceive(source);
	if (GetChar(source) == 0) {
		infected = true;
		return true;
	}
	return source;
}

int main() {
	for (int z = 2; z < NODES; z++) {
		int n = (ID + z) % NODES;
		MyVirusSend(n);
	}
	for (int z = 2; z < NODES; z++) {
		int n = (ID + NODES - z) % NODES;
		MyVirusReceive(n);
		if (z == 2) {
			MyVirusSend(n);
		}
	}
	MyVirusReceive((ID+2) % NODES);
	if (!infected) {
		PrintNumber(1);
		PrintNumber((ID + NODES - 1) % NODES);
	}

	return 0;
}
