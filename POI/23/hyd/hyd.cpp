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

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M; cin >> N >> M;
	vector<vector<pair<int, int>>> adj(N);
	for (int e = 0; e < M; e++) {
		int u, v; cin >> u >> v; u--, v--;
		adj[u].push_back({v, e});
		adj[v].push_back({u, e});
	}

	int C = 0; // number of cycles
	vector<vector<int>> cycle_verts; // item 0 is the root
	vector<vector<int>> vert_subcycles(N);

	const int ROOT = 0;
	{
		vector<bool> vis(N);
		vector<bool> in_stack(N);
		vector<int> stk; stk.reserve(N);
		std::y_combinator([&](auto self, int cur, int prvE = -1) -> void {
			assert(!vis[cur]);
			vis[cur] = true;
			in_stack[cur] = true;
			stk.push_back(cur);
			for (auto [nxt, e] : adj[cur]) {
				if (e == prvE) continue;
				if (vis[nxt]) {
					if (in_stack[nxt]) {
						//cerr << "found cycle " << cur << "->" << nxt << '\n';
						int idx = C++;
						// nxt is an ancestor
						auto it = stk.end();
						--it;
						while (*it != nxt) --it;

						cycle_verts.push_back(vector<int>(it, stk.end()));
						vert_subcycles[nxt].push_back(idx);
					}
				} else {
					self(nxt, e);
				}
			}
			assert(!stk.empty() && stk.back() == cur);
			in_stack[cur] = false;
			stk.pop_back();
		})(ROOT);
	}

	vector<int> node_type_down(N); // 2 types: first player winning (1) or reversing (0)
	vector<int> cyc_type_down(C); // 4 types: odd (1), even(0), first-player-winning (-1), second-player-winning (-2)
	std::y_combinator([&](auto self, int cur) -> void {
		bool is_winning = false;
		int tot_parity = 0;

		for (int idx : vert_subcycles[cur]) {
			assert(cur == cycle_verts[idx][0]);
			int L = int(cycle_verts[idx].size());

			int first_trigger = -1;
			int last_trigger = -1;
			for (int i = 1; i < L; i++) {
				int nxt = cycle_verts[idx][i];
				self(nxt);
				if (node_type_down[nxt]) {
					if (first_trigger == -1) first_trigger = i;
					last_trigger = i;
				}
			}
			if (first_trigger == -1) {
				cyc_type_down[idx] = int(cycle_verts[idx].size()) % 2;
			} else {
				if (first_trigger % 2 == 0 || (L - last_trigger) % 2 == 0) {
					cyc_type_down[idx] = -1;
				} else {
					cyc_type_down[idx] = -2;
				}
			}

			if (cyc_type_down[idx] == -2) {
				// do nothing
			} else if (cyc_type_down[idx] == -1) {
				is_winning = true;
			} else {
				tot_parity ^= cyc_type_down[idx];
			}
		}

		node_type_down[cur] = is_winning || (tot_parity == 1);
	})(ROOT);

	vector<int> cyc_type_up(N);
	vector<int> node_type_up(C);
	vector<int> node_type(N);

	cyc_type_up[ROOT] = 0;

	std::y_combinator([&](auto self, int cur) -> void {
		int num_winning = 0;
		int tot_parity = 0;
		for (int idx : vert_subcycles[cur]) {
			if (cyc_type_down[idx] == -1) {
				num_winning++;
			} else if (cyc_type_down[idx] == 1) {
				tot_parity ^= 1;
			}
		}

		if (cyc_type_up[cur] == -1) {
			num_winning++;
		} else if (cyc_type_up[cur] == 1) {
			tot_parity ^= 1;
		}

		node_type[cur] = num_winning || tot_parity;

		for (int idx : vert_subcycles[cur]) {
			if (cyc_type_down[idx] == -1) {
				num_winning--;
			} else if (cyc_type_down[idx] == 1) {
				tot_parity ^= 1;
			}

			node_type_up[idx] = bool(num_winning) || tot_parity;

			if (cyc_type_down[idx] == -1) {
				num_winning++;
			} else if (cyc_type_down[idx] == 1) {
				tot_parity ^= 1;
			}
		}

		for (int idx : vert_subcycles[cur]) {
			assert(cycle_verts[idx][0] == cur);
			int L = int(cycle_verts[idx].size());
			vector<bool> vert_wins(L);
			vert_wins[0] = node_type_up[idx];
			for (int i = 1; i < L; i++) {
				int nxt = cycle_verts[idx][i];
				vert_wins[i] = node_type_down[nxt];
			}
			int num_wins = 0;
			for (int i = 0; i < L; i++) num_wins += vert_wins[i];
			int prv_win = -1, nxt_win = -1;
			if (num_wins) {
				prv_win = 0;
				if (!vert_wins[0]) {
					prv_win = L-1;
					while (!vert_wins[prv_win]) prv_win--;
					prv_win -= L;
				}
				nxt_win = 1; // this will get bumped
			}
			for (int i = 1; i < L; i++) {
				int nxt = cycle_verts[idx][i];
				if (nxt_win == i) {
					nxt_win++;
					while (!vert_wins[nxt_win%L]) nxt_win++;
				}
				if (num_wins - vert_wins[i]) {
					assert(i - N < prv_win && prv_win <= i && i <= nxt_win && nxt_win < i + N);

					if ((nxt_win - i) % 2 == 0 || (i - prv_win) % 2 == 0) {
						cyc_type_up[nxt] = -1;
					} else {
						cyc_type_up[nxt] = -2;
					}
				} else {
					cyc_type_up[nxt] = L % 2;
				}
				self(nxt);

				if (vert_wins[i]) prv_win = i;
			}
		}
	})(ROOT);

	for (int i = 0; i < N; i++) {
		cout << (node_type[i] ? 1 : 2) << '\n';
	}

	return 0;
}

// At a given node, for each subtree (cycle), we'll compute whether it's winning or not
// Each subtree is either first-player-winning or second-player-winning or even or odd
// 2nd-player-winning means you should ignore it
// even also means you should ignore it
//
// Whether nodes are winning
// If there's a first-player-winning subtree, then this node is first-player-winning.
// If total parity of subtrees is even, this node is reversing.
// Otherwise, this node is first-player-winning.
//
// If a cycle has any first-player-winning node, then it's either first-player-winning or 2nd-player winning depending on parity to reach that node.
// Otherwise, it's just the parity of the cycle length.

// 1 - 2 - 3 - 1; 2 - 4 - 5 - 2; 3 - 6 - 7 - 3
