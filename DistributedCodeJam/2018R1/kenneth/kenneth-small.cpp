#include "message.h"
#ifndef INPUT_HEADER
#define INPUT_HEADER "kenneth.h"
#endif
#include INPUT_HEADER
#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const int NODES = NumberOfNodes();
const int ID = MyNodeId();

const ll MOD1 = ll(1e9) + 9;
const ll MOD2 = ll(1e9) + 7;
const ll BASE = 10;

const ll MAXN = 1.1e7;
pair<ll, ll> hashes[MAXN];

void go_small() {
	if (ID != 0) return;

	int N = GetPieceLength(ID);
	assert(N >= 2);

	hashes[0].first = 0;
	hashes[0].second = 0;
	for (int i = 0; i < N; i++) {
		hashes[i+1] = hashes[i];
		hashes[i+1].first *= BASE;
		hashes[i+1].first += GetSignalCharacter(i);
		hashes[i+1].first %= MOD1;
		hashes[i+1].second *= BASE;
		hashes[i+1].second += GetSignalCharacter(i);
		hashes[i+1].second %= MOD2;
	}

	pair<ll, ll> suff(0, 0);
	pair<ll, ll> base(1, 1);
	ll res = -1;
	for (int i = N-1; i > 0; i--) {
		suff.first += base.first * GetSignalCharacter(i) % MOD1;
		suff.first %= MOD1;
		suff.second += base.second * GetSignalCharacter(i) % MOD2;
		suff.second %= MOD2;
		base.first *= BASE;
		base.first %= MOD1;
		base.second *= BASE;
		base.second %= MOD2;

		if (suff == hashes[N - i]) {
			res = i;
		}
		cerr << i << '\n';
	}
	assert(res != -1);
	cout << res << '\n';
}

int main() {
	go_small();

	return 0;
}
