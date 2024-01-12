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

struct tree_solver {
	int N;
	std::vector<int> parity;
	std::vector<int> par;
	std::vector<bool> is_leaf;

	int t = 0;
	std::vector<int> last_alive_time;
	std::vector<bool> last_alive_val;
	std::vector<int> leaf_alive_time;
	std::vector<bool> leaf_alive_val;

	std::vector<int> alive_leaves;
	std::vector<int> dead_leaves;
	std::vector<std::set<int>> alive_ch;
	std::vector<std::set<int>> dead_ch;

	std::vector<std::set<int>> isolated_ch;

	std::set<int> to_mark;
	std::set<int> to_ch_mark;

	std::array<int, 2> alive_cnt{0, 0};
	int cur_parity;

	bool is_alive(int a) {
		if (is_leaf[a]) {
			int p = par[a];
			assert(p != -1);
			return leaf_alive_time[p] <= last_alive_time[a] ? leaf_alive_val[p] : last_alive_val[a];
		} else {
			return last_alive_val[a];
		}
	}

	tree_solver() {}
	tree_solver(const std::vector<std::vector<int>>& adj, int initial_parity) {
		N = int(adj.size());
		parity = std::vector<int>(N);
		par = std::vector<int>(N);
		is_leaf = std::vector<bool>(N);
		alive_leaves = std::vector<int>(N);
		dead_leaves = std::vector<int>(N);
		alive_ch = std::vector<std::set<int>>(N);
		dead_ch = std::vector<std::set<int>>(N);
		last_alive_time = std::vector<int>(N, -1);
		last_alive_val = std::vector<bool>(N, false);
		leaf_alive_time = std::vector<int>(N, -1);
		leaf_alive_val = std::vector<bool>(N, false);
		isolated_ch = std::vector<std::set<int>>(N);
		std::y_combinator([&](auto self, int cur, int prv) -> void {
			parity[cur] = (prv == -1) ? 0 : !parity[prv];
			par[cur] = prv;
			is_leaf[cur] = true;
			for (int nxt : adj[cur]) {
				if (nxt == prv) continue;
				is_leaf[cur] = false;
				self(nxt, cur);
				if (is_leaf[nxt]) {
					dead_leaves[cur]++;
				} else {
					dead_ch[cur].insert(nxt);
				}
			}
		})(0, -1);

		cur_parity = initial_parity;
	}

	void check_stuff(int a) {
		if (a == -1) return;
		if (is_leaf[a]) {
			return;
		}
		if (is_alive(a) && (dead_leaves[a] + int(dead_ch[a].size()) > 0)) {
			std::cerr << " +to_ch_mark " << a << '\n';
			to_ch_mark.insert(a);
		} else {
			std::cerr << " -to_ch_mark " << a << '\n';
			to_ch_mark.erase(a);
		}
		if (!is_alive(a) && (alive_leaves[a] + int(alive_ch[a].size()) > 0)) {
			std::cerr << " +to_mark " << a << '\n';
			to_mark.insert(a);
		} else {
			std::cerr << " -to_mark " << a << '\n';
			to_mark.erase(a);
		}
		if (is_alive(a) && (alive_leaves[a] + int(alive_ch[a].size()) == 0)) {
			if (par[a] != -1) {
				std::cerr << " +isolated_ch " << a << '\n';
				isolated_ch[par[a]].insert(a);
			}
		} else {
			if (par[a] != -1) {
				std::cerr << " -isolated_ch " << a << '\n';
				isolated_ch[par[a]].erase(a);
			}
		}
	}

	void do_unmark(int a) {
		std::cerr << "maybe unmarking " << a << '\n';
		t++;
		assert(parity[a] == cur_parity);
		if (!is_alive(a)) return;
		std::cerr << "unmarking " << a << '\n';
		last_alive_time[a] = t;
		last_alive_val[a] = false;
		assert(!is_alive(a));
		alive_cnt[parity[a]]--;
		int p = par[a];

		// Check if p is now isolated
		if (p != -1) {
			if (is_leaf[a]) {
				alive_leaves[p]--;
				dead_leaves[p]++;
			} else {
				alive_ch[p].erase(a);
				dead_ch[p].insert(a);
			}
			if (is_alive(p) && (alive_leaves[p] + int(alive_ch[p].size()) == 0) && (par[p] == -1 || !is_alive(par[p]))) {
				// p is isolated, kill it
				last_alive_time[p] = t;
				last_alive_val[p] = false;
				alive_cnt[parity[p]]--;
				int g = par[p];
				if (g != -1) {
					assert(!is_leaf[p]);
					alive_ch[g].erase(p);
					dead_ch[g].insert(p);
				}
				check_stuff(g);
			}
		}
		for (int c : isolated_ch[a]) {
			assert(!is_leaf[c]);
			assert(is_alive(c));
			last_alive_time[c] = t;
			last_alive_val[c] = false;
			alive_ch[a].erase(c);
			dead_ch[a].insert(c);
			alive_cnt[parity[c]]--;
			check_stuff(c);
		}
		// Handle all leaves in bulk
		{
			leaf_alive_time[a] = t;
			leaf_alive_val[a] = false;
			dead_leaves[a] += alive_leaves[a];
			alive_cnt[!parity[a]] -= alive_leaves[a];
			alive_leaves[a] = 0;
		}
		check_stuff(a);
		check_stuff(p);
	}

	void do_mark(int a) {
		t++;
		std::cerr << "maybe marking " << a << '\n';
		assert(parity[a] == cur_parity);
		if (is_alive(a)) return;
		std::cerr << "marking " << a << '\n';
		last_alive_time[a] = t;
		last_alive_val[a] = true;
		assert(is_alive(a));
		int p = par[a];
		alive_cnt[parity[a]]++;
		if (p != -1) {
			if (is_leaf[a]) {
				dead_leaves[p]--;
				alive_leaves[p]++;
			} else {
				dead_ch[p].erase(a);
				alive_ch[p].insert(a);
			}
			check_stuff(p);
		}
		check_stuff(a);
	}

	void do_flip() {
		std::cerr << "do_flip" << '\n';
		t++;
		cur_parity ^= 1;
		std::set<int> nxt_check;
		for (int a : to_ch_mark) {
			assert(is_alive(a));
			assert(dead_leaves[a] + int(dead_ch[a].size()) > 0);

			leaf_alive_time[a] = t;
			leaf_alive_val[a] = true;
			alive_leaves[a] += dead_leaves[a];
			alive_cnt[!parity[a]] += dead_leaves[a];
			dead_leaves[a] = 0;

			for (int c : dead_ch[a]) {
				assert(!is_alive(c));
				last_alive_time[c] = t;
				last_alive_val[c] = true;
				alive_cnt[!parity[a]]++;
				nxt_check.insert(c);
				alive_ch[a].insert(a);
			}
			dead_ch[a].clear();

			nxt_check.insert(a);
		}

		for (int a : to_mark) {
			assert(!is_leaf[a]);
			assert(alive_leaves[a] + int(alive_ch[a].size()) > 0);
			if (is_alive(a)) continue;
			last_alive_time[a] = t;
			last_alive_val[a] = true;
			alive_cnt[parity[a]]++;
			nxt_check.insert(a);
			int p = par[a];
			if (p != -1) {
				dead_ch[p].erase(a);
				alive_ch[p].insert(a);
				nxt_check.insert(p);
				nxt_check.insert(par[p]);
			}
		}

		to_mark.clear();
		to_ch_mark.clear();
		for (int a : nxt_check) {
			check_stuff(a);
		}
	}
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, Q; cin >> N >> Q;

	std::vector<std::vector<int>> adj(N);
	for (int e = 0; e < N-1; e++) {
		int u, v; cin >> u >> v; u--, v--;
		adj[u].push_back(v);
		adj[v].push_back(u);
	}

	std::array<tree_solver, 2> solvers{
		tree_solver(adj, 0),
		tree_solver(adj, 1),
	};
	assert(solvers[0].parity == solvers[1].parity);

	for (int q = 0; q < Q; q++) {
		int op; cin >> op;
		if (op == 0) {
			// unmark
			int v; cin >> v; v--;
			solvers[solvers[0].parity[v]].do_unmark(v);
		} else if (op == 1) {
			int v; cin >> v; v--;
			solvers[solvers[0].parity[v]].do_mark(v);
		} else if (op == 2) {
			solvers[0].do_flip();
			solvers[1].do_flip();
			std::swap(solvers[0], solvers[1]);
		} else assert(false);

		int ans = solvers[0].alive_cnt[0] + solvers[1].alive_cnt[1];
		cout << ans << " \n"[q+1==Q];
	}

	return 0;
}
