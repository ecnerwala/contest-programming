#include <bits/stdc++.h>

struct best_t {
	int d1, ch1, d2;
	void update(best_t o) {
		if (o.ch1 == ch1) {
			d1 = std::max(d1, o.d1);
			d2 = std::max(d2, o.d2);
		} else if (o.d1 > d1) {
			d2 = std::max(d1, o.d2);
			d1 = o.d1;
			ch1 = o.ch1;
		} else if (o.d1 > d2) {
			d2 = o.d1;
		}
		assert(d1 >= d2);
	}
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::string S; cin >> S;
		assert(int(S.size()) == N);
		for (int i = 0; i < N; i++) {
			assert(S[i] == '0' || S[i] == '1');
		}
		std::vector<std::vector<int>> adj(N);
		for (int e = 0; e < N-1; e++) {
			int u, v; cin >> u >> v; u--, v--;
			adj[u].push_back(v);
			adj[v].push_back(u);
		}

		std::vector<int> ans(N);
		for (int i = 0; i < N; i++) ans[i] = (S[i] == '0') ? -1 : 1;
		std::vector<int> min_dist(N);
		for (int i = 0; i < N; i++) {
			min_dist[i] = (S[i] == '0') ? 0 : 2 * N;
		}
		for (int l = 0; true; l++) {
			for (int i = 0; i < N; i++) {
				assert(min_dist[i] >= 0);
				min_dist[i] /= 2;
			}
			std::vector<int> n_min_dist(N, 0);

			std::vector<bool> alive(N, true);
			std::vector<int> sz(N);
			std::vector<best_t> bests; bests.reserve(N);
			[&](this auto&& rec_centroid, int rt) -> void {
				assert(alive[rt]);
				int tot_sz = [&](this auto&& self, int cur, int prv) -> int {
					if (!alive[cur]) {
						return sz[cur] = 0;
					}
					int v = 1;
					for (int nxt : adj[cur]) {
						if (nxt == prv) continue;
						v += self(nxt, cur);
					}
					return sz[cur] = v;
				}(rt, -1);
				while (true) {
					bool found = false;
					for (int nxt : adj[rt]) {
						if (alive[nxt] && sz[nxt] < sz[rt] && sz[nxt] * 2 >= tot_sz) {
							rt = nxt;
							found = true;
							break;
						}
					}
					if (!found) break;
				}

				bests.assign(tot_sz + 1, {-1, -1, -1});

				if (min_dist[rt] > 0) {
					assert(S[rt] == '1');
					bests[std::min(tot_sz, min_dist[rt])].update({0, rt, -1});
				}

				for (int ch : adj[rt]) {
					if (alive[ch]) {
						[&](this auto&& self, int cur, int prv, int depth) -> void {
							assert(alive[cur]);
							assert(depth < tot_sz);
							if (min_dist[cur] > 0 && min_dist[cur] >= depth) {
								assert(S[cur] == '1');
								bests[std::min(tot_sz, min_dist[cur] - depth)].update({depth, ch, -1});
							}
							for (int nxt : adj[cur]) {
								if (!alive[nxt]) continue;
								if (nxt == prv) continue;
								self(nxt, cur, depth + 1);
							}
						}(ch, rt, 1);
					}
				}

				for (int i = tot_sz; i > 0; i--) {
					bests[i-1].update(bests[i]);
				}

				auto query = [&](int cur, int ch, int depth) -> void {
					int d = bests[depth].ch1 == ch ? bests[depth].d2 : bests[depth].d1;
					if (d != -1) {
						n_min_dist[cur] = std::max(n_min_dist[cur], depth + d);
					}
				};

				query(rt, rt, 0);

				for (int ch : adj[rt]) {
					if (alive[ch]) {
						[&](this auto&& self, int cur, int prv, int depth) -> void {
							assert(alive[cur]);
							assert(depth < tot_sz);
							query(cur, ch, depth);
							for (int nxt : adj[cur]) {
								if (!alive[nxt]) continue;
								if (nxt == prv) continue;
								self(nxt, cur, depth + 1);
							}
						}(ch, rt, 1);
					}
				}

				bests.clear();

				alive[rt] = false;
				for (int ch : adj[rt]) {
					if (alive[ch]) {
						rec_centroid(ch);
					}
				}
			}(0);

			//for (int i = 0; i < N; i++) { cerr << n_min_dist[i] << ' '; } cerr << '\n';
			bool has = false;
			for (int i = 0; i < N; i++) {
				if (S[i] == '0') {
					min_dist[i] = 0;
				} else if (n_min_dist[i] > 0) {
					min_dist[i] = n_min_dist[i];
					assert(ans[i] == l+1);
					ans[i] = l+2;
					has = true;
				}
			}
			if (!has) break;
		}

		for (int i = 0; i < N; i++) {
			cout << (S[i] == '0' ? -1 : ans[i]) << " \n"[i+1==N];
		}
	}

	return 0;
}
