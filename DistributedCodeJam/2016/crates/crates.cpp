#include "message.h"
#include "crates.h"
#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const ll MOD = 1e9 + 7;

const int NODES = NumberOfNodes();
const int ID = MyNodeId();

const ll N = GetNumStacks();

const ll S = (N + NODES - 1) / NODES;

unordered_map<ll, ll> cache;
ll get(ll n) {
	assert(0 <= n && n < N);
	if(cache.count(n)) return cache[n];
	else return cache[n] = GetStackHeight(n + 1);
}

ll C;
ll F; // C % N
ll H; // C / N

// gets the target stack for crate a
ll target(ll a) {
	assert(0 <= a && a < C);
	if(a < (H + 1) * F) {
		return a / (H + 1);
	} else {
		return (a - F) / H;
	}
}

ll count(ll a) {
	assert(0 <= a && a <= N);
	if(a <= F) {
		return a * (H + 1);
	} else {
		return a * H + F;
	}
}

ll dist(ll a, ll b, ll i) {
	if(a == i) a++;
	if(b == i) b--;
	if(a > b) return 0;
	if(a < i && b > i) {
		return dist(a, i - 1, i) + dist(i + 1, b, i);
	}
	if(a < F && b >= F) {
		return dist(a, F - 1, i) + dist(F, b, i);
	}
	assert((a < F) == (b < F));
	ll h = H + (a < F); // height of each column
	return (b - a + 1) * (abs(b - i) + abs(a - i)) / 2 % MOD * h % MOD;
} 

int main() {

	ll sum = 0;
	for(ll i = S * ID; i < N && i < S * (ID + 1); i++) {
		sum += get(i);
	}

	ll pref = 0;

	if(ID) {
		Receive(ID - 1);
		pref = GetLL(ID - 1);
	}
	if(ID + 1 < NODES) {
		PutLL(ID + 1, pref + sum);
		Send(ID + 1);

		Receive(NODES - 1);
		C = GetLL(NODES - 1);
	} else {
		C = pref + sum;
		for(int i = 0; i < ID; i++) {
			PutLL(i, C);
			Send(i);
		}
	}
	//cerr << "C = " << C << '\n';

	F = C % N;
	H = C / N;

	ll res = 0;

	// now everyone knows pref and C
	for(ll i = S * ID; i < N && i < S * (ID + 1); pref += get(i), i++) {
		assert(i < N);
		// now let's figure out how far to send these guys
		// crates # [ pref,  pref+get(i) )
		ll v = get(i);
		if(!v) continue;

		ll l = pref, r = pref + v;

		ll a = target(l);
		ll b = target(r - 1);
		if(a == b) {
			res += (r - l) * abs(a - i); res %= MOD;
		} else {
			res += (count(a + 1) - l) * abs(a - i); res %= MOD;
			l = count(a + 1);
			a = target(l); // equivalent to a++

			res += (r - count(b)) * abs(b - i); res %= MOD;
			r = count(b);
			b = target(r - 1);

			res += dist(a, b, i); res %= MOD;
		}
	}

	if(ID) {
		PutLL(0, res);
		Send(0);
	} else {
		for(int i = 1; i < NODES; i++) {
			int source = Receive(-1);
			res += GetLL(source);
			res %= MOD;
		}
		printf("%lld\n", res);
	}


	return 0;
}
