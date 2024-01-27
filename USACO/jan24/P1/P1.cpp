#include <bits/stdc++.h>

int mod_inv_in_range(int a, int m) {
	return a == 1 ? 1 : m - int(int64_t(mod_inv_in_range(m % a, a)) * m / a);
}

template <int MOD_> struct modnum {
	static const int MOD = MOD_;
	int v;
	modnum() : v(0) {}
	explicit modnum(int64_t a) : v(int(a % MOD)) { if (v < 0) v += MOD; }
	explicit operator int() const { return v; }

	friend std::ostream& operator << (std::ostream& out, const modnum& n) { return out << int(n); }

	modnum& operator += (modnum o) { v -= MOD - o.v; v = (v < 0) ? v + MOD : v; return *this; }
	friend modnum operator + (modnum a, modnum b) { return a += b; }
	modnum& operator -= (modnum o) { v -= o.v; v = (v < 0) ? v + MOD : v; return *this; }
	friend modnum operator - (modnum a, modnum b) { return a -= b; }

	modnum& operator *= (modnum o) {
		v = int(int64_t(v) * int64_t(o.v) % MOD);
		return *this;
	}
	friend modnum operator * (modnum a, modnum b) { return a *= b; }

	friend modnum inv(modnum a) {
		modnum r;
		r.v = mod_inv_in_range(a.v, MOD);
		return r;
	}
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M; cin >> N >> M;

		using num = modnum<int(1e9) + 7>;
		std::vector<num> P(N);
		for (auto& p : P) cin >> p.v;

		std::vector<std::vector<int>> adj(N);
		for (int e = 0; e < M; e++) {
			int u, v; cin >> u >> v; u--, v--;
			adj[u].push_back(v);
			adj[v].push_back(u);
		}

		std::vector<num> invs(N+1);
		{
			num f = num(1);
			for (int i = 1; i <= N; i++) {
				invs[i] = f;
				f *= num(i);
			}
			f = inv(f);
			for (int i = N; i >= 1; i--) {
				invs[i] *= f;
				f *= num(i);
			}
		}

		std::vector<std::vector<int>> subchains(N);
		std::vector<std::vector<int>> chains; chains.reserve(N);
		{
			std::vector<int> depth(N, -1);
			std::vector<int> low_val(N, -1);
			std::vector<int> par(N, -1);
			std::vector<int> stk; stk.reserve(N);
			auto dfs = [&](auto self, int cur, int prv) -> void {
				par[cur] = prv;
				assert(depth[cur] == -1);
				depth[cur] = (prv == -1) ? 0 : depth[prv] + 1;
				low_val[cur] = depth[cur];
				stk.push_back(cur);
				for (int nxt : adj[cur]) {
					if (nxt == prv) continue;
					if (depth[nxt] == -1) {
						self(self, nxt, cur);
						if (low_val[nxt] >= depth[cur]) {
							subchains[cur].push_back(int(chains.size()));
							chains.emplace_back(stk.end() - (depth[nxt] - depth[cur]), stk.end());
							stk.resize(int(stk.size()) - (depth[nxt] - depth[cur]));
						} else {
							assert(low_val[cur] == depth[cur]);
							low_val[cur] = low_val[nxt];
						}
					} else if (depth[nxt] < depth[cur]) {
						assert(low_val[cur] == depth[cur]);
						low_val[cur] = depth[nxt];
					} else {
						// do nothing
					}
				}
			};
			dfs(dfs, 0, -1);
			assert(int(stk.size()) == 1);
		}
		int C = int(chains.size());

		std::vector<bool> has_out(N);
		std::vector<num> vert_escape(N);
		std::vector<num> chain_escape(C);
		{
			auto dfs = [&](auto self, int v, bool cur_has_out) -> void {
				has_out[v] = cur_has_out;
				std::vector<num> subchain_escape; subchain_escape.reserve(subchains.size() + 1);
				subchain_escape.push_back(num(1));

				int num_bridge = 0;
				for (int c : subchains[v]) {
					bool is_bridge = int(chains[c].size()) == 1;
					chain_escape[c] = num(1);
					for (int u : chains[c]) {
						self(self, u, !is_bridge);
						chain_escape[c] *= vert_escape[u];
					}
					if (cur_has_out) {
						if (is_bridge) {
							num_bridge++;
						} else {
							subchain_escape.push_back(chain_escape[c]);
							for (int z = int(subchain_escape.size()) - 1; z > 0; z--) {
								subchain_escape[z] += subchain_escape[z-1] * chain_escape[c];
							}
						}
					}
				}
				if (!cur_has_out) {
					vert_escape[v] = num(0);
				} else {
					// we have some number of chains,
					int num_edges = 1 + 2 * (int(subchain_escape.size()) - 1) + num_bridge;
					assert(num_edges + 1 == int(adj[v].size()));
				}
			};
			dfs(dfs, 0, false);
		}
	}

	return 0;
}
