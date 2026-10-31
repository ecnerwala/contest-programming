#include<bits/stdc++.h>

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

void modify(int p);
int query(int p);
void report(int x, int y);
int check(int x);

void explore_quadratic(int N, int M) {
	std::vector<bool> cur_state(N);
	for (int i = 0; i+1 < N; i++) {
		modify(i);
		for (int j = i+1; j < N; j++) {
			if (query(j) != cur_state[j]) {
				cur_state[j] = !cur_state[j];
				report(i, j);
			}
		}
	}
}

void explore_pairs(int N, int M) {
	assert(M * 2 == N);
	std::vector<int> lefts; lefts.reserve(N/2);
	std::vector<int> rights; rights.reserve(N/2);
	for (int i = 0; i < N; i++) {
		if (query(i)) {
			rights.push_back(i);
		} else {
			modify(i);
			lefts.push_back(i);
		}
	}
	std::vector<bool> state(N/2, true);
	std::vector<int> rtol(N/2, 0);
	for (int l = 0; (1 << l) < N/2; l++) {
		for (int i = 0; i < N/2; i++) {
			if (i & (1 << l)) {
				modify(lefts[i]);
			}
		}
		for (int i = 0; i < N/2; i++) {
			if (query(rights[i]) != state[i]) {
				state[i] = !state[i];
				rtol[i] |= (1 << l);
			}
		}
	}
	for (int i = 0; i < N/2; i++) {
		report(lefts[rtol[i]], rights[i]);
	}
}

void explore_sorted_tree(int N, int M) {
	assert(M == N-1);
	std::vector<int> par(N, 0);
	for (int l = 0; (1 << l) < N; l++) {
		for (int i = 0; i < N; i++) {
			if (i && query(i)) {
				par[i] |= (1 << l);
			}
			if (i & (1 << l)) {
				modify(i);
			}
		}
		for (int i = 0; i < N; i++) {
			if (i & (1 << l)) {
				modify(i);
			}
		}
	}
	for (int i = 1; i < N; i++) {
		report(par[i], i);
	}
}

void explore_general(int N, int M) {
	std::mt19937 mt(48);
	std::vector<bool> is_flipped(N, false);
	std::vector<bool> is_done(N, false);
	std::vector<std::vector<int>> known_neighbors(N);
	for (int i = 0; i < N; i++) known_neighbors[i].push_back(i);
	//std::vector<bool> known_state(N, false);
	auto my_modify = [&](int x) {
		modify(x);
		is_flipped[x] = !is_flipped[x];
		//for (int y : known_neighbors[x]) known_state[y] = !known_state[y];
	};
	auto my_query = [&](int x) -> bool {
		bool res = query(x);
		for (int y : known_neighbors[x]) res ^= is_flipped[y];
		return res;
	};
	int cur_edges = 0;
	auto my_report = [&](int x, int y) {
		cur_edges++;
		report(x, y);
		is_done[x] = check(x);
		is_done[y] = check(y);
		known_neighbors[y].push_back(x);
		//if (is_flipped[x]) known_state[y] = !known_state[y];
		known_neighbors[x].push_back(y);
		//if (is_flipped[y]) known_state[x] = !known_state[x];
	};

	while (cur_edges < M) {
		std::vector<int> alives; alives.reserve(N);
		for (int i = 0; i < N; i++) {
			if (!is_done[i]) alives.push_back(i);
		}
		std::shuffle(alives.begin(), alives.end(), mt);

		std::vector<int> all_cnds; all_cnds.reserve(alives.size());
		for (int i = 0; i < int(alives.size()); i++) {
			my_modify(alives[i]);
			if (my_query(alives[i])) {
				all_cnds.push_back(i);
			}
		}
		std::y_combinator([&](auto self, int l, int r, std::vector<int> cnds) -> void {
			if (r - l == 1) {
				for (auto v : cnds) {
					my_report(alives[l], alives[v]);
				}
				if (is_flipped[alives[l]]) {
					my_modify(alives[l]);
				}
				assert(!is_flipped[alives[l]]);
				return;
			}

			int m = (l + r) / 2;
			for (int i = l; i < r; i++) {
				if (is_flipped[alives[i]] != (i < m)) {
					my_modify(alives[i]);
				}
				assert(is_flipped[alives[i]] == (i < m));
			}
			std::vector<int> lo_cnds; lo_cnds.reserve(cnds.size());
			std::vector<int> hi_cnds; hi_cnds.reserve(cnds.size());
			for (int v : cnds) {
				if (v <= m || my_query(alives[v])) lo_cnds.push_back(v);
				else hi_cnds.push_back(v);
			}
			self(l, m, lo_cnds);
			self(m, r, hi_cnds);
			assert(!is_flipped[alives[l]]);
		})(0, int(alives.size()), all_cnds);
	}
}

void explore_tree(int N, int M) {
	assert(M == N-1);
	explore_general(N, M);
}

void explore_path(int N, int M) {
	assert(M == N-1);
	explore_tree(N, M);
}

void explore(int N, int M) {
	if (N <= 500) {
		explore_quadratic(N, M);
		return;
	}
	if (M * 2 == N) {
		explore_pairs(N, M);
		return;
	}
	if (N % 10 == 7) {
		explore_sorted_tree(N, M);
		return;
	}
	if (N % 10 == 6) {
		explore_path(N, M);
		return;
	}
	if (N % 10 == 5) {
		explore_tree(N, M);
		return;
	}
	explore_general(N, M);
}

#ifdef ECNERWALA
namespace ecnerwala {
using namespace std;
int N, M;
set<pair<int, int>> edges;

vector<vector<int>> adj;
vector<bool> cur_state;
set<pair<int, int>> reported_edges;
vector<int> reported_deg;

void init() {
	adj = vector<vector<int>>(N);
	for (auto [u, v] : edges) {
		adj[u].push_back(v);
		adj[v].push_back(u);
	}
	cur_state = vector<bool>(N, false);
	reported_edges = set<pair<int, int>>();
	reported_deg = vector<int>(N, 0);
}
}
void modify(int p) {
	using namespace ecnerwala;
	cur_state[p] = !cur_state[p];
	for (int o : adj[p]) cur_state[o] = !cur_state[o];
}
int query(int p) {
	using namespace ecnerwala;
	return cur_state[p];
}
void report(int x, int y) {
	using namespace ecnerwala;
	if (x > y) swap(x, y);
	assert(x < y);
	assert(edges.count({x, y}));
	assert(!reported_edges.count({x, y}));
	reported_edges.insert({x, y});
	reported_deg[x]++;
	reported_deg[y]++;
}
int check(int x) {
	using namespace ecnerwala;
	return reported_deg[x] == int(adj[x].size());
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	using namespace ecnerwala;
	N = 5;
	M = 7;
	mt19937 mt(10932);
	edges.clear();
	while (int(edges.size()) < M) {
		int u = int(mt() % N);
		int v = int(mt() % (N-1)); if (v >= u) v++;
		assert(u != v);
		if (u > v) swap(u, v);
		assert(u < v);
		edges.insert({u, v});
	}
	init();
	explore_general(N, M);
	assert(reported_edges == edges);

	return 0;
}
#endif
