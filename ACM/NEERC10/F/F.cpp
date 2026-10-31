#include<bits/stdc++.h>
using namespace std;

const int V = 1.1e4;
const int MAXV = V + 20;
bool isPrime[MAXV];
vector<int> primes;

void precomp() {
	for (int i = 2; i <= V; i++) isPrime[i] = true;
	for (int p = 2; p <= V; p++) {
		if (!isPrime[p]) continue;
		primes.push_back(p);
		for (int j = p + p; j <= V; j += p) isPrime[j] = false;
	}
	//cerr << primes.back() << '\n';
}

using ll = long long;
ll cnt[MAXV];

int main() {
	if (fopen("factorial.in", "r")) {
		freopen("factorial.in", "r", stdin);
		freopen("factorial.out", "w", stdout);
	}
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	precomp();
	int N, M; cin >> N >> M;
	for (int i = 0; i < N; i++) {
		int fac; cin >> fac;
		assert(primes.back() > fac);
		for (int p : primes) {
			if (p > fac) break;
			int f = 0;
			for (int v = fac / p; v; v /= p) f += v;
			cnt[p] += f;
		}
	}
	for (int i = 0; i < M; i++) {
		int fac; cin >> fac;
		assert(primes.back() > fac);
		for (int p : primes) {
			if (p > fac) break;
			int f = 0;
			for (int v = fac / p; v; v /= p) f += v;
			cnt[p] -= f;
		}
	}
	for (int p : primes) {
		if (cnt[p] < 0) {
			cout << -1 << '\n';
			exit(0);
		}
	}
	vector<pair<int, ll>> res;
	for (int fac = primes.back() - 1; fac > 1; fac --) {
		ll nUses = ll(1e18);
		for (int p : primes) {
			if (p > fac) break;
			int f = 0;
			for (int v = fac / p; v; v /= p) f += v;
			assert(f > 0);
			nUses = min(nUses, cnt[p] / f);
		}
		if (nUses == 0) continue;
		res.emplace_back(fac, nUses);
		for (int p : primes) {
			if (p > fac) break;
			int f = 0;
			for (int v = fac / p; v; v /= p) f += v;
			assert(f > 0);
			cnt[p] -= nUses * f;
		}
	}
	cout << res.size() << '\n';
	for (auto it : res) {
		cout << it.first << ' ' << it.second << '\n';
	}

	return 0;
}
