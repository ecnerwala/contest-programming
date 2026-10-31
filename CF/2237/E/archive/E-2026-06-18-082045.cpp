#include <bits/stdc++.h>
#include <cassert>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int T; std::cin >> T;
	while (T--) {
		int N; std::cin >> N;
		std::vector<int> A(N);
		for (auto& a : A) { std::cin >> a; a--; }
		std::vector<int> B(N);
		for (auto& b : B) { std::cin >> b; if (b != -1) b--; }

		if ([&]() -> bool {
			std::vector<bool> used_val(N, false);
			for (auto b : B) { if (b != -1) used_val[b] = true; }
			std::vector<bool> vis(N, false);
			std::vector<std::vector<int>> unused_ind(N+1);
			std::vector<std::vector<int>> unused_val(N+1);
			for (int i = 0; i < N; i++) {
				if (vis[i]) continue;
				int l = 0;
				int j = i;
				bool has_ind = false, has_val = false;
				do {
					l++;
					if (B[j] != -1) has_ind = true;
					if (used_val[j]) has_val = true;
					j = A[j];
				} while (j != i);
				if (!has_ind) unused_ind[l].push_back(i);
				if (!has_val) unused_val[l].push_back(i);
			}
			for (int l = 1; l <= N; l++) {
				if (int(unused_ind[l].size()) != int(unused_val[l].size())) return false;
				for (int z = 0; z < int(unused_ind[l].size()); z++) {
					B[unused_ind[l][z]] = unused_val[l][z];
				}
			}
			for (int st = 0; st < N; st++) {
				if (B[st] == -1) continue;
				for (int i = st; true; ) {
					if (B[A[i]] == -1) {
						B[A[i]] = A[B[i]];
						i = A[i];
						continue;
					} else {
						if (B[A[i]] != A[B[i]]) return false;
						else break;
					}
					assert(false);
				}
			}
			return true;
		}()) {
			std::cout << "YES" << '\n';
			for (int i = 0; i < N; i++) std::cout << B[i]+1 << " \n"[i+1==N];
		} else {
			std::cout << "NO" << '\n';
		}
	}

	return 0;
}
