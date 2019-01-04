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

const ll L = GetLength();

ll split(ll a, int i, int n) {
	assert(0 <= i && i <= n);
	ll res = (a / n) * ll(i) + min(a % n, ll(i));
	assert(0 <= res && res <= a);
	return res;
}

void phase2(int bad, int id) {
	ll bst = split(L, bad, NODES - 2), ben = split(L, bad + 1, NODES - 2);
	ll mst = bst + split(ben - bst, id, NODES - 2), men = bst + split(ben - bst, id + 1, NODES - 2);

	ll res = 0;
	bool good = true;
	ll i = mst;
	while (i < men) {
		assert(mst <= i && i < men);
		assert(0 <= mst && men <= L);
		int a = GetValue(i);
		for (int z = 0; z < 100; z++) {
			if (GetValue(i) != a) {
				good = false;
				break;
			}
		}
		res += a;
		i++;
		if (!good) {
			break;
		}
	}
	PutLL(NODES - 1, i);
	PutLL(NODES - 1, men);
	PutLL(NODES - 1, res);
	Send(NODES - 1);
}

int main() {
	assert(NODES > 2);
	ll tot = 0;
	if (ID < NODES - 2) {
		ll st = split(L, ID, NODES - 2), en = split(L, ID+1, NODES - 2);
		ll res = 0;
		for (ll i = st; i < en; i++) {
			res += GetValue(i);
		}
		bool good = true;
		if (st < en) {
			int a = GetValue(st);
			for (int i = 0; i < 100; i++) {
				if (GetValue(st) != a) {
					good = false;
					break;
				}
			}
		}

		if (good) {
			PutLL(NODES - 1, res);
		} else {
			PutLL(NODES - 1, -1);
		}
		Send(NODES - 1);

		if (!good) exit(0);

		Receive(NODES - 1);
		int bad = GetInt(NODES - 1);
		phase2(bad, ID);

	} else if (ID == NODES - 2) {
		Receive(NODES - 1);
		int bad = GetInt(NODES - 1);
		phase2(bad, bad);

	} else if (ID == NODES - 1) {
		int bad = -1;
		for (int i = 0; i < NODES-2; i++) {
			Receive(i);
			ll res = GetLL(i);
			if (res == -1) {
				assert(bad == -1);
				bad = i;
			} else {
				tot += res;
			}
		}
		assert(bad != -1);
		cerr << "Bad is " << bad << '\n';

		for (int i = 0; i < NODES - 2; i++) {
			if (i == bad) continue;
			PutInt(i, bad);
			Send(i);
		}

		PutInt(NODES - 2, bad);
		Send(NODES - 2);

		for (int i = 0; i <= NODES - 2; i++) {
			if (i == bad) continue;
			Receive(i);
			ll st = GetLL(i);
			ll en = GetLL(i);
			ll res = GetLL(i);
			tot += res;
			for (ll z = st; z < en; z++) {
				tot += GetValue(z);
			}
		}
		cout << tot << '\n';
	} else assert(false);
	return 0;
}
