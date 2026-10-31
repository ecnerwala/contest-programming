#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	const int V = 1.1e6;
	vector<int> pfac(V+1);
	vector<int> primes; primes.reserve(1e5);
	for (int i = 2; i <= V; i++) {
		if (!pfac[i]) {
			pfac[i] = i;
			primes.push_back(i);
		}
		for (int p : primes) {
			if (p > V / i) break;
			pfac[i*p] = p;
			if (p == pfac[i]) break;
		}
	}
	for (int i = 1; i <= V; i++) {
		pfac[i] = (pfac[i] == i);
	}
	for (int i = V; i >= 1; i--) {
		pfac[i] = pfac[i] && pfac[i-2];
	}
	for (int i = 1; i <= V; i++) {
		pfac[i] += pfac[i-1];
	}
	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		cout << pfac[N] << '\n';
	}

	return 0;
}
