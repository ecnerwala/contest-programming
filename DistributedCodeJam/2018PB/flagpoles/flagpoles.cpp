#include "message.h"
#ifndef INPUT_HEADER
#define INPUT_HEADER "flagpoles.h"
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

const ll L = GetNumFlagpoles() - 2;

bool good(long long i) {
	return GetHeight(i+1) * 2 == GetHeight(i) + GetHeight(i+2);
}

int main() {
	if (GetNumFlagpoles() <= 2) {
		if (ID == 0) {
			cout << GetNumFlagpoles() << '\n';
		}
		return 0;
	}

	assert(L > 0);

	ll res = 0;
	ll pref = 0;
	ll suff = 0;
	bool all = true;

	const ll st = split(L, ID, NODES);
	const ll en = split(L, ID+1, NODES);
	for (ll i = st; i < en; i++) {
		if (good(i)) {
			if (all) pref++;
			suff++;
			res = max(res, suff);
		} else {
			all = false;
			suff = 0;
		}
	}
	if (ID > 0) {
		PutLL(0, res);
		PutLL(0, pref);
		PutLL(0, suff);
		PutLL(0, all ? 1ll : 0ll);
		Send(0);
	} else {
		for (int n = 1; n < NODES; n++) {
			Receive(n);
			res = max(res, GetLL(n));
			ll np = GetLL(n);
			ll ns = GetLL(n);
			ll na = GetLL(n);
			res = max(res, suff + np);
			if (all) pref += np;
			if (na) ns += suff;
			suff = ns;
			all = all && na;
		}
		cout << res + 2 << '\n';
	}


	return 0;
}
