#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	vector<int> par(N);
	par[0] = -1;
	for (int i = 1; i < N; i++) cin >> par[i], par[i]--;
	vector<int> sz(N, 1);
	for (int i = N-1; i > 0; i--) {
		sz[par[i]] += sz[i];
	}

	// N! divided by each of the sizes
	vector<int> pfac(N+1);
	vector<int> primes; primes.reserve(N+1);
	for (int i = 2; i <= N; i++) {
		if (!pfac[i]) {
			primes.push_back(i);
			pfac[i] = i;
		}
		for (int p : primes) {
			if (p > N / i || p > pfac[i]) break;
			pfac[i*p] = p;
		}
	}

	map<int, int> cnt;
	for (int p : primes) {
		int v = 0;
		for (int a = N/p; a; a /= p) v += a;
		cnt[p] = v;
	}
	for (int s : sz) {
		for (; s > 1; s /= pfac[s]) {
			cnt[pfac[s]]--;
		}
	}

	vector<pair<int, int>> res; res.reserve(cnt.size());
	for (auto it : cnt) {
		if (it.second) res.push_back(it);
	}
	cout << res.size() << '\n';
	for (auto it : res) {
		cout << it.first << ' ' << it.second << '\n';
	}

	return 0;
}
