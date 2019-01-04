#include "message.h"
#ifndef INPUT_HEADER
#define INPUT_HEADER "query_of_death.h"
#endif
#include INPUT_HEADER
#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const int NODES = NumberOfNodes();
const int ID = MyNodeId();

ll split(ll a, ll i, ll n) {
	return (a / n) * i + min(a % n, i);
}

const ll N = GetLength();

int main() {
	vector<int> alive;
	for (int i = 0; i < NODES; i++) {
		alive.push_back(i);
	}

	ll tot = 0;
	ll mi = 0;
	ll ma = N;
	while (ma - mi > 1) {
		int id = int(lower_bound(alive.begin(), alive.end(), ID) - alive.begin());
		assert(alive[id] == ID);
		ll l = split(ma - mi, id, int(alive.size())) + mi;
		ll r = split(ma - mi, id+1, int(alive.size())) + mi;
		ll cur = 0;
		if (l < r) {
			for (ll i = l+1; i < r; i++) {
				cur += GetValue(i);
			}
			int a = GetValue(l);
			cur += a;
			for (int i = 0; i < 100; i++) {
				if (GetValue(l) != a) {
					cur = -1;
					break;
				}
			}
		}
		//cerr << tot << ' ' << mi << ' ' << ma << ' ' << id << ' ' << l << ' ' << r << ' ' << cur << '\n';

		for (int z = 0; z < int(alive.size()); z++) {
			int n = alive[(id + z) % alive.size()];
			PutLL(n, cur);
			Send(n);
		}

		vector<int> nalive;
		ll nmi = mi, nma = ma;
		for (int z = int(alive.size()); z > 0; z--) {
			int nid = (id+z) % int(alive.size());
			int n = alive[nid];
			Receive(n);
			ll v = GetLL(n);
			if (v == -1) {
				nmi = split(ma - mi, nid, int(alive.size())) + mi;
				nma = split(ma - mi, nid+1, int(alive.size())) + mi;
			} else {
				tot += v;
				nalive.push_back(n);
			}
		}
		sort(nalive.begin(), nalive.end());
		assert(nalive.size() == alive.size() - 1);
		mi = nmi;
		ma = nma;
		alive = nalive;

		if (cur == -1) return 0;
	}
	assert(!alive.empty());
	assert(ma - mi == 1);
	tot += GetValue(mi);
	if (ID == alive[0]) {
		cout << tot << '\n';
	}

	return 0;
}
