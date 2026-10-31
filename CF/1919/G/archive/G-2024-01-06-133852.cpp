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

	std::mt19937_64 mt(std::chrono::steady_clock::now().time_since_epoch().count());

	int N; cin >> N;
	std::vector<std::string> S(N);
	for (auto& s : S) cin >> s;

	std::vector<int> par(N, -1);
	auto get_par = [&](int a) -> int {
		while (par[a] >= 0) {
			if (par[par[a]] >= 0) par[a] = par[par[a]];
			a = par[a];
		}
		return a;
	};
	auto merge = [&](int a, int b) -> bool {
		a = get_par(a), b = get_par(b);
		if (a == b) return false;
		if (par[a] >= par[b]) std::swap(a, b);
		par[a] += par[b];
		par[b] = a;
		return true;
	};
	std::vector<std::array<int, 2>> edges; edges.reserve(N-1);
	auto add_edge = [&](int a, int b) -> void {
		if (merge(a, b)) {
			edges.push_back({a, b});
		} else {
			cout << "NO" << '\n';
			exit(0);
		}
	};

	uint64_t tot_val = 0;
	std::vector<uint64_t> node_val(N);
	for (int i = 0; i < N; i++) {
		node_val[i] = mt();
		tot_val ^= node_val[i];
	}

	std::vector<int64_t> node_out_val(N);
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (S[i][j] == '1') node_out_val[i] ^= node_val[j];
		}
	}

	std::vector<std::pair<uint64_t, int>> nodes_by_out_val(N);
	for (int i = 0; i < N; i++) {
		nodes_by_out_val[i] = {node_out_val[i], i};
	}
	std::sort(nodes_by_out_val.begin(), nodes_by_out_val.end());
	std::map<uint64_t, int> all_specials;
	for (int z = 0; z < N; ) {
		std::vector<int> buf_0s;
		std::vector<int> buf_1s;
		std::vector<int> buf_specials;
		{
			uint64_t hsh = nodes_by_out_val[z].first;
			while (z < N && nodes_by_out_val[z].first == hsh) {
				int v = nodes_by_out_val[z].second;
				{
					int cnt_0 = 0;
					int cnt_1 = 0;
					uint64_t r_hsh = 0;
					for (int r = 0; r < N; r++) {
						if (S[r][v] == '0') cnt_0++;
						else if (S[r][v] == '1') {
							r_hsh ^= node_val[r];
							cnt_1++;
						}
						else assert(false);
					}
					assert(cnt_0 + cnt_1 == N);
					if (cnt_0 == N) {
						// This guy is a 0
						buf_0s.push_back(v);
					} else if (cnt_1 == N) {
						// This guy is a 1
						buf_1s.push_back(v);
					} else {
						// This guy is special
						buf_specials.push_back(v);
						{
							auto it = all_specials.find(tot_val ^ r_hsh);
							if (it != all_specials.end()) {
								add_edge(it->second, v);
								all_specials.erase(it);
							} else {
								all_specials.insert({r_hsh, v});
							}
						}
					}
				}
				z++;
			}
		}

		// We need to connect everything into a tree
		// Hardest constraint to satisfy is that 1's connect to at least 2 0's each
		// Specials can connect to any 1's or specials
		if (!buf_1s.empty()) {
			if (buf_0s.size() < buf_1s.size() + 1) {
				cout << "NO" << '\n';
				exit(0);
			}
			for (int i = 0; i < int(buf_1s.size()); i++) {
				add_edge(buf_1s[i], buf_0s[i]);
				add_edge(buf_1s[i], buf_0s[i+1]);
			}
			for (int i = int(buf_1s.size()) + 1; i < int(buf_0s.size()); i++) {
				add_edge(buf_1s[0], buf_0s[i]);
			}
			for (int v : buf_specials) {
				add_edge(buf_1s[0], v);
			}
		} else if (!buf_specials.empty()) {
			if (buf_0s.size() > 0) {
				cout << "NO" << '\n';
				exit(0);
			}
			for (int i = 0; i+1 < int(buf_specials.size()); i++) {
				add_edge(buf_specials[i], buf_specials[i+1]);
			}
		} else {
			// only 0's
			if (buf_0s.size() >= 2) {
				cout << "NO" << '\n';
				exit(0);
			}
		}
	}

	if (int(edges.size()) != N-1) {
		cout << "NO" << '\n';
		exit(0);
	}

	std::vector<std::vector<int>> adj(N);
	for (auto [u, v] : edges) {
		adj[u].push_back(v);
		adj[v].push_back(u);
	}
	for (int r = 0; r < N; r++) {
		std::y_combinator([&](auto self, int cur, int prv) -> bool {
			bool winning = false;
			for (int nxt : adj[cur]) {
				if (nxt == prv) continue;
				if (!self(nxt, cur)) winning = true;
			}
			if (S[r][cur] != ('0' + winning)) {
				cout << "NO" << '\n';
				exit(0);
			}
			return winning;
		})(r, -1);
	}

	cout << "YES" << '\n';
	for (auto [u, v] : edges) {
		cout << u+1 << ' ' << v+1 << '\n';
	}

	return 0;
}

// Consider a particular vertex
// If s[i, i] == 0, then necessarily s[r, i] == 0 for all r, and s[i->j] == 1 for all neighbors j
// If s[i, i] == 1, and all s[r, i] == 1, then there's at least two neighbors s[i->j] == 0
//
// If s[i, i] == 1 and some s[r, i] == 0, then those roots are all part of the same subtree with s[i->j] == 0
// We can just cut that apart at i and solve the two subproblems.
// Let the halves be L, R, with L cap R == {i}
// Then, {s[L, r]} must match and {s[R, l]} must match except s[R, i] == 0 but s[i, i] == 1
// Then, we can just solve the two problems independently if we attach an extra leaf to i
// We should peel i off from R though.
// We just need to pick any j in R so that s[i, R / i / j] == s[j, R / i / j] and s[i, j] == 0 and s[j, j] == 1 and s[*, j] == 1
