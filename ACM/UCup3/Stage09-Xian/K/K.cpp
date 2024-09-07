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

	int N, W_MAX; cin >> N >> W_MAX;
	std::vector<int> P(N);
	for (auto& p : P) { cin >> p; p--; }
	std::reverse(P.begin(), P.end());
	std::vector<int> W(N);
	for (auto& w : W) { cin >> w; }

	// indices into P[i]
	std::vector<std::pair<int, int>> bigs; bigs.reserve(N);
	for (int i = 0; i < N; i++) {
		if (W[P[i]] * 2 > W_MAX) {
			bigs.push_back({W[i], i});
		}
	}
	auto ct = CartesianTree::build_max_tree(bigs);
	std::vector<std::vector<int>> inds(ct.size());

	{
		std::vector<bool> used(N);
		std::vector<std::pair<int, int>> stk; stk.reserve(N);
		std::y_combinator([&](auto self, int cur) -> void {
			if (!(cur & 1)) {
				// we're a leaf, so let's process all the leaves here
				assert(ct[cur].c[0] == -1);
				assert(ct[cur].c[1] == -1);
				assert(ct[cur].r - ct[cur].l == 1);
				int l = ct[cur].l-1 == -1 ? -1 : bigs[ct[cur].l-1].second;
				int r = ct[cur].r+1 == int(bigs.size()) ? N : bigs[ct[cur].r+1].second;
				for (int i = l+1; i <= r-1; i++) {
					assert(W[P[i]] * 2 <= W_MAX);
					// binary search
					int lim = W_MAX - W[P[i]];
					int mi = -1;
					int ma = int(stk.size());
					while (ma - mi > 1) {
						int md = (mi + ma) / 2;
						if (stk[md].first <= lim) {
							// can move past
							ma = md;
						} else {
							mi = md;
						}
					}
					// look at ma
					assert(!used[i]);
					used[i] = true;
					inds[ma == int(stk.size()) ? cur : stk[ma].second].push_back(P[i]);
				}
				return;
			} else {
				assert(!used[bigs[ct[cur].m].second]);
				used[bigs[ct[cur].m].second] = true;
				stk.push_back({bigs[ct[cur].m].first, cur});
				self(ct[cur].c[0]);
				self(ct[cur].c[1]);
				stk.pop_back();
			}
		})(ct.root);
		for (int i = 0; i < N; i++) assert(used[i]);
	}

	return 0;
}
