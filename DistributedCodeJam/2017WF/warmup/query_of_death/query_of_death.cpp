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

ll boundary(ll N, ll i, ll tot) {
	assert(0 <= i && i <= tot);
	return N / tot * i + min(i, N % tot);
}

ll do_worker(ll a, ll b) {
	if (a == b) return false;
	ll res = 0;
	for (ll i = a+1; i < b; i++) {
		res += GetValue(i);
	}
	int first = GetValue(a);
	res += first;
	for (int i = 0; i < 30; i++) {
		if (first != GetValue(a)) {
			return -1;
		}
	}
	return res;
}

int main() {
	if (ID == 0) {
		ll a = 0, b = GetLength();
		vector<int> alive;
		for (int i = 1; i < NODES; i++) alive.push_back(i);
		ll tot = 0;
		while (b - a > 1) {
			for (int i = 0; i < int(alive.size()); i++) {
				ll st = boundary(b-a, i, int(alive.size())) + a;
				ll en = boundary(b-a, i+1, int(alive.size())) + a;
				assert(a <= st && st <= b);
				assert(a <= st && en <= b);
				PutLL(alive[i], st);
				PutLL(alive[i], en);
				Send(alive[i]);
			}
			vector<int> nalive;
			for (int i = 0; i < int(alive.size()); i++) {
				int cur = alive[i];
				Receive(cur);
				ll st = GetLL(cur);
				ll en = GetLL(cur);
				ll res = GetLL(cur);
				if (res == -1) {
					assert(a <= st && en <= b);
					b = en, a = st;
				} else {
					tot += res;
					nalive.push_back(cur);
				}
			}
			alive = nalive;
		}
		tot += GetValue(a);
		cout << tot << '\n';
		for (int i = 1; i < NODES; i++) {
			PutLL(i, -1);
			Send(i);
		}
	} else {
		while (true) {
			Receive(0);
			ll a = GetLL(0);
			if (a == -1) break;
			ll b = GetLL(0);
			ll res = do_worker(a,b);
			PutLL(0, a);
			PutLL(0, b);
			PutLL(0, res);
			Send(0);
		}
	}
	return 0;
}
