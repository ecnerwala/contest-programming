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

template <typename F> struct reverse_comparator_t {
	F f;
	template <typename Arg1, typename Arg2> constexpr bool operator() (Arg1&& arg1, Arg2&& arg2) & {
		return f(std::forward<Arg2>(arg2), std::forward<Arg1>(arg1));
	}
	template <typename Arg1, typename Arg2> constexpr bool operator() (Arg1&& arg1, Arg2&& arg2) const& {
		return f(std::forward<Arg2>(arg2), std::forward<Arg1>(arg1));
	}
	template <typename Arg1, typename Arg2> constexpr bool operator() (Arg1&& arg1, Arg2&& arg2) && {
		return std::move(f)(std::forward<Arg2>(arg2), std::forward<Arg1>(arg1));
	}
	template <typename Arg1, typename Arg2> constexpr bool operator() (Arg1&& arg1, Arg2&& arg2) const&& {
		return std::move(f)(std::forward<Arg2>(arg2), std::forward<Arg1>(arg1));
	}
};

template <typename F> constexpr reverse_comparator_t<std::decay_t<F>> reverse_comparator(F&& f) {
	return { std::forward<F>(f) };
}

class CartesianTree {
public:
	struct Node {
		int l, m, r; // inclusive ranges
		std::array<int, 2> c;
	};
	std::vector<Node> nodes;
	int root = -1;

	CartesianTree() {}

	Node& operator [] (int idx) { return nodes[idx]; }
	const Node& operator [] (int idx) const { return nodes[idx]; }

	int size() const { return int(nodes.size()); }

private:
	CartesianTree(std::vector<Node>&& nodes_, int root_) : nodes(std::move(nodes_)), root(root_) {}

public:

	// min-cartesian-tree, with earlier cells tiebroken earlier
	template <typename T, typename Comp = std::less<T>>
	static CartesianTree build_min_tree(const std::vector<T>& v, Comp comp = Comp()) {
		std::vector<Node> nodes(v.size()*2+1);
		std::vector<int> stk; stk.reserve(v.size());
		int root = -1;
		for (int i = 0; i <= int(v.size()); i++) {
			int cur = 2*i;
			nodes[cur].l = i;
			nodes[cur].r = i-1;
			nodes[cur].m = i-1;
			nodes[cur].c = {-1, -1};
			while (!stk.empty() && (i == int(v.size()) || comp(v[i], v[nodes[stk.back()].m]))) {
				int nxt = stk.back(); stk.pop_back();
				nodes[nxt].c[1] = cur;
				nodes[nxt].r = nodes[cur].r;
				cur = nxt;
			}
			if (i == int(v.size())) {
				root = cur;
				break;
			}
			nodes[2*i+1].l = nodes[cur].l;
			nodes[2*i+1].m = i;
			nodes[2*i+1].c[0] = cur;
			stk.push_back(2*i+1);
		}
		return {std::move(nodes), root};
	}

	// max-cartesian-tree, with earlier cells tiebroken earlier
	template <typename T, typename Comp = std::less<T>>
	static CartesianTree build_max_tree(const std::vector<T>& v, Comp comp = Comp()) {
		return build_min_tree(v, reverse_comparator(comp));
	}
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<int> P(N);
	for (auto& p : P) { cin >> p; p--; }

	CartesianTree tree = CartesianTree::build_max_tree(P);

	std::vector<std::vector<int>> long_ch(N);
	std::vector<std::vector<int>> short_ch(N);
	std::vector<int> long_par(N);
	std::vector<int> short_par(N);
	int root = -1;
	for (int i = 0; i < N; i++) {
		int a = tree[2*i+1].l - 1;
		int b = tree[2*i+1].r + 1;
		if (2*i+1 == tree.root) {
			assert(a == -1 && b == N);
			long_par[i] = -1;
			short_par[i] = -1;
		} else if (a == -1) {
			assert(b != N);
			long_par[i] = b;
			short_par[i] = b;
		} else if (b == N) {
			assert(a != -1);
			long_par[i] = a;
			short_par[i] = a;
		} else {
			long_par[i] = (P[a] > P[b]) ? a : b;
			short_par[i] = (P[a] < P[b]) ? a : b;
		}
		if (long_par[i] != -1) {
			long_ch[long_par[i]].push_back(i);
			short_ch[short_par[i]].push_back(i);
		} else {
			root = i;
		}
		//cerr << "p " << i << ' ' << long_par[i] << ' ' << short_par[i] << '\n';
	}
	assert(root != -1);

	std::vector<int> long_sz(N);
	std::y_combinator([&](auto self, int cur) -> void {
		long_sz[cur] = 1;
		for (int nxt : long_ch[cur]) {
			self(nxt);
			long_sz[cur] += long_sz[nxt];
		}
	})(root);
	std::vector<int64_t> tot_cost(N);
	tot_cost[root] = std::accumulate(long_sz.begin(), long_sz.end(), int64_t(0));
	std::y_combinator([&](auto self, int cur) -> void {
		for (int nxt : short_ch[cur]) {
			tot_cost[nxt] = tot_cost[cur] - long_sz[nxt];
			self(nxt);
		}
	})(root);

	//for (int i = 0; i < N; i++) { cerr << long_sz[i] << ' '; } cerr << '\n';
	//for (int i = 0; i < N; i++) { cerr << tot_cost[i] << ' '; } cerr << '\n';

	std::vector<int64_t> answers(N);
	for (int i = 0; i < N; i++) {
		int64_t ans = tot_cost[i];
		ans -= long_sz[i];
		for (int c : tree[2*i+1].c) {
			if (c & 1) {
				c /= 2;
				if (long_par[c] != i) {
					ans -= long_sz[c];
				}
			}
		}
		answers[i] = ans;
	}
	for (int i = 0; i < N; i++) {
		cout << answers[i] << " \n"[i+1==N];
	}

	return 0;
}
