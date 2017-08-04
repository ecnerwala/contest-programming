#include "message.h"
#include "query_of_death.h"
#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const int MAX_NODES = 200;
const int NODES = NumberOfNodes();
const int ID = MyNodeId();

const ll L = GetLength();

const ll NUM_CHECKS = 100;

ll do_master() {
	ll st = 0, en = L;
	ll sum = 0;

	int numLive = 0;
	assert(NODES < MAX_NODES);
	bool isLive[MAX_NODES];
	for (int i = 1; i < NODES; i++) {
		isLive[i] = true;
		numLive++;
	}

	while (en - st > 1) {
		assert(numLive > 0);
		{
			int cntLive = numLive;
			ll cur = st;
			for (int i = 1; i < NODES; i++) {
				if (isLive[i]) {
					assert(cntLive > 0);
					PutLL(i, cur);
					cur += (en - cur) / cntLive;
					PutLL(i, cur);
					Send(i);
					cntLive--;
				}
			}
			assert(cntLive == 0);
			assert(cur == en);
		}
		int numFound = 0;
		for (int i = 1; i < NODES; i++) {
			if (isLive[i]) {
				Receive(i);
				ll v = GetLL(i);
				if (v == -1) {
					numFound++;
					st = GetLL(i);
					en = GetLL(i);
					assert(st < en);

					isLive[i] = false;
					numLive--;
				} else {
					sum += v;
				}
			}
		}
		assert(numFound <= 1);
	}

	assert(en - st == 1);
	sum += GetValue(st);

	// signal to exit
	for (int i = 1; i < NODES; i++) {
		PutLL(i, -1);
		Send(i);
	}
	return sum;
}

void do_slave() {
	while(true) {
		Receive(0);
		ll st = GetLL(0);
		if (st == -1) return;
		ll en = GetLL(0);

		bool broken = false;
		ll sum = 0;
		for (ll i = st; i < en; i++) {
			sum += GetValue(i);
		}
		if (st < en) {
			ll v = GetValue(st);
			for (ll i = 0; i < NUM_CHECKS; i++) {
				if (GetValue(st) != v) {
					broken = true;
					break;
				}
			}
		}
		if (broken) {
			PutLL(0, -1);
			PutLL(0, st);
			PutLL(0, en);
		} else {
			PutLL(0, sum);
		}
		Send(0);
	}
}

int main() {
	if (ID == 0) {
		ll res = do_master();
		cout << res << '\n';
	} else {
		do_slave();
	}

	return 0;
}
