#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int64_t> C(N);
		for (auto& c : C) cin >> c;
		std::vector<int64_t> V(N);
		for (auto& v : V) cin >> v;

		// lowest value
		int64_t best_next = 0;
		int64_t next_val = 0;
		for (int i = N-1; i >= 0; i--) {
			int64_t v = V[i];
			int64_t c = C[i];
			std::map<int64_t, int64_t> vis;
			int64_t a = next_val;
			for (int64_t z = 0; z < c; z++) {
				if (vis.count(a)) {
					int64_t cyc = z - vis[a];
					z += (c - 1 - z) / cyc * cyc;
				} else {
					vis[a] = z;
				}
				a = std::max(v - a, best_next);
			}

			next_val = a;
			best_next = std::max(best_next, a);
		}
		cout << next_val << '\n';
	}

	return 0;
}
