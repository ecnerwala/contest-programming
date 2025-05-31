#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, Q; cin >> N >> Q;
		std::vector<int> B(N);
		for (auto& b : B) cin >> b;
		int V = N+Q;
		std::vector<int> cur_vars(N);
		std::iota(cur_vars.begin(), cur_vars.end(), 0);
		std::vector<std::vector<int>> in_edges(V);
		std::vector<std::vector<int>> out_edges(V);
		std::vector<int> indeg(V);
		for (int i = 0; i < N; i++) {
			// dummy indeg
			indeg[i] = 1;
		}
		for (int q = 0; q < Q; q++) {
			int v = N+q;
			int x, y, z; cin >> x >> y >> z; x--, y--, z--;
			in_edges[v].push_back({cur_vars[x]});
			in_edges[v].push_back({cur_vars[y]});
			out_edges[cur_vars[x]].push_back(v);
			out_edges[cur_vars[y]].push_back(v);
			cur_vars[z] = q;
			indeg[v] = 2;
		}
		std::vector<bool> alive(V, true);
		std::vector<int> ans(V);
		std::vector<std::pair<int, int>> evts(N);
		for (int i = 0; i < N; i++) evts[i] = {B[i], cur_vars[i]};
		std::sort(evts.begin(), evts.end(), std::greater<>());

		if ([&]() -> bool {
			std::vector<int> q; q.reserve(V);
			for (auto [v, st] : evts)  {
				if (!alive[st]) {
					if (ans[st] == v) continue;
					else return false;
				}
				q.clear();
				alive[st] = false;
				ans[st] = v;
				q.push_back(st);
				while (!q.empty()) {
					int cur = q.back(); q.pop_back();
					for (int nxt : in_edges[cur]) {
						if (alive[nxt]) {
							alive[nxt] = false;
							ans[nxt] = v;
							q.push_back(nxt);
						}
					}
					for (int nxt : out_edges[cur]) {
						--indeg[nxt];
						if (indeg[nxt] == 0) {
							// we're the last parent, so we must set this guy
							assert(nxt >= N);
							if (alive[nxt]) {
								alive[nxt] = false;
								ans[nxt] = v;
								q.push_back(nxt);
							}
						}
					}
				}
			}
		}()) {
			for (int i = 0; i < N; i++) cout << ans[i] << " \n"[i+1==N];
		} else {
			cout << -1 << '\n';
		}
	}

	return 0;
}
