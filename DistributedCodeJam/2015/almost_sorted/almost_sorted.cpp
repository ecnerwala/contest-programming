#include "message.h"
#include "almost_sorted.h"
#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const int NODES = NumberOfNodes();
const int ID = MyNodeId();

const int N = NumberOfFiles();
const int D = MaxDistance();

const int S = max(D + 1, (N + NODES - 1) / NODES);

const int MAXS = 1.2e6;
ll V[MAXS * 3];

const int MOD = 1ll << 20;

int main() {
	assert(S < MAXS);
	int s = 0;
	for(int i = max(0, (ID - 1) * S); i < ID * S && i < N; i++) {
		V[s] = Identifier(i);
		s++;
	}
	int t = s;
	for(int i = ID * S; i < (ID + 1) * S && i < N; i++) {
		V[t] = Identifier(i);
		t++;
	}
	int e = t;
	for(int i = (ID + 1) * S; i < (ID + 2) * S && i < N; i++) {
		V[e] = Identifier(i);
		e++;
	}

	sort(V, V + e);

	int val = 0;

	for(int i = s; i < t; i++) {
		val = (ll(V[i] % MOD) * (ll(i - s + ID * S) % MOD) + val) % MOD;
	}

	if(ID) {
		PutInt(0, val);
		Send(0);
	} else {
		for(int i = 1; i < NODES; i++) {
			int source = Receive(-1);
			val += GetInt(source);
			val %= MOD;
		}
		printf("%d\n", val);
	}

	return 0;
}
