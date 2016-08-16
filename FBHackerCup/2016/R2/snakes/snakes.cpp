#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const ll INF = 2e9;

typedef pair<ll, ll> pll;
#define A first
#define B second

const ll MOD = 1e9 + 7;

const int MAXN = 3e5;
ll N;

pll snakes[MAXN];

ll go() {
	cin >> N;
	assert(N < MAXN);
	for(ll i = 0; i < N; i++) {
		ll x, h; cin >> x >> h;
		assert(x < MOD);
		assert(h < MOD);
		snakes[i] = pll(h, x);
	}
	sort(snakes, snakes + N);
	reverse(snakes, snakes + N);

	set<ll> taller;

	ll res = 0;

	for(ll z = 0; z < N;) {
		ll h = snakes[z].A;
		vector<ll> x;
		while(z < N && snakes[z].A == h) {
			x.push_back(snakes[z].B);
			z++;
		}

		// x is reverse-sorted
		reverse(x.begin(), x.end()); // sort x

		vector<ll> sumsq(x.size() + 1);
		vector<ll> sum(x.size() + 1);
		sum[0] = sumsq[0] = 0;
		for(ll i = 0; i < int(x.size()); i++) {
			sum[i + 1] = (sum[i] + x[i]) % MOD;
			sumsq[i + 1] = (sumsq[i] + x[i] * x[i]) % MOD;
		}

		for(ll i = 0; i < int(x.size()); i++) {
			auto next_tall_it = taller.upper_bound(x[i]);
			ll next_tall = (next_tall_it == taller.end()) ? INF : *next_tall_it;
			// note that if the next guy equals the next tall one, we're ok, so we use upper_bound
			ll j = upper_bound(x.begin(), x.end(), next_tall) - x.begin();
			// we take [i,j)
			res += x[i] * x[i] % MOD * (j - i) % MOD;
			res += (sumsq[j] - sumsq[i] + MOD) % MOD;
			res += (sum[i] - sum[j] + MOD) % MOD * x[i] % MOD * 2 % MOD;
			res %= MOD;
		}

		for(ll i : x) {
			taller.insert(i);
		}
	}


	return res;
}

int main() {
	ios_base::sync_with_stdio(0);
	int __T; cin >> __T;
	for(int T = 1; T <= __T; T++) {
		ll res = go() % MOD;
		cout << "Case #" << T << ": " << res << '\n';
	}

	return 0;
}
