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

template <typename T> void setmax(T& a, T b) { if (b > a) a = b; }

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> P(N);
		for (auto& p : P) { cin >> p; p--; }
		std::string S; cin >> S;
		auto solve = [&]() -> int {
			CartesianTree ct = CartesianTree::build_max_tree(P);
			// first, enforce connected by ensuring there are no extra roots
			for (int i = 0; i < N; i++) {
				if (ct.root == 2*i+1) {
					assert(ct[2*i+1].l == 0 && ct[2*i+1].r == N-1);
				} else if (ct[2*i+1].l == 0) {
					// not allowed to point left
					if (S[i] == 'L') {
						return -1;
					}
					S[i] = 'R';
				} else if (ct[2*i+1].r == N-1) {
					if (S[i] == 'R') {
						return -1;
					}
					S[i] = 'L';
				}
			}
			// we care about edge count
			int ans = 0;
			std::y_combinator([&](auto self, int cur) -> std::array<int, 3> {
				if (!(cur & 1)) {
					return {0, 0, 0};
				}
				char own_c = S[cur / 2];
				assert(own_c == 'L' || own_c == 'R' || own_c == '?');
				auto dp0 = self(ct[cur].c[0]);
				auto dp1 = self(ct[cur].c[1]);

				std::array<int, 3> res{0, 0, 0};

				// if we use one from each subtree, we have:
				setmax(ans, dp0[1] + dp1[0]);
				setmax(res[2], dp0[0] + dp1[1]);
				setmax(res[1], dp1[1]);
				setmax(res[0], dp0[0]);

				// iterate over our own direction
				for (int z = 0; z < 2; z++) {
					if (!(own_c == '?' || own_c == "LR"[z])) continue;

					// someone pointing towards us, and us outwards
					setmax(res[z], 1 + std::max(dp0[1], dp1[0]));

					// point towards the same guy
					// both left
					if (z == 0) {
						if (ct[cur].l > 0) { setmax(ans, dp0[0] + 1 + dp1[0]); }
					} else {
					}
					// both right
					if (ct[cur].r < N-1) { setmax(ans, dp0[1] + dp1[1]); }
				}

				return res;
			})(ct.root);
			return ans;
		};

		int ans = solve();
		cout << ans << '\n';
	}

	return 0;
}
