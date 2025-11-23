#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, K, Q; cin >> N >> K >> Q;
		std::vector<std::array<int, 2>> c1s; c1s.reserve(Q);
		std::vector<std::array<int, 2>> c2s; c2s.reserve(Q);
		for (int q = 0; q < Q; q++) {
			int c, l, r; cin >> c >> l >> r; l--;
			if (c == 1) {
				c1s.push_back({l, r});
			} else if (c == 2) {
				c2s.push_back({l, r});
			} else assert(false);
		}
		std::vector<int> A(N, K);
		for (auto [l, r] : c2s) {
			for (int i = l; i < r; i++) A[i] = -1;
		}
		for (auto [l, r] : c1s) {
			for (int i = l; i < r; i++) if (A[i] == -1) A[i] = K+1;
		}
		int cnt = 0;
		for (int i = 0; i < N; i++) {
			if (A[i] == -1) A[i] = (cnt++) % K;
		}
		for (int i = 0; i < N; i++) {
			cout << A[i] << " \n"[i+1==N];
		}
	}

	return 0;
}
