#include "message.h"
#ifndef INPUT_HEADER
#define INPUT_HEADER "number_bases.h"
#endif
#include INPUT_HEADER
#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const int NODES = NumberOfNodes();
const int ID = MyNodeId();

ll split (ll a, ll i, ll n = NODES) {
	return (a / n) * i + min(a % n, i);
}

const ll N = GetLength();
const ll ST = split(N, ID);
const ll EN = split(N, ID+1);

int main() {
	ll maxVal = 0;
	ll res = 0;
	for (ll i = ST; i < EN; i++) {
		assert(res != -1);
		maxVal = max(maxVal, GetDigitX(i));
		maxVal = max(maxVal, GetDigitY(i));
		maxVal = max(maxVal, GetDigitZ(i));
		ll c = 0;
		if (i && GetDigitX(i-1) + GetDigitY(i-1) > GetDigitZ(i-1)) {
			c = 1;
		}
		c += GetDigitX(i);
		c += GetDigitY(i);
		if (c < GetDigitZ(i)) {
			res = -1;
			break;
		} else if (c == GetDigitZ(i)) {
			// Noop
		} else {
			if (i == N-1) {
				res = -1;
				break;
			}
			ll b = c - GetDigitZ(i);
			if (b == 1) {
				res = -1;
				break;
			} else if (res == 0 || res == b) {
				res = b;
			} else {
				res = -1;
				break;
			}
		}
	}
	if (ID > 0) {
		PutLL(0, res);
		PutLL(0, maxVal);
		Send(0);
	} else {
		for (int i = 1; i < NODES; i++) {
			Receive(i);
			ll b = GetLL(i);
			maxVal = max(maxVal, GetLL(i));
			if (res == -1 || b == -1) {
				res = -1;
			} else if (b == 0) {
				// no-op
			} else if (res == 0 || res == b) {
				res = b;
			} else {
				res = -1;
			}
		}
		if (res > 0 && maxVal >= res) {
			res = -1;
		}
		if (res == -1) {
			cout << "IMPOSSIBLE\n";
		} else if (res == 0) {
			cout << "NON-UNIQUE\n";
		} else {
			assert(res >= 2);
			cout << res << '\n';
		}
	}

	return 0;
}
