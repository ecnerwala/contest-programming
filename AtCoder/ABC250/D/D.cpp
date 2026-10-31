#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int64_t N; cin >> N;
	int V = 1;
	while (int64_t(V) * int64_t(V) * int64_t(V) <= N) V++;
	V--;
	std::vector<int> primes; primes.reserve(V/2);
	std::vector<int> pfac(V+1);
	std::vector<int> num_primes(V+1);
	int64_t ans = 0;
	for (int i = 2; i <= V; i++) {
		num_primes[i] = num_primes[i-1];
		if (!pfac[i]) {
			primes.push_back(i);
			pfac[i] = i;
			num_primes[i]++;
			ans += num_primes[std::min<int64_t>(i-1, N / i / i / i)];
		}
		for (int p : primes) {
			if (int64_t(i) * int64_t(p) > V) break;
			pfac[i * p] = p;
			if (p == pfac[i]) break;
		}
	}
	cout << ans << '\n';

	return 0;
}
