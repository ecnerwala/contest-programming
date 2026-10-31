#include<bits/stdc++.h>
using namespace std;

using ll = long long;

ll P;

ll modpow(ll a, ll e) {
	assert(e >= 1);
	ll r = 1;
	while (e) {
		if (e & 1) r = r * a % P;
		a = a * a % P;
		e /= 2;
	}
	return r;
}

const int MAXL = 2.1e5;
ll T[MAXL];

vector<ll> pfacs;

bool solve() {
	int L; cin >> L;
	for (int i = 0; i < L; i++) {
		cin >> T[i];
	}
	ll G; cin >> G;
	if (G == 0) {
		for (int i = 0; i < L; i++) {
			if (T[i] == 0) return true;
		}
		return false;
	}
	for (ll p : pfacs) {
		ll ord = P-1;
		while (ord % p == 0 && modpow(G, ord/p) == 1) {
			ord /= p;
		}
		if (ord % p == 0) {
			bool isGood = false;
			for (int i = 0; i < L; i++) {
				if (modpow(T[i], ord/p) != 1) {
					isGood = true;
					break;
				}
			}
			if (!isGood) return false;
		}
	}
	return true;
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> P;
	ll v = P-1;
	for (ll p = 2; v > 1; p++) {
		if (p * p > v) p = v;
		if (v % p == 0) {
			pfacs.push_back(p);
			while (v % p == 0) v /= p;
		}
	}
	int N; cin >> N;
	while (N--) {
		bool res = solve();
		cout << int(res) << '\n';
	}

	return 0;
}
