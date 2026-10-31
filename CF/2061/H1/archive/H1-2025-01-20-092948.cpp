#include <bits/stdc++.h>

template <typename flow_t = int, typename tot_flow_t = flow_t>
struct Dinic {
	int N;
	std::vector<std::vector<int>> adj;
	struct edge_t {
		int dest;
		flow_t cap;
	};
	std::vector<edge_t> edges;

	std::vector<char> seen;

	explicit Dinic(int N_) : N(N_), adj(N) {}

	void add_edge(int from, int to, flow_t cap) {
		return add_bi_edge(from, to, cap, 0);
	}

	void add_bi_edge(int from, int to, flow_t cap, flow_t rev_cap) {
		assert(cap >= 0);
		assert(rev_cap >= 0);
		int e = int(edges.size());
		edges.emplace_back(edge_t{to, cap});
		edges.emplace_back(edge_t{from, rev_cap});
		adj[from].push_back(e);
		adj[to].push_back(e+1);
	}

	static constexpr tot_flow_t INF_FLOW = std::numeric_limits<tot_flow_t>::max() / 4;
	std::vector<int> buf;
	std::vector<int> level;
	tot_flow_t dinic_dfs(int cur, int t, tot_flow_t f) {
		if (cur == t) return f;
		tot_flow_t cur_f = 0;
		assert(f > 0);
		for (; buf[cur] < int(adj[cur].size()); buf[cur]++) {
			int e = adj[cur][buf[cur]];
			int nxt = edges[e].dest;
			if (level[nxt] == level[cur] + 1 && edges[e].cap > 0) {
				flow_t v = flow_t(dinic_dfs(nxt, t, std::min<tot_flow_t>(f, edges[e].cap)));
				edges[e].cap -= v;
				edges[e^1].cap += v;
				f -= v;
				cur_f += v;
				if (f == 0) break;
			}
		}
		return cur_f;
	}
	tot_flow_t dinic(int s, int t) {
		tot_flow_t tot_flow = 0;
		while (true) {
			buf.clear();
			buf.reserve(N);
			level.assign(N, -1);
			buf.push_back(s);
			level[s] = 0;
			for (int z = 0; z < int(buf.size()); z++) {
				int cur = buf[z];
				for (int e : adj[cur]) {
					int nxt = edges[e].dest;
					if (edges[e].cap > 0 && level[nxt] == -1) {
						level[nxt] = level[cur] + 1;
						buf.push_back(nxt);
					}
				}
			}
			if (level[t] == -1) break;
			buf.assign(N, 0);
			tot_flow += dinic_dfs(s, t, INF_FLOW);
		}
		return tot_flow;
	}
	tot_flow_t max_flow(int s, int t) { return dinic(s, t); }
};

using std::cerr;
bool solve(std::string S, std::string T, std::vector<std::array<int, 2>> edges) {
	if (S == T) return true;
	int N = int(S.size());
	std::vector<std::vector<int>> adj(N);
	for (auto [u, v] : edges) {
		adj[u].push_back(v);
		adj[v].push_back(u);
	}

	bool can_same = true, can_flip = true;

	// if there's a legal move on each side, and connected components are good
	std::vector<bool> vis(N);
	std::vector<int> side(N);
	for (int i = 0; i < N; i++) {
		if (vis[i]) continue;
		bool is_bipartite = true;
		std::array<int, 2> S_cnt{0, 0};
		std::array<int, 2> T_cnt{0, 0};
		[&](this auto&& self, int cur, int d) -> void {
			assert(!vis[cur]);
			vis[cur] = true;
			side[cur] = d;
			S_cnt[d] += S[cur] == '1';
			T_cnt[d] += T[cur] == '1';
			for (int nxt : adj[cur]) {
				if (!vis[nxt]) self(nxt, d ^ 1);

				if (d == side[nxt]) is_bipartite = false;
			}
		}(i, 0);
		if (!(S[0] == T[0] && S[1] == T[1])) can_same = false;
		if (!(S[0] == T[1] && S[1] == T[0])) can_flip = false;
	}
	if (!can_same && !can_flip) return false;

	for (std::string cur : {S, T}) {
		Dinic flower(N * 2 + 2);
		for (auto [u, v] : edges) {
			flower.add_edge(u, N + v, 1);
			flower.add_edge(v, N + u, 1);
		}
		int tgt = 0;
		for (int i = 0; i < N; i++) {
			if (cur[i] == '1') {
				flower.add_edge(2 * N + 0, i, 1);
				tgt++;
			}
			flower.add_edge(N + i, 2 * N + 1, 1);
		}
		if (flower.max_flow(2 * N, 2 * N + 1) != tgt) {
			return false;
		}
	}

	return true;
}

void test() {
	int N = 6;
	int st = 0b000111;
	std::vector<bool> poss(1 << N); poss[st] = true;
	std::vector<int> q; q.push_back(st);
	for (int z = 0; z < int(q.size()); z++) {
		int cur = q[z];
		for (int a = cur; true; a = (a-1) & cur) {
			int b = cur ^ a;
			int nxt = (b >> 1) | (b << (N-1)) | (a >> (N-1)) | (a << 1);
			nxt &= (1 << N) - 1;
			if (!poss[nxt]) {
				poss[nxt] = true;
				q.push_back(nxt);
			}
			if (a == 0) break;
		}
	}
	for (int a = 0; a < (1 << N); a++) {
		if (__builtin_popcount(a) == __builtin_popcount(st)) {
			if (!poss[a]) {
				cerr << a << '\n';
			}
		}
	}
	exit(0);
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	//test();

	int CASES; cin >> CASES;
	while (CASES--) {
		int N, M; cin >> N >> M;
		std::string S; cin >> S;
		std::string T; cin >> T;
		std::vector<std::array<int, 2>> edges(M);
		for (auto& [u, v] : edges) { cin >> u >> v; u--, v--; }
		cout << (solve(S, T, edges) ? "Yes" : "No") << '\n';
	}

	return 0;
}
