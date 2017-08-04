#include "message.h"
#include "pancakes.h"
#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const int NODES = NumberOfNodes();
const int ID = MyNodeId();
const ll N = GetStackSize();
const ll D = GetNumDiners();

int main() {

	ll cnt = 0;
	for (ll i = ID; i+1 < N; i += NODES) {
		ll a = GetStackItem(i);
		ll b = GetStackItem(i+1);
		if (b < a) {
			cnt++;
		}
	}
	if (ID + 1 < NODES) {
		Receive(ID + 1);
		cnt += GetLL(ID + 1);
	}
	if(ID > 0) {
		PutLL(ID - 1, cnt);
		Send(ID - 1);
	} else {
		printf("%lld\n", cnt + 1);
	}

	return 0;
}
