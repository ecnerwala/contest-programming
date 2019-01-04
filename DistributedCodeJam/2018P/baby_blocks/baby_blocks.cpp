#include "message.h"
#ifndef INPUT_HEADER
#define INPUT_HEADER "baby_blocks.h"
#endif
#include INPUT_HEADER
#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const int NODES = NumberOfNodes();
const int ID = MyNodeId();

const ll N = GetNumberOfBlocks();

const int MAXNODES = 200;
ll P[MAXNODES];
ll S[MAXNODES];
ll T;

void phase1() {
	ll s = 0;
	for (ll i = P[ID]; i < P[ID+1]; i++) {
		s += GetBlockWeight(i);
	}
	for (int i = 0; i < NODES; i++) {
		PutLL(i, s);
		Send(i);
	}

	for (int z = 0; z < NODES; z++) {
		int i = Receive(-1);
		S[i+1] = GetLL(i);
	}

	for (int i = 0; i < NODES; i++) {
		S[i+1] += S[i];
	}
	T = S[NODES];
}

ll do_job(int i, int j) {
	ll res = 0;
	ll a = P[i+1];
	ll b = P[j];
	ll v = S[i+1] + S[j];
	while (a > P[i] && b < P[j+1]) {
		//assert(a > 0 && b < N);
		if (v == T) {
			if (a <= b) res ++;
			//cerr << a << ' ' << b << '\n';
			v -= GetBlockWeight(--a);
			v += GetBlockWeight(b++);
		} else if (v > T) {
			v -= GetBlockWeight(--a);
		} else if (v < T) {
			v += GetBlockWeight(b++);
		} else assert(false);
	}
	return res;
}

void phase2() {
	int jobs = 0;
	ll myres = 0;
	for (int i = 0; i < NODES; i++) {
		for (int j = i; j < NODES; j++) {
			if (S[i] + S[j] < T && S[i+1] + S[j+1] > T) {
				jobs ++;
				if (jobs % NODES == ID) {
					myres += do_job(i, j);
				}
			}
		}
	}

	assert(jobs <= 3 * NODES);

	PutLL(0, myres);
	Send(0);

	if (ID == 0) {
		ll res = 0;
		for (int z = 0; z < NODES; z++) {
			int i = Receive(-1);
			res += GetLL(i);
		}
		cout << res << '\n';
	}
}

int main() {
	for (int i = 0; i <= NODES; i++) {
		P[i] = (N / NODES) * ll(i) + min(N % NODES, ll(i));
	}
	//assert(P[0] == 0);
	//assert(P[NODES] == N);

	phase1();

	phase2();

	return 0;
}
