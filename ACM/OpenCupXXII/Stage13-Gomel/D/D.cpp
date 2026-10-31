#include <bits/stdc++.h>

void solve0(int N) {
	int best_msk = -1;
	for (int msk = 0; msk < 1 << (N * (N-1) / 2); msk++) {
		int v = msk;
		std::vector<int> adj(N);
		for (int i = 0; i < N; i++) {
			adj[i] |= 1 << i;
			for (int j = i+1; j < N; j++) {
				if (v & 1) {
					adj[i] |= 1 << j;
					adj[j] |= 1 << i;
				}
				v >>= 1;
			}
		}
		for (int i = 0; i < N; i++) {
			if (__builtin_popcount(adj[i]) <= 2) goto bad;
		}
		for (int i = 0; i < N; i++) {
			for (int j = i+1; j < N; j++) {
				if (adj[i] & adj[j]) {
				} else {
					goto bad;
				}
			}
		}
		if (__builtin_popcount(msk) < __builtin_popcount(best_msk)) {
			best_msk = msk;
		}
bad:
		continue;
	}
	int v = best_msk;
	std::cerr << __builtin_popcount(best_msk) << '\n';
	for (int i = 0; i < N; i++) {
		for (int j = i+1; j < N; j++) {
			if (v & 1) {
				std::cerr << i << ' ' << j << '\n';
			}
			v >>= 1;
		}
	}
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	/*
	solve0(6);
	solve0(7);
	solve0(8);
	*/

	int T; cin >> T;
	while (T--) {
		int N, K; cin >> N >> K;
		if (K == N) {
			assert(N >= 3);
			cout << -1 << '\n';
		} else if (K == N-2) {
			cout << -1 << '\n';
		} else if (K > 0) {
			std::vector<std::array<int, 2>> edges;
			for (int i = 0; i < N-1; i++) {
				edges.push_back({i, N-1});
			}
			for (int i = K; i < N-1; i += 2) {
				if (i+1 == N-1) {
					assert(i > K);
					edges.push_back({i, i-1});
				} else {
					edges.push_back({i, i+1});
				}
			}
			cout << edges.size() << '\n';
			for (auto [u, v] : edges) {
				cout << u+1 << ' ' << v+1 << '\n';
			}
		} else if (N == 4 || N == 5) {
			std::vector<std::array<int, 2>> edges;
			for (int i = 0; i < N; i++) {
				edges.push_back({i, (i+1)%N});
			}
			cout << edges.size() << '\n';
			for (auto [u, v] : edges) {
				cout << u+1 << ' ' << v+1 << '\n';
			}
		} else if (N == 6) {
			std::vector<std::array<int, 2>> edges;
			edges.push_back({0, 1});
			edges.push_back({1, 2});
			edges.push_back({2, 3});
			edges.push_back({3, 4});
			edges.push_back({4, 0});
			edges.push_back({0, 5});
			edges.push_back({2, 5});
			cout << edges.size() << '\n';
			for (auto [u, v] : edges) {
				cout << u+1 << ' ' << v+1 << '\n';
			}
		} else {
			std::vector<std::array<int, 2>> edges;
			for (int i = 0; i < N-1; i++) {
				edges.push_back({i, N-1});
			}
			for (int i = K; i < N-1; i += 2) {
				if (i+1 == N-1) {
					assert(i > K);
					edges.push_back({i, i-1});
				} else {
					edges.push_back({i, i+1});
				}
			}
			cout << edges.size() << '\n';
			for (auto [u, v] : edges) {
				cout << u+1 << ' ' << v+1 << '\n';
			}
		}
	}

	return 0;
}
