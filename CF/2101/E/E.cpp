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

std::vector<int> slow(int N, std::string S, std::vector<std::vector<int>> adj) {
	std::vector<std::vector<int>> dist(N);
	for (int i = 0; i < N; i++) {
		dist[i] = std::vector<int>(N);
		[&](this auto&& self, int cur, int prv, int depth) -> void {
			dist[i][cur] = depth;
			for (int nxt : adj[cur]) {
				if (nxt == prv) continue;
				self(nxt, cur, depth + 1);
			}
		}(i, -1, 0);
	}
	std::vector<int> min_dist(N, 2*N);
	std::vector<int> ans(N);
	for (int i = 0; i < N; i++) ans[i] = (S[i] == '1') ? 1 : -1;
	for (int l = 0; true; l++) {
		bool has = false;
		std::vector<int> n_min_dist(N, 0);
		for (int i = 0; i < N; i++) {
			if (S[i] == '1') {
				for (int j = 0; j < N; j++) {
					if (j == i) continue;
					if (S[j] == '1') {
						if (dist[i][j] * 2 <= min_dist[j]) {
							std::cerr << i << '-' << j << ' ' << l << ' ' << dist[i][j] << ' ' << min_dist[j] << '\n';
							n_min_dist[i] = std::max(n_min_dist[i], dist[i][j]);
							assert(ans[i] == l+1 || ans[i] == l+2);
							ans[i] = l+2;
							has = true;
						}
					}
				}
			}
		}
		min_dist = n_min_dist;
		if (!has) break;
	}
	return ans;
}

std::vector<int> solve(int N, std::string S, std::vector<std::vector<int>> adj) {
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
					assert(d >= 0);
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

	return ans;
}

void test() {
	std::mt19937 mt(48);
	for (int z = 0; z < 1000; z++) {
		int N = std::uniform_int_distribution<int>(1, 20)(mt);
		std::string S(N, '?');
		for (auto& c : S) {
			c = std::uniform_int_distribution<char>('0', '1')(mt);
		}
		std::vector<int> perm(N); std::iota(perm.begin(), perm.end(), 0);
		std::shuffle(perm.begin(), perm.end(), mt);
		std::vector<std::vector<int>> adj(N);
		for (int i = 1; i < N; i++) {
			int j = std::uniform_int_distribution<int>(0, i)(mt);
			adj[perm[i]].push_back(perm[j]);
			adj[perm[j]].push_back(perm[i]);
		}
		std::cerr << "test " << N << '\n';
		auto b = solve(N, S, adj);
		std::cerr << "solve done\n";
		auto a = slow(N, S, adj);
		assert(a == b);
	}
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	test();

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

		auto ans = slow(N, std::move(S), std::move(adj));

		for (int i = 0; i < N; i++) {
			cout << ans[i] << " \n"[i+1==N];
		}
	}

	return 0;
}
