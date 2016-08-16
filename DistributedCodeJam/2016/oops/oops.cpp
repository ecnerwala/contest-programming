#include "message.h"
#include "oops.h"
#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const ll INF = 2e18;

const int NODES = NumberOfNodes();
const int ID = MyNodeId();

const int N = GetN();

const int S = (N + NODES - 1) / NODES;

int main() {

	ll mi = INF, ma = -INF;
	for(int i = S * ID; i < N && i < S * (ID + 1); i++) {
		ll v = GetNumber(i);
		mi = min(mi, v);
		ma = max(ma, v);
	}

	if(ID) {
		PutLL(0, mi);
		PutLL(0, ma);
		Send(0);
	} else {
		for(int i = 1; i < NODES; i++) {
			int source = Receive(-1);
			ll nmi = GetLL(source);
			ll nma = GetLL(source);
			mi = min(mi, nmi);
			ma = max(ma, nma);
		}
		printf("%lld\n", ma - mi);
	}

	return 0;
}
