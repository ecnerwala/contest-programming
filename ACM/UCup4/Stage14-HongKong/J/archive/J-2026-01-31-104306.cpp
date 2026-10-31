#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int Q; cin >> Q;
	while (Q--) {
		int N;
		cin >> N;
		std::array<int, 3> L;
		for (auto& l : L) cin >> l;
		std::array<std::vector<int>, 3> times;
		for (auto& t : times) t.reserve(N);
		int tot_time = 0;
		for (int i = 0; i < N; i++) {
			int p, c; cin >> p >> c; p--; times[p].push_back(c);
			tot_time += c;
		}
		for (auto& t : times) std::sort(t.begin(), t.end());
		int64_t tgt_penalty; cin >> tgt_penalty;

		for (int iter = 0; tgt_penalty > 0 && iter < N; iter++) {
			int best_id = -1;
			int best_time = 0;
			for (int z = 0; z < 3; z++) {
				if (tot_time <= L[z] && !times[z].empty()) {
					int cnd = times[z].back();
					if (cnd > best_time) { best_id = z, best_time = cnd; }
				}
			}
			if (best_id == -1) {
				tgt_penalty = -1;
				break;
			}
			tgt_penalty -= tot_time;
			tot_time -= best_time;
			times[best_id].pop_back();
		}
		cout << (tgt_penalty > 0 ? "YES" : "NO") << '\n';
	}

	return 0;
}
