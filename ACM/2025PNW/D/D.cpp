#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<std::vector<std::pair<int, int64_t>>> adj(N);
	for (int e = 0; e < N; e++) {
		int u, v; int d; cin >> u >> v >> d; u--, v--;
		adj[u].push_back({v, d});
		adj[v].push_back({u, d});
	}

	std::vector<std::pair<int, int64_t>> cyc; cyc.reserve(N);
	std::vector<bool> in_cyc(N, false);
	int64_t cyc_len;
	{
		std::vector<int64_t> depth(N, -1);
		depth[0] = 0;
		bool found_cyc = [&](this auto&& self, int cur, int prv) -> bool {
			cyc.push_back({cur, depth[cur]});
			for (auto [nxt, d] : adj[cur]) {
				if (nxt == prv) continue;
				if (depth[nxt] != -1) {
					int z = 0; while (cyc[z].first != nxt) z++;
					cyc.erase(cyc.begin(), cyc.begin() + z);
					int64_t d_base = cyc[0].second;
					for (auto& [v, pos] : cyc) {
						in_cyc[v] = true;
						pos -= d_base;
					}
					cyc_len = cyc.back().second + d;
					return true;
				} else {
					depth[nxt] = depth[cur] + d;
					if (self(nxt, cur)) {
						return true;
					}
				}
			}
			cyc.pop_back();
			return false;
		}(0, -1);
		assert(found_cyc);
	}

	std::vector<int64_t> dp_up(N);
	for (auto [rt, _] : cyc) {
		[&](this auto&& self, int cur, int prv) -> void {
			dp_up[cur] = 0;
			for (auto [nxt, d] : adj[cur]) {
				if (nxt == prv) continue;
				if (in_cyc[nxt]) continue;
				self(nxt, cur);
				dp_up[cur] = std::max(dp_up[cur], dp_up[nxt] + d);
			}
		}(rt, -1);
	}

	std::vector<int64_t> dp_down(N, 0);

	{
		// TODO: Do the top cycle stuff
		for (int z = 0; z < 2; z++) {
			int head = 0;
			// cyc pos, vert pos
			std::vector<std::pair<int64_t, int64_t>> q; q.reserve(2*int(cyc.size()));
			for (int iter = 0; iter < 2; iter++) {
				for (int i = 0; i < int(cyc.size()); i++) {
					int v = cyc[i].first;
					int64_t pos = iter * cyc_len + cyc[i].second;
					if (iter) {
						while (head < int(q.size()) && (pos - q[head].first) * 2 > cyc_len) {
							head++;
						}
						if (head < int(q.size())) {
							dp_down[v] = std::max(dp_down[v], pos - q[head].second);
						}
					}
					{
						int64_t cnd = pos - dp_up[v];
						while (head < int(q.size()) && cnd <= q.back().second) {
							q.pop_back();
						}
						q.push_back({pos, cnd});
					}
				}
			}
			std::reverse(cyc.begin(), cyc.end());
			for (auto& [_, pos] : cyc) pos = cyc_len - pos;
		}
	}

	std::vector<int64_t> ans(N);
	for (auto [rt, _] : cyc) {
		[&](this auto&& self, int cur, int prv) -> void {
			int64_t m1 = dp_down[cur];
			int64_t m2 = dp_down[cur];
			for (auto [nxt, d] : adj[cur]) {
				if (nxt == prv) continue;
				if (in_cyc[nxt]) continue;
				int64_t cnd = dp_up[nxt] + d;
				if (cnd > m2) {
					std::swap(m2, cnd);
					if (m2 > m1) {
						std::swap(m1, m2);
					}
				}
			}
			ans[cur] = m1;
			for (auto [nxt, d] : adj[cur]) {
				if (nxt == prv) continue;
				if (in_cyc[nxt]) continue;
				int64_t cnd = dp_up[nxt] + d;
				dp_down[nxt] = (m1 == cnd ? m2 : m1) + d;
				self(nxt, cur);
			}
		}(rt, -1);
	}
	for (int i = 0; i < N; i++) {
		cout << ans[i] << " \n"[i+1==N];
	}

	return 0;
}
