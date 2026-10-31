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
	vector<vector<int>> adj(N);
	for (int e = 0; e < N-1; e++) {
		int u, v; cin >> u >> v; u--, v--;
		adj[u].push_back(v);
		adj[v].push_back(u);
	}
	vector<int> C(N); for (auto& c : C) cin >> c;

	bool c0 = 0, c1 = 0;

	if (N == 1) c1 ^= 1;
	if (N == 2) c0 ^= 1;

	// Check if there are an odd number of ways to order this subtree's edges
	// such that parent is before child. Returns # edges if it's odd, and -1 otherwise.
	auto dfs = std::y_combinator([&](auto self, int cur, int prv) -> int {
		int res = 0;
		for (int nxt : adj[cur]) {
			if (nxt == prv) continue;
			int c = self(nxt, cur)+1;
			if (!c) return -1;
			if (res & c) return -1;
			res += c;
		}
		return res;
	});

	for (int i = 0; i < N; i++) {
		if (!C[i]) {
			int r = dfs(i, -1);
			if (r != -1) {
				c0 ^= 1, c1 ^= 1;
			}
		}
	}

	for (int i = 0; i < N; i++) {
		for (int j : adj[i]) {
			if (j < i) continue;
			if (!C[i] && !C[j]) {
				int x = dfs(i, j);
				int y = dfs(j, i);
				if (x != -1 && y != -1 && !(x & y) && !(1 & (x+y))) {
					c0 ^= 1, c1 ^= 1;
				}
			}
		}
	}

	cout << int(c1) << '\n';

	return 0;
}

// To solve this problem, we'll first do some black magic PIE over choices of
// C; in particular, we will relax some subset of 0's to ?'s and set the rest
// to 1's. Summing over all such choices gives the correct count. Here, '?'
// means a choice between 0 and 1, so the total number of scenarios will be
// (N-1)! * 2^{# ?}.
//
// Now, we will analyze the case where all nodes are initially ?/1, and there
// is at least one ?. First, note that NAND(?,1) = {0, 1} = ?, NAND(?,0) = {1,
// 1}. Thus, if we ever contract ? and 0, then we will have an even number of
// identical outcomes, so we can discard such cases. Also, contracting ? with 1
// essentially "swallows" the 1 into the ?. Finally, observe that NAND(?, ?) =
// {0, 1, 1, 1} = ? mod 2.
//
// Now, we will show that, modulo 2, we can consider only NAND(?, 1)
// contractions that use an original 1. Consider the earliest contraction 1 =
// NAND(., .) (deepest contraction in the tree of merges). By minimality, this
// must be of the form 1 = NAND(0 = NAND(1_a, 1_b), 0 = NAND(1_c, 1_d)) or
// 1 = NAND(0 = NAND(1_a, 1_b), 1_c). In the former case, we can swap the order
// of the two initial NAND(1,1) contractions; in the latter, we can swap to 1 =
// NAND(1_a, NAND(1_b, 1_c)) (exactly one of 1_a and 1_b is connected to 1_c).
// Thus, both of these cases can be paired up and have an even number of
// equivalent outcomes, so we only need to consider NAND(?, 1_original). (Note
// that this analysis is also shows that we can ignore the no-?/all-1s case if
// N >= 3.)
//
// We've now shown that we can consider only performing NAND(?, 1) = ? and
// NAND(?, ?) = ?. If there's a connected component of ?'s of size 3 or more,
// then the order of the contractions inside the component do not matter, so
// there will be an even number of such outcomes. If there are two connected
// components of ?'s, then we can pick either to merge with the connecting 1's
// first, so there are also an even number of outcomes. Thus, there is either
// 1 or 2 ?'s in the graph, and they must be adjacent. Then, we simply need
// to count the number of sequences which do NAND(?, 1) and NAND(?, ?) only;
// this is simple, we just need to count the number of topological sorts of the
// vertices which obey parent-before-child relative to the ? as the root. This
// is also almost always even, in particular it's even if there isn't a perfect
// matching of the 1's.
