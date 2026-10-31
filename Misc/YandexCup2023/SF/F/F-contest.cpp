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
	
	std::vector<int64_t> ans(N);

	// NOTE: we could do this as we go, but that's too much brain for 7am Andrew
	auto solve_tree = [&](const std::vector<std::pair<int, std::array<int, 2>>>& edges, int w) {
		assert(std::is_sorted(edges.begin(), edges.end(), [&](auto a, auto b) { return a.first > b.first; }));

		// linearize first
		std::vector<std::pair<int, int>> nxt(N, {-1, -1});
		std::vector<int> par(N, -1);
		auto get_par = [&](int a) -> int {
			while (par[a] >= 0) {
				if (par[par[a]] >= 0) par[a] = par[par[a]];
				a = par[a];
			}
			return a;
		};
		std::vector<std::pair<int, int>> lst(N);
		for (int i = 0; i < N; i++) lst[i] = {i, i};
		auto merge = [&](int a, int b, int w) -> void {
			a = get_par(a), b = get_par(b);
			assert(a != b);
			nxt[lst[a].second] = {lst[b].first, w};
			std::pair<int, int> nlst = {lst[a].first, lst[b].second};
			if (par[a] > par[b]) std::swap(a, b);
			lst[a] = nlst;
			lst[b] = {-1, -1};
			par[a] += par[b];
			par[b] = a;
		};
		for (auto& [w, ends] : edges) {
			merge(w, ends[0], ends[1]);
		}
		std::vector<int> weights; weights.reserve(N-1);
		std::vector<int> verts; verts.reserve(N);
		for (int cur = lst[get_par(0)].first; ; ) {
			verts.push_back(cur);
			if (nxt[cur].first == -1) break;
			weights.push_back(nxt[cur].second);
			cur = nxt[cur].first;
		}
		assert(int(weights.size()) == N-1);
		assert(int(verts.size()) == N);
		// TODO
	};

	std::vector<std::pair<int, std::pair<int, std::array<int, 2>>>> all_edges; all_edges.reserve(2*(N-1));
	for (int z = 0; z < 2; z++) {
		std::vector<std::pair<int, std::array<int, 2>>> edges(N-1);
		for (auto& [w, ends] : edges) {
			cin >> w >> ends[0] >> ends[1];
			ends[0]--, ends[1]--;
		}
		std::sort(edges.begin(), edges.end(), [&](auto a, auto b) { return a.first > b.first; });
		solve_tree(edges, +1);

		for (auto [w, ends] : edges) {
			all_edges.push_back({w, {z, ends}});
		}
	}

	std::sort(all_edges.begin(), all_edges.end(), [&](auto a, auto b) { return a.first > b.first; });
	std::vector<std::pair<int, std::array<int, 2>>> rect_edges; rect_edges.reserve(N-1);

	// TODO: Generate them

	std::vector<int> par(N, -1);
	for (auto& [w, z_ends] : all_edges) {
		auto [z, ends] = z_ends;
	}

	std::sort(rect_edges.begin(), rect_edges.end(), [&](auto a, auto b) { return a.first > b.first; });
	solve_tree(rect_edges, -1);

	for (int i = 0; i < N; i++) {
		cout << ans[i] << " \n"[i+1==N];
	}

	return 0;
}
