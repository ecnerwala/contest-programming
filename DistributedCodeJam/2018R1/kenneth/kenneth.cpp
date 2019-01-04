#include "message.h"
#ifndef INPUT_HEADER
#define INPUT_HEADER "kenneth.h"
#endif
#include INPUT_HEADER
#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const int NODES = NumberOfNodes();
const int ID = MyNodeId();

const ll MOD1 = ll(1e9) + 9;
const ll MOD2 = ll(1e9) + 7;
const ll BASE = 10;

ll powmod(ll a, ll e, ll m) {
	assert(e >= 0);
	ll r = 1;
	while (e) {
		if (e & 1) r *= a, r %= m;
		a *= a, a %= m;
		e /= 2;
	}
	return r;
}

const int MAXNODES = 150;
int L[MAXNODES];

int T;

const int MAXN = 1.1e7;
int N = int(GetPieceLength(ID));


pair<ll, ll> prefs[MAXN];

vector<int> factors;

void go() {
	L[0] = 0;
	for (int i = 0; i < NODES; i++) {
		L[i+1] = L[i] + int(GetPieceLength(i));
	}
	T = L[NODES];

	{
		for (ll f = 1; f * f <= T; f++) {
			if (T % f != 0) continue;
			factors.push_back(int(f));
			if (T / f < T && T / f > f) factors.push_back(int(T / f));
		}
		//for (int f : factors) cerr << f << '\n';
	}

	{
		pair<ll, ll> base(powmod(BASE, L[ID], MOD1), powmod(BASE, L[ID], MOD2));
		prefs[0] = pair<ll, ll>(0, 0);
		for (int i = 0; i < N; i++) {
			ll c = GetSignalCharacter(L[ID] + i);
			prefs[i+1] = pair<ll, ll>(base.first * c % MOD1, base.second * c % MOD2);
			base.first *= BASE, base.first %= MOD1;
			base.second *= BASE, base.second %= MOD2;
		}
		for (int i = 0; i < N; i++) {
			prefs[i+1].first += prefs[i].first % MOD1;
			prefs[i+1].second += prefs[i].second % MOD2;
		}
	}

	for (int f : factors) {
		// take the pref T - f and the last T - f
		pair<ll, ll> cur(0, 0);
		if (T - f >= L[ID]) {
			int end = min(T-f, L[ID+1]);
			assert(L[ID] <= end && end <= L[ID+1]);
			end -= L[ID];
			assert(0 <= end && end <= N);
			cur.first += prefs[end].first * powmod(BASE, f, MOD1); cur.first %= MOD1;
			cur.second += prefs[end].second * powmod(BASE, f, MOD2); cur.second %= MOD2;
		}
		if (f < L[ID+1]) {
			int st = max(f, L[ID]);
			assert(L[ID] <= st && st < L[ID+1]);
			st -= L[ID];
			assert(0 <= st && st <= N);
			pair<ll, ll> pt(prefs[N].first - prefs[st].first, prefs[N].second - prefs[st].second);
			cur.first -= pt.first; cur.first %= MOD1;
			cur.second -= pt.second; cur.second %= MOD2;
		}

		PutLL(0, cur.first);
		PutLL(0, cur.second);
	}
	Send(0);

	if (ID == 0) {
		vector<pair<ll, ll>> sums(factors.size());
		for (int i = 0; i < NODES; i++) {
			Receive(i);
			for (int z = 0; z < int(factors.size()); z++) {
				sums[z].first += GetLL(i);
				sums[z].first %= MOD1;
				sums[z].second += GetLL(i);
				sums[z].second %= MOD2;
			}
		}
		int res = T;
		for (int i = 0; i < int(factors.size()); i++) {
			//cerr << factors[i] << ' ' << sums[i].first << ' ' << sums[i].second << '\n';
			if (sums[i].first == 0 && sums[i].second == 0) {
				res = min(res, factors[i]);
			}
		}
		cout << res << '\n';
	}
}

int main() {
	go();
	return 0;
}
