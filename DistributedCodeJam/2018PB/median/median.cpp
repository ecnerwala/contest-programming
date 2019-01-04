#include "message.h"
#ifndef INPUT_HEADER
#define INPUT_HEADER "median.h"
#endif
#include INPUT_HEADER
#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

ll split (ll a, ll i, ll n) {
	return (a / n) * i + min(a % n, i);
}

const int NODES = NumberOfNodes();
const int ID = MyNodeId();

using hash_t = unsigned long long;
const hash_t BASE = 7;

hash_t powmod(hash_t a, ll b) {
	assert(b >= 0);
	hash_t r = 1;
	while (b) {
		if (b & 1) r = r * a;
		a = a * a;
		b /= 2;
	}
	return r;
}

// 1e4 buckets
const int B = int(1e4);
const int L = int(1e5);
const int WINDOW = 100;
const hash_t B_MAX = powmod(BASE, WINDOW);

hash_t GetHash(ll st) {
	hash_t v = 0;
	for (ll j = 0; j < WINDOW; j++) {
		v = v * BASE + hash_t(GetData(st+j));
	}
	return v;
}

const ll MAXSPLIT = 1.1e7;
ll N;
ll ST, EN;

const int ARY = 1000;
ll splits[ARY + 20];
ll cnts[ARY + 20];

int main() {
	{
		if (ID == 0) {
			for (ll i = 0; i < B; i++) {
				hash_t v = GetHash(i*L);
				for (int n = 0; n < NODES; n++) {
					PutLL(n, reinterpret_cast<ll&>(v));
				}
			}
			for (int n = 0; n < NODES; n++) {
				Send(n);
			}
		}
		unordered_map<hash_t, ll> hashes;
		Receive(0);
		for (ll i = 0; i < B; i++) {
			ll v = GetLL(0);
			hashes[reinterpret_cast<hash_t&>(v)] = i*L;
		}

		const ll MINOFF = B * L;
		hash_t v = GetHash(MINOFF);
		ll offset = -1;
		for (ll i = MINOFF; true; i++) {
			if (hashes.count(v)) {
				offset = i - hashes[v];
				break;
			}

			v *= BASE;
			v -= hash_t(GetData(i)) * B_MAX;
			v += hash_t(GetData(i + WINDOW));
		}

		if (ID == 0) {
			assert(offset > 0);
			ll len = abs(offset);
			assert(len > 0);
			while (len % 2 == 0) len /= 2;
			assert(len % 2 == 1);
			for (int n = 0; n < NODES; n++) {
				PutLL(n, len);
				Send(n);
			}
		}

		Receive(0);
		N = GetLL(0);
		assert(N % 2 == 1);

		ST = split(N, ID, NODES) + offset;
		EN = split(N, ID+1, NODES) + offset;
	}

	ll mi = 0, ma = ll(1e9) + 1;
	while (ma - mi > 1) {
		cerr << mi << ' ' << ma << '\n';
		// 1000-ary search
		for (int a = 0; a <= ARY; a++) {
			splits[a] = split(ma - mi, a, ARY) + mi;
			cnts[a] = 0;
		}
		for (ll i = ST; i < EN; i++) {
			ll ind = lower_bound(splits, splits + ARY+1, GetData(i)) - splits;
			if (ind <= ARY) {
				cnts[ind] ++;
			}
		}
		for (int a = 1; a < ARY; a++) {
			cnts[a] += cnts[a-1];
			PutLL(0, cnts[a]);
		}
		Send(0);
		if (ID == 0) {
			ll nmi = mi;
			ll nma = ma;
			for (int n = 0; n < NODES; n++) {
				Receive(n);
			}
			for (int a = 1; a < ARY; a++) {
				ll cnt = 0;
				for (int n = 0; n < NODES; n++) {
					cnt += GetLL(n);
				}
				if (cnt * 2 < N) {
					nmi = max(nmi, splits[a]);
				} else {
					nma = min(nma, splits[a]);
				}
			}
			for (int n = 0; n < NODES; n++) {
				PutLL(n, nmi);
				PutLL(n, nma);
				Send(n);
			}
		}
		Receive(0);
		mi = GetLL(0);
		ma = GetLL(0);
	}
	if (ID == 0) {
		cout << ma << '\n';
	}

	return 0;
}
