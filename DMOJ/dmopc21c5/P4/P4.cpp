#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	const int V = 1e6;
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

	std::vector<int> goldbach(V+1);
	for (int i = 4; i <= V; i += 2) {
		for (int p : primes) {
			if (p >= i-p) break;
			if (pfac[i-p] == i-p) {
				goldbach[i] = p;
				break;
			}
		}
	}

	goldbach[5] = 2;
	goldbach[7] = 2;

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		if (N >= 5) {
			// N+1 or N
			int L = N;
			if (!goldbach[L]) L++;
			assert(goldbach[L]);
			std::vector<int> A; A.reserve(L);
			// +p -q
			int p = goldbach[L];
			int q = L-p;
			A.push_back(0);
			for (int cur = p; cur != 0; ) {
				A.push_back(cur);
				if (cur >= q) cur -= q;
				else cur += p;
			}
			assert(int(A.size()) == L);
			if (L == N) {
				for (int i = 0; i < N; i++) {
					cout << A[i]+1 << " \n"[i+1==N];
				}
			} else if (L == N+1) {
				for (int i = 0; i < N; i++) {
					cout << A[i+1] << " \n"[i+1==N];
				}
			} else assert(false);
		} else if (N == 1) {
			cout << 1 << '\n';
		} else if (N == 2) {
			cout << -1 << '\n';
		} else if (N == 3) {
			cout << -1 << '\n';
		} else if (N == 4) {
			cout << 3 << ' ' << 1 << ' ' << 4 << ' ' << 2 << '\n';
		} else assert(false);
	}

	return 0;
}
