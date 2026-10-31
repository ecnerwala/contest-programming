#include <bits/stdc++.h>

namespace std {

template<class Fun>
class y_combinator_result {
	Fun fun_;
public:
	template<class T>
	explicit y_combinator_result(T &&fun): fun_(std::forward<T>(fun)) {}

	template<class ...Args>
	decltype(auto) operator()(Args &&...args) {
		return fun_(std::ref(*this), std::forward<Args>(args)...);
	}
};

template<class Fun>
decltype(auto) y_combinator(Fun &&fun) {
	return y_combinator_result<std::decay_t<Fun>>(std::forward<Fun>(fun));
}

} // namespace std

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	// edge_id (closer to you), dest
	std::vector<std::vector<std::pair<int, int>>> adj(N);
	std::vector<int> edge_end(4*N-2);
	for (int e = 0; e < N-1; e++) {
		int u, v; cin >> u >> v; u--, v--;
		adj[u].push_back({2*(N+e), v});
		adj[v].push_back({2*(N+e)+1, u});
		edge_end[2*(N+e)] = u;
		edge_end[2*(N+e)+1] = v;
	}

	// current location of the ant; < N is vert, >= N is edge
	std::vector<int> ant_loc(N);
	std::iota(ant_loc.begin(), ant_loc.end(), 0);

	std::vector<int> loc_ant(N);
	std::iota(loc_ant.begin(), loc_ant.end(), 0);

	bool is_done = false;
	int64_t num_ant_pairs = 0;
	std::vector<int> ant_par(N, -1);
	auto get_ant_par = [&](int a) -> int {
		while (ant_par[a] >= 0) {
			if (ant_par[ant_par[a]] >= 0) {
				ant_par[a] = ant_par[ant_par[a]];
			}
			a = ant_par[a];
		}
		return a;
	};
	auto merge_ants = [&](int a, int b) -> int {
		if (a == -1) return b;
		if (b == -1) return a;
		assert(ant_par[a] < 0 && ant_par[b] < 0);
		if (ant_par[a] > ant_par[b]) {
			std::swap(a, b);
		}
		num_ant_pairs += int64_t(-ant_par[a]) * int64_t(-ant_par[b]);
		is_done = num_ant_pairs == int64_t(N) * int64_t(N-1) / 2;
		ant_loc[b] = -1;
		ant_par[a] += ant_par[b];
		ant_par[b] = a;
		return a;
	};
	std::vector<int> edge_par(2*N-1, -1);
	auto get_edge_par = [&](int a) -> int {
		while (edge_par[a] >= 0) {
			if (edge_par[edge_par[a]] >= 0) {
				edge_par[a] = edge_par[edge_par[a]];
			}
			a = edge_par[a];
		}
		return a;
	};

	std::vector<std::array<int, 2>> ant_edge_neighbors(4*N-2, {-1, -1});
	for (int e = 0; e < N-1; e++) {
		ant_edge_neighbors[2*(N+e)][0] = 2*(N+e)+1;
		ant_edge_neighbors[2*(N+e)+1][0] = 2*(N+e);
	}

	auto push_ant = [&](int e, int a) {
		assert(e >= 2*N);
		assert(a < N);
		int a_old = ant_edge_neighbors[e][0];
		int a_old_d = ant_edge_neighbors[a_old][1] == e;
		ant_edge_neighbors[a_old][a_old_d] = a;
		ant_edge_neighbors[e][0] = a;

		ant_edge_neighbors[a][0] = e;
		ant_edge_neighbors[a][1] = a_old;
	};
	auto pop_ant = [&](int e) -> int {
		assert(e >= 2*N);
		int a = ant_edge_neighbors[e][0];
		assert(a != (e^1));
		int a2 = ant_edge_neighbors[a][0] ^ ant_edge_neighbors[a][1] ^ e;
		ant_edge_neighbors[a2][ant_edge_neighbors[a2][1] == a] = e;
		ant_edge_neighbors[e][0] = a2;

		ant_edge_neighbors[a][0] = -1;
		ant_edge_neighbors[a][1] = -1;
		return a;
	};
	auto edge_empty = [&](int e) {
		return ant_edge_neighbors[e][0] == (e^1);
	};

	auto dfs = std::y_combinator([&](auto self, int cur, int prvE, int prv) -> int {
		//cerr << "dfs " << cur << " <- " << prvE << " <- " << prv << '\n';
		if (loc_ant[cur] == -1) return cur;
		assert(loc_ant[cur] != -1);
		prvE ^= 1;
		// current guy goes up the stack
		{
			int ant = loc_ant[cur];
			ant_loc[ant] = prvE / 2;
			push_ant(prvE, ant);
		}

		int L = 0;
		int cur_ant = -1;
		for (int z = 0; z < int(adj[cur].size()); z++) {
			auto [e, nxt] = adj[cur][z];
			if (e == prvE) {
				assert(nxt == prv);
				continue;
			}
			nxt = self(nxt, e, cur);
			int nxt_ant = pop_ant(e);
			cur_ant = merge_ants(cur_ant, nxt_ant);
			if (loc_ant[nxt] == -1 && edge_empty(e)) {
				continue;
			}
			adj[cur][L++] = {e, nxt};
		}
		adj[cur][L++] = {prvE, prv};
		adj[cur].resize(L);


		if (cur_ant == -1) {
			assert(int(adj[cur].size()) == 1);
			// everything is empty, we're just popping the leaf return nothing
			loc_ant[cur] = -1;
			return cur;
		} else if (adj[cur].size() == 2) {
			// compress the lower edge
			auto [e, nxt] = adj[cur][!(adj[cur][1].first == prvE)];
			{
				int u = ant_edge_neighbors[e][0];
				ant_edge_neighbors[u][ant_edge_neighbors[u][1] == e] = cur_ant;
				ant_edge_neighbors[cur_ant][0] = u;
			}
			{
				int v = ant_edge_neighbors[prvE][0];
				ant_edge_neighbors[v][ant_edge_neighbors[v][1] == prvE] = cur_ant;
				ant_edge_neighbors[cur_ant][1] = v;
			}
			{
				int u = ant_edge_neighbors[e^1][0];
				ant_edge_neighbors[u][ant_edge_neighbors[u][1] == (e^1)] = prvE;
				ant_edge_neighbors[prvE][0] = u;
			}
			edge_end[prvE] = nxt;
			assert(adj[nxt].back().first == (e^1));
			adj[nxt].back() = {prvE, prv};
			ant_edge_neighbors[e^1][0] = -1;
			ant_edge_neighbors[e][0] = -1;
			edge_par[e/2] = prvE/2;
			ant_loc[cur_ant] = prvE / 2;
			loc_ant[cur] = -1;
			return nxt;
		} else {
			loc_ant[cur] = cur_ant;
			ant_loc[cur_ant] = cur;
			return cur;
		}
	});

	int Q; cin >> Q;
	while (Q--) {
		int ant_id; cin >> ant_id; ant_id--;
		ant_id = get_ant_par(ant_id);
		//cerr << "ant " << ant_id << ' ' << " at " << ant_loc[ant_id] << '\n';

		if (is_done) {
			//cerr << "is done, skipping " << '\n';
			// do nothing
		} else if (ant_loc[ant_id] < N) {
			int cur = ant_loc[ant_id];
			int L = 0;
			int cur_ant = loc_ant[cur];
			for (int z = 0; z < int(adj[cur].size()); z++) {
				auto [e, nxt] = adj[cur][z];
				nxt = dfs(nxt, e, cur);
				int nxt_ant = pop_ant(e);
				cur_ant = merge_ants(cur_ant, nxt_ant);
				if (loc_ant[nxt] == -1 && edge_empty(e)) {
					continue;
				}
				adj[cur][L++] = {e, nxt};
			}
			adj[cur].resize(L);
			loc_ant[cur] = cur_ant;
			ant_loc[cur_ant] = cur;
		} else if (ant_loc[ant_id] >= N) {
			int e = get_edge_par(ant_loc[ant_id]);
			// e is 2*e..2*e+1
			for (int z = 0; z < 2; z++) {
				int cur = edge_end[2*e+z];
				int prvE = 2*e+!z;
				int prv = edge_end[prvE];
				int ncur = dfs(cur, prvE, prv);
				for (int z = 0; z < int(adj[prv].size()); z++) {
					if (adj[prv][z].first == prvE) {
						assert(adj[prv][z].second == cur);
						adj[prv][z].second = ncur;
					}
				}
			}
			// merge the two neighboring ants into yourself
			int cur_ant = ant_id;

			int a0 = ant_edge_neighbors[ant_id][0];
			int u, ud;
			if (a0 < N) {
				cur_ant = merge_ants(cur_ant, a0);
				u = ant_edge_neighbors[a0][0] ^ ant_edge_neighbors[a0][1] ^ ant_id;
				ud = ant_edge_neighbors[u][1] == a0;
			} else {
				u = a0;
				ud = 0;
				assert(ant_edge_neighbors[u][ud] == ant_id);
			}

			int a1 = ant_edge_neighbors[ant_id][1];
			int v, vd;
			if (a1 < N) {
				cur_ant = merge_ants(cur_ant, a1);
				v = ant_edge_neighbors[a1][0] ^ ant_edge_neighbors[a1][1] ^ ant_id;
				vd = ant_edge_neighbors[v][1] == a1;
			} else {
				v = a1;
				vd = 0;
				assert(ant_edge_neighbors[v][vd] == ant_id);
			}

			ant_edge_neighbors[u][ud] = cur_ant;
			ant_edge_neighbors[v][vd] = cur_ant;
			ant_edge_neighbors[cur_ant][0] = u;
			ant_edge_neighbors[cur_ant][1] = v;
		} else assert(false);
		//cerr << num_ant_pairs << '\n' << flush;
		cout << num_ant_pairs << '\n';
	}

	return 0;
}
