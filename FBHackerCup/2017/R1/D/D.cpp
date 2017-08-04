#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

static const ll MOD = 1e9 + 7;

const int MAXN = 4e3;
int N;

const int MAXR = 4e3;
ll R[MAXN];

ll M;

ll inv(ll a, ll m) {
	return (a == 1) ? 1 : m - inv(m % a, a) * m / a;
}

ll perm(ll n, ll r) {
	if (n < r) return 0;
	ll res = 1;
	for (int i = 0; i < r; i++) {
		res *= n, res %= MOD;
		n--;
	}
	return res % MOD;
}

ll go() {
	if (N == 1) return M;
	ll res = 0;

	M--; // M is number of gaps

	for (int i = 0; i < N; i++) {
		M -= 2 * R[i];
	}

	unordered_map<ll, ll> mem;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (i == j) continue;

			// value is (N-2)! * choose(N + # gaps, N)
			// = perm(N + # gaps, N) / N / (N-1)

			ll gaps = M + R[i] + R[j];
			if (gaps < 0) continue;
			if (mem.count(gaps)) res += mem[gaps];
			else res += (mem[gaps] = perm(N + gaps, N));
			res %= MOD;
		}
	}

	res %= MOD;

	res *= inv(N * (N-1) % MOD, MOD), res %= MOD;

	return res % MOD;
}

int main() {
	int __T; cin >> __T;
	for (int T = 1; T <= __T; T++) {
		cin >> N >> M;
		assert(MAXN > N);
		for(int i = 0; i < N; i++) {
			cin >> R[i];
			assert(MAXR > R[i]);
		}
		cout << "Case #" << T << ": ";
		ll res = go() % MOD;
		cout << res << '\n';
	}
	return 0;
}
