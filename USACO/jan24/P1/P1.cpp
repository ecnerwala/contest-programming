#include <bits/stdc++.h>

void wssert(bool b) {
	if (!b) {
		std::cout << "GARBAGE" << '\n';
		exit(0);
	}
}

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

using num = modnum<int(1e9) + 7>;
std::vector<num> solve(int N, std::vector<num> P, std::vector<std::vector<int>> adj) {
	using namespace std;

	const int V = 3*N + 10;
	std::vector<num> fact(V+1);
	fact[0] = num(1);
	for (int i = 1; i <= V; i++) fact[i] = fact[i-1] * num(i);
	std::vector<num> ifact(V+1);
	ifact[V] = inv(fact[V]);
	std::vector<num> invs(V+1);
	for (int i = V; i >= 1; i--) {
		ifact[i-1] = ifact[i] * num(i);
		invs[i] = ifact[i] * fact[i-1];
	}

	std::vector<std::vector<int>> subchains(N);
	std::vector<std::vector<int>> chains; chains.reserve(N);
	{
		std::vector<int> depth(N, -1);
		std::vector<int> low_val(N, -1);
		std::vector<int> par(N, -1);
		std::vector<int> stk; stk.reserve(N);
		auto dfs = [&](auto self, int cur, int prv) -> void {
			//cerr << "dfs " << cur << ' ' << prv << '\n';
			par[cur] = prv;
			wssert(depth[cur] == -1);
			depth[cur] = (prv == -1) ? 0 : depth[prv] + 1;
			low_val[cur] = depth[cur];
			stk.push_back(cur);
			subchains[cur].reserve(adj[cur].size());
			for (int nxt : adj[cur]) {
				if (nxt == prv) continue;
				if (depth[nxt] == -1) {
					int idx_cur = int(stk.size());
					self(self, nxt, cur);
					if (low_val[nxt] >= depth[cur]) {
						subchains[cur].push_back(int(chains.size()));
						chains.emplace_back();
						chains.back().reserve(int(stk.size()) - idx_cur);
						chains.back().insert(chains.back().end(), stk.begin() + idx_cur, stk.end());
						stk.resize(idx_cur);
						//cerr << "chain " << int(chains.size())-1 << ": "; cerr << cur << ' '; for (int v : chains.back()) { cerr << v << ' '; } cerr << '\n';
					} else {
						wssert(low_val[cur] == depth[cur]);
						low_val[cur] = low_val[nxt];
					}
				} else if (depth[nxt] < depth[cur]) {
					wssert(low_val[cur] == depth[cur]);
					low_val[cur] = depth[nxt];
				} else {
					// do nothing
				}
			}
		};
		dfs(dfs, 0, -1);
		wssert(int(stk.size()) == 1);
	}
	int C = int(chains.size());

	std::vector<bool> has_out(N);
	std::vector<num> vert_escape(N);
	std::vector<num> chain_escape(C);
	std::vector<std::vector<num>> subchain_escape(N);
	std::vector<int> num_bridge(N);
	{
		auto dfs = [&](auto self, int v, bool cur_has_out) -> void {
			has_out[v] = cur_has_out;
			subchain_escape[v].reserve(subchains.size() + 1);
			subchain_escape[v].push_back(num(1));

			for (int c : subchains[v]) {
				bool is_bridge = int(chains[c].size()) == 1;
				chain_escape[c] = num(1);
				for (int u : chains[c]) {
					self(self, u, !is_bridge);
					chain_escape[c] *= vert_escape[u];
				}
				//cerr << "chain " << c << " " << chain_escape[c] << '\n';
				if (is_bridge) {
					num_bridge[v]++;
				} else {
					subchain_escape[v].push_back(num(0));
					for (int z = int(subchain_escape[v].size()) - 1; z > 0; z--) {
						subchain_escape[v][z] += subchain_escape[v][z-1] * chain_escape[c];
					}
				}
			}
			if (!cur_has_out) {
				vert_escape[v] = num(0);
			} else {
				// we have some number of chains,
				int cycs = int(subchain_escape[v].size()) - 1;
				int num_edges = 1 + 2 * cycs + num_bridge[v];
				wssert(num_edges + 1 == int(adj[v].size()));
				num val = num(0);
				num w = num(1);
				for (int num_cycs = 0; num_cycs <= cycs; num_cycs++) {
					w *= invs[num_edges - num_cycs * 2];
					w *= num(1) - P[v];
					if (num_cycs) {
						w *= num(2 * num_cycs);
					}
					val += w * subchain_escape[v][num_cycs];
				}
				vert_escape[v] = val;
				//cerr << "vert " << v << ' ' << vert_escape[v] << '\n';
			}
		};
		dfs(dfs, 0, false);
	}

	std::vector<num> ans(N);
	{
		auto dfs = [&](auto self, int v, num p_reach_v) -> void {
			//cerr << "reach v " << v << ' ' << p_reach_v << '\n';
			// what's the chance we get stuck here
			{
				int bridges = num_bridge[v];
				int cycs = int(subchain_escape[v].size()) - 1;
				int num_edges = has_out[v] + 2 * cycs + bridges;

				num p_stay = num(0);
				num w = p_reach_v;
				for (int num_cycs = 0; num_cycs <= cycs; num_cycs++) {
					if (num_cycs) {
						w *= num(2 * num_cycs);
						w *= invs[num_edges - (num_cycs - 1) * 2];
						w *= num(1) - P[v];
					}
					if (num_cycs * 2 != num_edges) {
						p_stay += w * subchain_escape[v][num_cycs] * P[v];
					} else {
						p_stay += w * subchain_escape[v][num_cycs];
					}
				}
				ans[v] = p_stay;
				//cerr << "stay " << v << ' ' << ans[v] << '\n';
			}
			for (int c : subchains[v]) {
				num p_reach_c = num(0);

				{
					bool is_bridge = int(chains[c].size()) == 1;
					int bridges = num_bridge[v];
					int cycs = int(subchain_escape[v].size()) - 1;
					int num_edges = has_out[v] + 2 * cycs + bridges;

					if (!is_bridge) {
						// all the other chain
						for (int z = 1; z < int(subchain_escape[v].size()); z++) {
							subchain_escape[v][z] -= subchain_escape[v][z-1] * chain_escape[c];
						}
						subchain_escape[v].pop_back();
						cycs--;
					} else {
						bridges--;
					}

					num w = p_reach_v;
					for (int num_cycs = 0; num_cycs <= cycs; num_cycs++) {
						w *= invs[num_edges - num_cycs * 2];
						w *= num(1) - P[v];
						if (num_cycs) {
							w *= num(2 * num_cycs);
						}
						p_reach_c += w * subchain_escape[v][num_cycs];
					}

					// restore
					if (!is_bridge) {
						subchain_escape[v].push_back(num(0));
						for (int z = int(subchain_escape[v].size()) - 1; z > 0; z--) {
							subchain_escape[v][z] += subchain_escape[v][z-1] * chain_escape[c];
						}
					} else {
						p_reach_c *= invs[2];
					}
				}

				//cerr << "reach c " << c << ' ' << p_reach_c << '\n';
				// go from the front and the back
				std::vector<num> p_reach_u(chains[c].size());
				{
					num w = p_reach_c;
					for (int i = 0; i < int(chains[c].size()); i++) {
						p_reach_u[i] += w;
						w *= vert_escape[chains[c][i]];
					}
				}
				{
					num w = p_reach_c;
					for (int i = int(chains[c].size()) - 1; i >= 0; i--) {
						p_reach_u[i] += w;
						w *= vert_escape[chains[c][i]];
					}
				}
				for (int i = 0; i < int(chains[c].size()); i++) {
					self(self, chains[c][i], p_reach_u[i]);
				}
			}
		};
		dfs(dfs, 0, num(1));
	}

	return ans;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	std::mt19937 mt(48);
	{
		for (int z = 0; z < 1; z++) {
			//int N = std::uniform_int_distribution<int>(2, 1e4)(mt);
			int N = 1e4;
			cerr << "TEST " << z << ' ' << N << '\n';
			std::vector<num> P(N);
			for (auto& p : P) p = num(std::uniform_int_distribution<int>(0, 5)(mt));
			std::vector<std::vector<int>> adj(N);
			for (int i = 1; i < N; i++) {
				int j = std::uniform_int_distribution<int>(0, i-1)(mt);
				adj[j].push_back(i);
				adj[i].push_back(j);
			}
			solve(N, std::move(P), std::move(adj));
		}
	}

	int T; cin >> T;
	while (T--) {
		int N, M; cin >> N >> M;

		std::vector<num> P(N);
		for (auto& p : P) cin >> p.v;

		std::vector<std::vector<int>> adj(N);
		for (int e = 0; e < M; e++) {
			int u, v; cin >> u >> v; u--, v--;
			//cerr << "edge " << u << ' ' << v << '\n';
			adj[u].push_back(v);
			adj[v].push_back(u);
		}

		auto ans = solve(N, std::move(P), std::move(adj));

		for (int i = 0; i < N; i++) {
			cout << ans[i] << " \n"[i+1==N];
		}
	}

	return 0;
}
