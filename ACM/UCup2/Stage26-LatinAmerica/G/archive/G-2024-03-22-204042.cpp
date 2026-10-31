#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<int> primes; primes.reserve(N);
	std::vector<int> pfac(N+1);
	for (int i = 2; i <= N; i++) {
		if (pfac[i] == 0) {
			primes.push_back(i);
			pfac[i] = i;
		}
		for (int p : primes) {
			if (int64_t(i) * int64_t(p) > N) break;
			pfac[i * p] = p;
			if (p == pfac[i]) break;
		}
	}

	std::vector<int> W(N+1);
	int tot_W = 0;
	for (int i = 1; i <= N; i++) {
		cin >> W[i];
		tot_W += W[i];
	}

	for (int p : primes) {
		for (int i = 1; i <= N/p; i++) {
			W[i*p] += W[i];
		}
	}


	std::vector<double> vals(N+1);
	for (int i = 1; i <= N; i++) {
		vals[i] = double(W[i]) / double(tot_W - W[i]);
	}
	for (int p : primes) {
		for (int i = N/p; i >= 1; i--) {
			vals[i*p] -= vals[i];
		}
	}

	cout << fixed << setprecision(10) << std::accumulate(vals.begin(), vals.end(), 0.) << '\n';

	return 0;
}
