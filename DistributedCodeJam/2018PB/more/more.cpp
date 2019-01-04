#include "message.h"
#ifndef INPUT_HEADER
#define INPUT_HEADER "more.h"
#endif
#include INPUT_HEADER
#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const int NODES = NumberOfNodes();
const int ID = MyNodeId();

ll split (ll a, ll i, ll n) {
	return (a / n) * i + min(a % n, i);
}

const int MOD = int(GetP());
struct num {
	int v;
	num() : v(0) {}
	num(int a) : v(a) {
		assert(0 <= a && a < MOD);
	}
	num& operator += (const num &o) {
		v += o.v;
		if (v >= MOD) v -= MOD;
		return *this;
	}
	num& operator -= (const num &o) {
		v -= o.v;
		if (v < 0) v += MOD;
		return *this;
	}
	num& operator *= (const num &o) {
		v = int(ll(v) * ll(o.v) % MOD);
		return *this;
	}
	explicit operator int() const {
		return v;
	}
};
num operator - (const num &n) {
	if (n.v == 0) return num(0);
	return num(MOD-n.v);
}
num operator + (const num &a, const num &b) { return num(a)+=b; }
num operator - (const num &a, const num &b) { return num(a)-=b; }
num operator * (const num &a, const num &b) { return num(a)*=b; }
bool operator == (const num &a, const num &b) { return a.v == b.v; }
bool operator != (const num &a, const num &b) { return a.v != b.v; }
std::ostream& operator << (std::ostream& out, const num &n) {
	return out << int(n);
}

const num M2 = num(int(GetM()));
const ll Q = GetN();

num A(ll i) {
	assert(i > 0);
	assert(i <= Q);
	if (i == 1) return 1;
	else if (i == 2) return 0;
	else return int(GetA(i));
}

const ll MAXSPLIT = ll(1e7) + 20;
const ll MAXPREF = MAXSPLIT;
const ll PREF = split(Q, 1, NODES+1);
num _V[MAXPREF+20];
num * const V = (_V+1);

num fibo[MAXSPLIT+20][2];
void precomp() {
	V[-1] = 0;
	V[0] = 0;
	for (int i = 1; i <= PREF; i++) {
		V[i] = V[i-1] + M2 * V[i-2] + V[i/3] + A(i);
	}
	fibo[0][0] = 1;
	fibo[0][1] = 0;
	fibo[1][0] = 0;
	fibo[1][1] = 1;
	for (int i = 2; i <= MAXSPLIT; i++) {
		for (int z = 0; z < 2; z++) {
			fibo[i][z] = fibo[i-1][z] + M2 * fibo[i-2][z];
		}
	}
}

num dp[MAXSPLIT];
num pdp[MAXSPLIT];
void solve(ll a, ll b) {
	if (b <= PREF) {
		for (ll i = a-1; i <= b; i++) {
			dp[i-(a-1)] = V[i];
		}
		return;
	}
	ll na = a/3, nb = b/3;
	solve(na, nb);
	memcpy(pdp, dp, sizeof(dp));
	if (a <= PREF) {
		dp[0] = V[a-1];
		dp[1] = V[a];
	} else {
		dp[0] = 0;
		dp[1] = 0;
	}
	for (ll i = a+1; i <= b; i++) {
		assert(na <= i/3 && i/3 <= nb);
		dp[i - (a-1)] = dp[i-1 - (a-1)] + M2 * dp[i-2 - (a-1)] + pdp[i/3 - (na-1)] + A(i);
	}
	if (a > PREF) {
		assert(ID > 0);
		Receive(ID - 1);
		dp[0] = GetInt(ID-1);
		dp[1] = GetInt(ID-1);
	}
	assert(b > PREF);
	if (ID + 1 < NODES) {
		for (ll i = b-1; i <= b; i++) {
			num v = dp[i-(a-1)];
			if (i > a && a > PREF) {
				// we need to patch old data
				v += fibo[i-(a-1)][0] * dp[0];
				v += fibo[i-(a-1)][1] * dp[1];
			}
			PutInt(ID+1, int(v));
		}
		Send(ID+1);
	}
	if (a > PREF) {
		for (ll i = a+1; i <= b; i++) {
			dp[i-(a-1)] += fibo[i-(a-1)][0] * dp[0] + fibo[i-(a-1)][1] * dp[1];
		}
	}
}

const ll ST = split(Q, ID+1, NODES+1);
const ll EN = split(Q, ID+2, NODES+1);

int main() {
	precomp();
	solve(ST, EN);
	if (ID == NODES-1) {
		cout << dp[EN - (ST-1)] << '\n';
	}

	return 0;
}
