#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	const int N = int(1e7) + 10;
	const int V = N + 2;
	std::vector<int> primes; primes.reserve(V);
	std::vector<int> pfac(V+1);
	for (int i = 2; i <= V; i++) {
		if (!pfac[i]) {
			primes.push_back(i);
			pfac[i] = i;
		}
		for (int p : primes) {
			if (int64_t(p) * int64_t(i) > V) break;
			pfac[i * p] = p;
			if (p == pfac[i]) break;
		}
	}
	std::vector<int> ans(N+1);
	// p == 2
	ans[5]++;
	for (int i = 3; i <= N; i++) {
		if (pfac[i] == i && pfac[i+2] == i+2) {
			ans[i+2]++;
		}
	}
	for (int i = 1; i <= N; i++) ans[i] += ans[i-1];

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		int n; cin >> n;
		assert(n <= N);

		cout << "Case #" << case_num << ": " << ans[n] << '\n';
	}

	return 0;
}
