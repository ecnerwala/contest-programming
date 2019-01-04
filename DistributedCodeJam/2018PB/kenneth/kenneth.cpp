#include "message.h"
#ifndef INPUT_HEADER
#define INPUT_HEADER "kenneth.h"
#endif
#include INPUT_HEADER
#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

struct num {
	static const int MOD = int(1e9) + 7;
	int v;
	num () : v(0) {}
	num (ll v_) : v(int(v_ % MOD)) { if (v < 0) v += MOD; }
	num& operator += (const num& o) {
		v += o.v;
		if (v >= MOD) v -= MOD;
		return *this;
	}
	num& operator -= (const num& o) {
		v -= o.v;
		if (v < 0) v += MOD;
		return *this;
	}
	num& operator *= (const num& o) {
		v = int(ll(v) * ll(o.v) % MOD);
		return *this;
	}
	operator int() const { return v; }
};
num operator - (const num &a) {
	num res(a);
	if (res.v) res.v = num::MOD - res.v;
	return res;
}
num operator + (const num &a, const num &b) { return num(a) += b; }
num operator - (const num &a, const num &b) { return num(a) -= b; }
num operator * (const num &a, const num &b) { return num(a) *= b; }
bool operator == (const num &a, const num &b) { return a.v == b.v; }
bool operator != (const num &a, const num &b) { return a.v != b.v; }

using hash_t = num;
const hash_t BASE = 31;

hash_t powmod(hash_t a, int b) {
	assert(b >= 0);
	hash_t r = 1;
	while (b) {
		if (b & 1) r *= a;
		a *= a;
		b /= 2;
	}
	return r;
}

const int NODES = NumberOfNodes();
const int ID = MyNodeId();

const int MAXSPLIT = 1.1e7;
hash_t pref[MAXSPLIT];

int main() {
	int L = 0;
	for (int n = 0; n < NODES; n++) {
		L += int(GetPieceLength(n));
	}
	assert(L >= 1);
	vector<int> factors;
	for (int i = 1; i * i <= L; i++) {
		if (L % i == 0) {
			factors.push_back(i);
			if (L / i > i) {
				factors.push_back(L / i);
			}
		}
	}
	sort(factors.begin(), factors.end());

	int S = 0;
	for (int n = 0; n < ID; n++) {
		S += int(GetPieceLength(n));
	}
	int T = S + int(GetPieceLength(ID));

	hash_t b = powmod(BASE, S);
	pref[0] = 0;
	for (int i = S; i < T; i++, b *= BASE) {
		pref[i-S+1] += pref[i-S] + b * hash_t(GetSignalCharacter(i));
	}

	for (int i : factors) {
		// L-i prefix * BASE^i
		hash_t p = 0;
		if (S < L-i) {
			p += pref[min(L-i, T) - S] * powmod(BASE, i);
		}
		// [i, ...) suffix
		hash_t s = 0;
		if (S >= i) {
			s += pref[T-S];
		} else if (T >= i) {
			s += pref[T-S] - pref[i-S];
		}
		PutInt(0, int(s - p));
	}
	Send(0);
	if (ID == 0) {
		for (int n = 0; n < NODES; n++) {
			Receive(n);
		}
		int res = L+1;
		for (int i : factors) {
			hash_t diff = 0;
			for (int n = 0; n < NODES; n++) {
				diff += hash_t(GetInt(n));
			}
			if (diff == hash_t(0)) {
				res = min(res, i);
			}
		}
		assert(res <= L);
		cout << res << '\n';
	}

	return 0;
}
