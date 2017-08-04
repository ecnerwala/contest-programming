#include "message.h"
#include "weird_editor.h"
#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const ll MOD = ll(1e9) + 7;

const int NODES = NumberOfNodes();
const int ID = MyNodeId();
const ll L = GetNumberLength();

ll get_start(ll id) {
	return id * (L / NODES) + min(id, L % NODES);
}

ll powmod(ll a, ll b, ll m) {
	ll r = 1;
	a %= m;
	while (b) {
		if(b & 1) r *= a, r %= m;
		a *= a, a %= m;
		b /= 2;
	}
	return r;
}

int main() {
	ll start = get_start(ID);
	ll end = get_start(ID + 1);

	ll last[11];
	ll value[11];
	ll cnt[11];
	ll pow[11];
	for (int st = 0; st < 10; st++) {
		last[st] = st;
		value[st] = 0;
		cnt[st] = 0;
		pow[st] = 1;
	}
	for (ll i = end-1; i >= start; i--) {
		ll d = GetDigit(i);
		for (int st = 0; st < 10; st++) {
			if (d >= last[st]) {
				last[st] = d;
				value[st] += pow[st] * d, value[st] %= MOD;
				cnt[st] ++;
				pow[st] *= 10, pow[st] %= MOD;
			}
		}
	}
	ll last_right = 0;
	ll value_right = 0;
	ll cnt_right = 0;
	ll pow_right = 1;
	if (ID + 1 < NODES) {
		Receive(ID + 1);
		last_right = GetLL(ID + 1);
		value_right = GetLL(ID + 1);
		cnt_right = GetLL(ID + 1);
		pow_right = GetLL(ID + 1);
	}
	ll st = last_right;

	ll last_left = last[st];
	ll value_left = (value[st] * pow_right % MOD + value_right) % MOD;
	ll cnt_left = cnt[st] + cnt_right;
	ll pow_left = pow[st] * pow_right % MOD;

	if (ID > 0) {
		PutLL(ID - 1, last_left);
		PutLL(ID - 1, value_left);
		PutLL(ID - 1, cnt_left);
		PutLL(ID - 1, pow_left);
		Send(ID - 1);
	} else {
		cout << value_left * powmod(10, L - cnt_left, MOD) % MOD << '\n';
	}

	return 0;
}
