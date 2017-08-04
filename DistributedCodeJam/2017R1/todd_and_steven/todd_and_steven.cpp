#include "message.h"
#include "todd_and_steven.h"
#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const ll MOD = ll(1e9) + 7;

const int NODES = NumberOfNodes();
const int ID = MyNodeId();

const ll T = GetToddLength();
const ll S = GetStevenLength();

ll get_T_start(ll id) {
	return id * (T / NODES) + min(id, T % NODES);
}

ll get_S_start(ll id) {
	return id * (S / NODES) + min(id, S % NODES);
}

int main() {

	ll res = 0;
	{
		ll start = get_T_start(ID);
		ll end = get_T_start(ID+1);

		ll mi = -1, ma = S;
		for (ll ind = start; ind < end; ind++) {
			assert(ma > mi);
			ll val = GetToddValue(ind);
			while (!(ma == S || GetStevenValue(ma) > val)) {
				ma += (ma - mi);
				if (ma > S) {
					ma = S;
				}
			}
			assert(ma <= S);
			while (ma - mi > 1) {
				ll md = (mi + ma) / 2;
				if (GetStevenValue(md) > val) {
					ma = md;
				} else {
					mi = md;
				}
			}
			res += (ind + ma) ^ val;
			res %= MOD;
		}
	}

	{
		ll start = get_S_start(ID);
		ll end = get_S_start(ID+1);

		ll mi = -1, ma = T;
		for (ll ind = start; ind < end; ind++) {
			assert(ma > mi);
			ll val = GetStevenValue(ind);
			while (!(ma == T || GetToddValue(ma) > val)) {
				ma += (ma - mi);
				if (ma > T) {
					ma = T;
				}
			}
			assert(ma <= T);
			while (ma - mi > 1) {
				ll md = (mi + ma) / 2;
				if (GetToddValue(md) > val) {
					ma = md;
				} else {
					mi = md;
				}
			}
			res += (ind + ma) ^ val;
			res %= MOD;
		}
	}

	if (ID + 1 < NODES) {
		Receive(ID + 1);
		res += GetLL(ID + 1);
		res %= MOD;
	}
	
	if (ID > 0) {
		PutLL(ID - 1, res);
		Send(ID - 1);
	} else {
		cout << res << '\n';
	}


	return 0;
}
