#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		const int B = 30;
		int N, Q; cin >> N >> Q;
		std::vector<std::pair<int, int>> A(N);
		for (auto& [a, _] : A) cin >> a;
		for (int i = 0; i < N; i++) A[i].second = i;

		std::array<std::vector<std::pair<int, int>>, B> bests;
		std::vector<std::pair<int, int>> nA; nA.reserve(N);
		for (int b = 0; b < B; b++) {
			// radix-sort the whole array decreasing
			for (int z = 1; z >= 0; z--) {
				for (int i = 0; i < N; i++) {
					if (((A[i].first >> b) & 1) == z) {
						nA.push_back(A[i]);
					}
				}
			}
			std::swap(A, nA);
			nA.clear();
			bests[b] = std::vector<std::pair<int, int>>(A.begin(), A.begin() + std::min(B-b+1, N));
		}

		std::vector<bool> used(N, false);
		std::vector<int> buf; buf.reserve(B);
		for (int q = 0; q < Q; q++) {
			int c; cin >> c;
			int64_t ans = 0;
			for (int b = B-1; b >= 0; b--) {
				bool want = (c >> b) & 1;
				int i = 0;
				while (i < int(bests[b].size()) && used[bests[b][i].second]) i++;
				bool has = (i < int(bests[b].size()) && ((bests[b][i].first >> b) & 1));
				if (has && want) {
					want = false;
					i++;
					while (i < int(bests[b].size()) && used[bests[b][i].second]) i++;
					has = (i < int(bests[b].size()) && ((bests[b][i].first >> b) & 1));
				}

				if (has && !want) {
					// we're done, have an extra one
					break;
				} else if (!has && !want) {
					// just keep going
					continue;
				} else {
					assert(!has && want);
					int cur = i < int(bests[b].size()) ? (bests[b][i].first & ((1 << b) - 1)) : 0;
					ans += (1 << b) - cur;
					if (i < int(bests[b].size())) {
						buf.push_back(bests[b][i].second);
						used[bests[b][i].second] = true;
					}
				}
			}
			cout << ans << '\n';
			for (auto v : buf) used[v] = false;
		}
	}

	return 0;
}
