#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::array<std::vector<pair<int, int>>, 3> P;
		for (int z = 0; z < 3; z++) {
			P[z].resize(N);
		}
		for (int i = 0; i < N; i++) {
			for (int z = 0; z < 3; z++) {
				int v; cin >> v;
				P[z][i] = {v, i};
			}
		}
		std::vector<std::array<int, 3>> loc(N);
		for (int z = 0; z < 3; z++) {
			std::sort(P[z].begin(), P[z].end());
			for (int i = 0; i < N; i++) {
				loc[P[z][i].second][z] = i;
			}
		}

		// if we can toposort the people obeying
		auto is_good = [&](int k) -> bool {
			struct dim_t {
				std::vector<bool> alive;
				int first_alive;
				int nxt_banned;
			};
			std::array<dim_t, 3> dims;
			std::vector<int8_t> cnt_good(N, 3);
			std::vector<int> q; q.reserve(N);
			for (int z = 0; z < 3; z++) {
				dims[z].alive.assign(N, true);
				dims[z].first_alive = 0;
				dims[z].nxt_banned = 0;
				while (dims[z].nxt_banned < N && P[z][dims[z].nxt_banned].first - P[z][dims[z].first_alive].first <= 2 * k) {
					int i = P[z][dims[z].nxt_banned++].second;
					if (!--cnt_good[i]) q.push_back(i);
				}
			}
			for (int it = 0; it < int(q.size()); it++) {
				int cur = q[it];
				for (int z = 0; z < 3; z++) {
					int v = loc[cur][z];
					dims[z].alive[v] = false;
					if (dims[z].first_alive == v) {
						dims[z].first_alive++;
						while (dims[z].first_alive < N && !dims[z].alive[dims[z].first_alive]) dims[z].first_alive++;
						while (dims[z].nxt_banned < N && (dims[z].first_alive == N || P[z][dims[z].nxt_banned].first - P[z][dims[z].first_alive].first <= 2 * k)) {
							int i = P[z][dims[z].nxt_banned++].second;
							if (!--cnt_good[i]) q.push_back(i);
						}
					}
				}
			}
			return int(q.size()) == N;
		};

		int mi = -1;
		int ma = int(1e9) + 100;
		while (ma - mi > 1) {
			int md = mi + (ma - mi) / 2;
			if (is_good(md)) ma = md;
			else mi = md;
		}
		cout << ma << '\n';
	}

	return 0;
}
