#include "message.h"
#ifndef INPUT_HEADER
#define INPUT_HEADER "baby_blocks.h"
#endif
#include INPUT_HEADER
#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

ll split (ll a, ll i, ll n) {
	return (a / n) * i + min(a % n, i);
}

const int NODES = NumberOfNodes();
const int ID = MyNodeId();

const ll L = GetNumberOfBlocks();

const int MAXNODES = 120;
ll splits[MAXNODES + 1];
ll prefs[MAXNODES + 1];
ll T;

int main() {
	for (int i = 0; i <= NODES; i++) {
		splits[i] = split(L, i, NODES);
	}
	{
		ll tot = 0;
		for (ll i = splits[ID]; i < splits[ID+1]; i++) {
			tot += GetBlockWeight(i);
		}
		for (int z = 0; z < NODES; z++) {
			int n = (ID + z) % NODES;
			PutLL(n, tot);
			Send(n);
		}
		for (int z = NODES; z > 0; z--) {
			int n = (ID + z) % NODES;
			Receive(n);
			prefs[n+1] = GetLL(n);
		}
		for (int i = 0; i < NODES; i++) {
			prefs[i+1] += prefs[i];
		}
		T = prefs[NODES];
	}
	ll res = 0;
	ll jobs = 0;
	for (int i = 0; i < NODES; i++) {
		for (int j = NODES-1; j >= i; j--) {
			if (prefs[j+1] + prefs[i+1] >= T && prefs[j] + prefs[i] <= T) {
				jobs++;
				if (jobs % NODES == ID) {
					ll a = splits[i], b = splits[j+1];
					ll s = prefs[i] + prefs[j+1];
					while (a < splits[i+1] && b >= splits[j] && a <= b) {
						if (s == T) {
							if (b < splits[j+1]) {
								res++;
							}
							if (a+1 < splits[i+1]) s += GetBlockWeight(a);
							a++;
							if (b-1 >= splits[j]) s -= GetBlockWeight(b-1);
							b--;
						} else if (s < T) {
							if (a+1 < splits[i+1]) s += GetBlockWeight(a);
							a++;
						} else if (s > T) {
							if (b-1 >= splits[j]) s -= GetBlockWeight(b-1);
							b--;
						} else assert(false);
					}
				}
			}
		}
	}
	if (ID > 0) {
		PutLL(0, res);
		Send(0);
	} else {
		for (int n = 1; n < NODES; n++) {
			Receive(n);
			res += GetLL(n);
		}
		cout << res << '\n';
	}

	return 0;
}
