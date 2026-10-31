#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	const int V = 1e5;
	std::vector<int> primes; primes.reserve(V);
	std::vector<int> pfac(V+1);
	for (int i = 2; i <= V; i++) {
		if (!pfac[i]) {
			primes.push_back(i);
			pfac[i] = i;
		}
		for (int p : primes) {
			if (int64_t(i) * int64_t(p) > V) break;
			pfac[i*p] = p;
			if (p == pfac[i]) break;
		}
	}

	std::vector<int> base_wins(V+1);
	for (int i = 0; i <= V; i++) {
		base_wins[i] = false;
		for (int p : primes) {
			if (p > i) break;
			if (!base_wins[i-p]) {
				base_wins[i] = true;
				break;
			}
		}
	}
	auto solve = [&](int N) {
		std::vector<int> A(N);
		std::iota(A.begin(), A.end(), 1);

		if (N <= 2) {
			cout << -1 << '\n';
			return;
		} else if (base_wins[N-1]) {
			for (int i = 0; i < N; i++) {
				cout << A[i] << " \n"[i+1==N];
			}
			return;
		} else if (base_wins[N-2]) {
			std::swap(A[N-2], A[N-1]);
			for (int i = 0; i < N; i++) {
				cout << A[i] << " \n"[i+1==N];
			}
			return;
		} else if (base_wins[N-3]) {
			std::swap(A[N-2], A[N-1]);
			std::swap(A[N-3], A[N-2]);
			for (int i = 0; i < N; i++) {
				cout << A[i] << " \n"[i+1==N];
			}
			return;
		} else assert(false);
	};

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		solve(N);
	}

	return 0;
}
