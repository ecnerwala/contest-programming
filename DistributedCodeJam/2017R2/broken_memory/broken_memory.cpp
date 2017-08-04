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

const ll MAXN = 1.2e7;
ll pref[MAXN][2];

pair<ll, ll> do_compare(int id) {
	pair<ll, ll> res;
	{
		// phase 1, find first difference
		ll mi = 0, ma = N;
		while (ma - mi > 1) {
			ll md = (mi + ma) / 2;
			ll v1 = pref[md][0];
			ll v2 = pref[md][1];
			PutLL(id, v1);
			PutLL(id, v2);
			Send(id);
			Receive(id);
			ll u1 = GetLL(id);
			ll u2 = GetLL(id);
			if (u1 == v1 && u2 == v2) {
				mi = md;
			} else {
				ma = md;
			}
		}
		res.first = mi;
	}
	{
		// phase 2, find last difference
		ll mi = 0, ma = N;
		while (ma - mi > 1) {
			ll md = (mi + ma) / 2;
			ll v1 = pref[N][0] ^ pref[md][0];
			ll v2 = pref[N][1] - pref[md][1];
			PutLL(id, v1);
			PutLL(id, v2);
			Send(id);
			Receive(id);
			ll u1 = GetLL(id);
			ll u2 = GetLL(id);
			if (u1 == v1 && u2 == v2) {
				ma = md;
			} else {
				mi = md;
			}
		}
		cerr << mi << ' ' << ma << '\n';
		res.second = mi;
	}
	cerr << ID << ' ' << id << ' ' << res.first << ' ' << res.second << '\n';
	assert(res.first < res.second);
	return res;
}

ll find_common(pair<ll, ll> a, pair<ll, ll> b) {
	if (a.first == b.first || a.first == b.second) return a.first;
	if (a.second == b.first || a.second == b.second) return a.second;
	assert(false);
	return -1;
}

int main() {
	pref[0][0] = 0;
	ll mult = 1;
	for (int i = 0; i < N; i++, mult *= 3) {
		ll v = GetValue(i);
		pref[i+1][0] = v ^ pref[i][0];
		pref[i+1][1] = (v * mult) + pref[i][1];
	}

	assert(NODES >= 3);

	vector<pair<ll, ll>> results;

	// 0-1, 2-3, ...
	if (ID % 2 == 0 && ID + 1 < NODES) {
		results.push_back(do_compare(ID + 1));
	} else if (ID % 2 == 1 && ID - 1 >= 0) {
		results.push_back(do_compare(ID - 1));
	}

	if (ID % 2 == 1 && ID + 1 < NODES) {
		results.push_back(do_compare(ID + 1));
	} else if (ID % 2 == 0 && ID - 1 >= 0) {
		results.push_back(do_compare(ID - 1));
	}

	if (ID == 0) {
		results.push_back(do_compare(NODES - 1));
	} else if (ID == NODES - 1) {
		results.push_back(do_compare(0));
	}

	assert(results.size() == 2);

	if (ID) {
		ll res = find_common(results[0], results[1]);
		PutLL(0, res);
		Send(0);
	} else {
		cout << find_common(results[0], results[1]);
		for (int i = 1; i < NODES; i++) {
			Receive(i);
			ll res = GetLL(i);
			cout << ' ' << res;
		}
		cout << '\n';
	}

	return 0;
}
