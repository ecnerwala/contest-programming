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

	PutLL(ID, ID);
	Send(ID);
	int o = Receive(-1);
	ll val = GetLL(o);
	cout << val << '\n';
	assert(ID == val);
	return 0;
}
