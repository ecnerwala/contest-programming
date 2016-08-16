#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const ll MOD = 1e9 + 7;

ll comp_inv(ll a, ll m = MOD) { return (a == 1) ? 1 : (m - comp_inv(m % a, a) * m / a); }

const int MAXN = 12;
const int MAXP = 2e6;
int N;
ll P;
ll X[MAXN];

ll inv[MAXP];
ll fact[MAXP];
ll ifact[MAXP];
void precomp() {
	fact[0] = ifact[0] = 1;
	for(ll i = 1; i < MAXP; i++) {
		inv[i] = comp_inv(i);
		fact[i] = fact[i-1] * i % MOD;
		ifact[i] = ifact[i-1] * inv[i] % MOD;
	}
}

inline ll choose(ll n, ll r) {
	return fact[n] * ifact[n - r] % MOD * ifact[r] % MOD;
}

// number of ways of distributing up to n things into b buckets
inline ll dist(ll n, ll b) {
	if(n < 0) return 0;
	return choose(n + b, b);
}

ll dp[1 << MAXN];
ll ndp[1 << MAXN];

ll dig[MAXN];

ll transition[1 << MAXN];
void make_transition(int s) {
	for(int m = s; true; m = s & (m - 1)) {
		ll cnt = 0;
		ll tot = P - 1;
		for(int i = 0; i < N; i++) {
			if(s & (1 << i)) {
				cnt++;
				if(m & (1 << i)) {
					tot -= dig[i];
				}
			} else {
				tot -= dig[i];
			}
		}
		transition[m] = dist(tot, cnt);
		if(!m) break;
	}

	// 1 is >=, 0 is all

	for(int i = 0; i < N; i++) {
		if(s & (1 << i)) {
			for(int m = s; true; m = s & (m - 1)) {
				if(m & (1 << i)) continue;
				transition[m] -= transition[m | (1 << i)];
				if(transition[m] < 0) transition[m] += MOD;
				if(!m) break;
			}
		}
	}

	// 1 is >=, 0 is <

	for(int i = 0; i < N; i++) {
		if(s & (1 << i)) {
			for(int m = s; true; m = s & (m - 1)) {
				if(m & (1 << i)) continue;
				transition[m | (1 << i)] += transition[m];
				if(transition[m | (1 << i)] >= MOD) transition[m | (1 << i)] -= MOD;
				if(!m) break;
			}
		}
	}

	// 1 is all, 0 is <
}

int main() {
	precomp();
	cin >> N >> P;
	for(int i = 0; i < N; i++) cin >> X[i];
	ll tot = 1;
	for(int i = 0; i < N; i++) tot *= X[i] + 1, tot %= MOD;
	for(int m = 0; m < (1 << N); m++) {
		dp[m] = 1;
	}
	for(int z = 0; z < 30; z++) {
		for(int i = 0; i < N; i++) {
			dig[i] = X[i] % P;
			X[i] /= P;
		}
		for(int m = 0; m < (1 << N); m++) {
			ndp[m] = 0;
		}
		for(int s = 0; s < (1 << N); s++) {
			make_transition(s);
			for(int m = s; true; m = s & (m - 1)) {
				ndp[m] += transition[m] * dp[s];
				ndp[m] %= MOD;
				if(!m) break;
			}
		}
		for(int m = 0; m < (1 << N); m++) {
			dp[m] = ndp[m];
		}
	}

	tot -= dp[0];
	tot %= MOD;
	if(tot < 0) tot += MOD;

	cout << tot << '\n';

	return 0;
}
