#include "message.h"
#ifndef INPUT_HEADER
#define INPUT_HEADER "broken_memory.h"
#endif
#include INPUT_HEADER
#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const int NODES = NumberOfNodes();
const int ID = MyNodeId();

const ll N = GetLength();

const ll MOD = ll(1e9) + 9;

ll split(ll st, ll en, ll i, ll n) {
	assert(st <= en);
	assert(0 <= i && i <= n);
	ll l = en - st;
	return st + (l / n) * i + min(l % n, i);
}

const ll B = 240;
ll H[2 * B];

int main() {
	assert(NODES % 2 == 0);
	int OID = (ID ^ 1);
	vector<pair<ll, ll>> ranges{{0, N}};
	vector<ll> cnds{};
	while (!ranges.empty()) {
		ll st = ranges.back().first, en = ranges.back().second;
		ranges.pop_back();
		//cerr << st << ' ' << en << '\n';

		if (en - st == 1) {
			cnds.push_back(st);
			continue;
		}

		for (ll i = 0; i < B; i++) {
			ll a = split(st, en, i, B);
			ll b = split(st, en, i+1, B);
			if (a == b) continue;
			ll hash = 0;
			for (ll z = a; z < b; z++) {
				hash %= MOD;
				hash *= 12345;
				hash %= MOD;
				hash += GetValue(z);
			}
			H[i] = hash;
			PutLL(OID, hash);
		}
		Send(OID);

		bool created = false;

		Receive(OID);
		for (ll i = 0; i < B; i++) {
			ll a = split(st, en, i, B);
			ll b = split(st, en, i+1, B);
			if (a == b) continue;
			ll hash = GetLL(OID);
			if (H[i] != hash) {
				ranges.emplace_back(a, b);
				created = true;
			}
		}

		assert(created);
		assert(ranges.size() > 0);
		assert(ranges.size() + cnds.size() <= 2);
	}

	assert(cnds.size() == 2);
	//cerr << cnds[0] << ' ' << cnds[1] << '\n';

	assert(NODES >= 4);
	PutLL((ID + 2) % NODES, cnds[0]);
	Send((ID + 2) % NODES);

	{
		int o = Receive((ID + NODES - 2) % NODES);
		ll ind = GetLL(o);
		ll v = GetValue(ind);
		PutLL(o, v);
		Send(o);
	}

	ll res;
	{
		int o = Receive((ID + 2) % NODES);
		ll v = GetLL(o);
		if (v != GetValue(cnds[0])) {
			res = cnds[0];
		} else {
			res = cnds[1];
		}
	}
	//cerr << res << '\n';

	PutLL(0, res);
	Send(0);

	if (ID == 0) {
		for (int i = 0; i < NODES; i++) {
			Receive(i);
			cout << GetLL(i) << " \n"[i == NODES - 1];
		}
	}

	return 0;
}
