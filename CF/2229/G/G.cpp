#include <bits/stdc++.h>
#include <cassert>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int T; std::cin >> T;
	while (T--) {
		int N, K, X; std::cin >> N >> K >> X; X--;
		std::vector<int> V(N); for (auto& h : V) std::cin >> h;
		std::vector<int> road_D(N-1); for (auto& d : road_D) std::cin >> d;
		std::vector<int> D(N);
		D[X] = 0;
		for (int i = X+1; i < N; i++) {
			D[i] = std::max(D[i-1] + 1, road_D[i-1]);
		}
		for (int i = X-1; i >= 0; i--) {
			D[i] = std::max(D[i+1] + 1, road_D[i]);
		}

		// we just go to bigger houses over time, and we always reach them at their opening time
		struct house_t {
			int x;
			int v;
			int d;
			int64_t pref;
			int64_t val = 0; // value of the house is v * time + val in absolute time
		};
		std::array<std::vector<house_t>, 2> H;
		H[0].reserve(N);
		H[1].reserve(N);
		for (int z = 0; z < 2; z++) {
			int64_t pref = 0;
			for (int i = X; i < N && i >= 0; i += (z ? +1 : -1)) {
				// pref is inclusive of everything
				pref += V[i];
				if (!H[z].empty() && H[z].back().v > V[i]) continue;
				H[z].push_back({std::abs(i - X), V[i], D[i], pref});
			}
		}

		// process the houses in increasing order of D
		int64_t ans = int64_t(V[X]) * int64_t(K);

		std::array<int, 2> proc_it{1, 1};
		std::array<int, 2> ins_it{0, 0};
		while (proc_it[0] < int(H[0].size()) || proc_it[1] < int(H[1].size())) {
			int z = proc_it[1] < int(H[1].size()) && (proc_it[0] == int(H[0].size()) || H[1][proc_it[1]].v < H[0][proc_it[0]].v);
			int i = proc_it[z]++;
			auto& h = H[z][i];

			{
				auto& o = H[z][i-1];
				h.val = o.val + int64_t(o.v) * int64_t(h.d - (h.x - o.x)) + (h.pref - o.pref);
			}

			while (ins_it[!z] < proc_it[!z] && H[!z][ins_it[!z]].v <= h.v) {
				int j = ins_it[!z]++;
				auto& o = H[!z][j];
				if (o.d + h.x + o.x <= h.d) {
					h.val = std::max(h.val, o.val + int64_t(o.v) * int64_t(h.d - (h.x + o.x + 1)) + o.pref + h.pref - V[X]);
				}
			}
			h.val -= int64_t(h.d) * int64_t(h.v);

			if (h.d <= K) {
				ans = std::max(ans, h.val + int64_t(h.v) * int64_t(K));
			}
		}
		std::cout << ans << '\n';
	}

	return 0;
}
