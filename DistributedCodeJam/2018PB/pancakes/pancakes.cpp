#include "message.h"
#ifndef INPUT_HEADER
#define INPUT_HEADER "pancakes.h"
#endif
#include INPUT_HEADER
#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const int NODES = NumberOfNodes();
const int ID = MyNodeId();

ll split (ll a, ll i, ll n = NODES) {
	return (a / n) * i + min(a % n, i);
}

const ll N = GetStackSize();
const ll D = GetNumDiners();

const ll ST = split(N-1, ID);
const ll EN = split(N-1, ID+1);

int main() {
	ll res = 0;
	for (ll i = ST; i < EN; i++) {
		ll a = GetStackItem(i);
		ll b = GetStackItem(i+1);
		if (b < a) {
			res ++;
		}
	}
	if (ID > 0) {
		PutLL(0, res);
		Send(0);
	} else {
		for (int i = 1; i < NODES; i++) {
			Receive(i);
			res += GetLL(i);
		}
		cout << res+1 << '\n';
	}

	return 0;
}
