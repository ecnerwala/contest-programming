#include<bits/stdc++.h>
using namespace std;

using ll = long long;

const int CAP = 1000000000;

ll gcd(ll a, ll b) { if (a == 0 || b == 0) { return a ^ b; } return __gcd(a, b); }
ll lcm(ll a, ll b) { if (a == 0 || b == 0) { return 0; } return a / __gcd(a, b) * b; }

const int MAXN = 2500;
int N;
char O[MAXN];
ll V[MAXN];
ll R[MAXN];

bool isGood(ll a) {
	for (int i = 0; i < N; i++) {
		if (O[i] == 'L') {
			if (lcm(a, V[i]) != R[i]) return false;
		} else if (O[i] == 'G') {
			if (gcd(a, V[i]) != R[i]) return false;
		}
	}
	return true;
}

ll filterPrimes(ll primes, ll val) {
	ll ans = 1;
	while (true) {
		ll g = gcd(val, primes);
		if (g == 1) break;
		ans *= g;
		val /= g;
	}
	return ans;
}

ll go() {
	ll lo = 1;
	ll hi = 0;
	for (int i = 0; i < N; i++) {
		if (O[i] == 'L') {
			if (R[i] % V[i] != 0) return -1;
			ll gapPrimes = R[i] / V[i];

			lo = lcm(lo, filterPrimes(gapPrimes, R[i]));

			hi = gcd(hi, R[i]);
		} else if (O[i] == 'G') {
			if (V[i] % R[i] != 0) return -1;

			//ll gapPrimes = V[i] / R[i];

			lo = lcm(lo, R[i]);
		} else assert(false);

		if (lo > CAP) return -1;
		if (hi % lo != 0) {
			return -1;
		}
	}

	if (lo <= CAP && isGood(lo)) {
		return lo;
	} else {
		return -1;
	}
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);

	int T; cin >> T;
	for (int t = 1; t <= T; t++) {
		cerr << "Case #" << t << ":" << '\n';

		cin >> N;
		for (int i = 0; i < N; i++) {
			cin >> O[i] >> V[i] >> R[i];
		}

		ll ans = go();
		cout << "Case #" << t << ": " << ans << '\n';
	}

	return 0;
}
