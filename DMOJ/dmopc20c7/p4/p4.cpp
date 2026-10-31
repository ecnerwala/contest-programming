#include <bits/stdc++.h>

/** Binary-indexed tree
 *
 *  A binary indexed tree with N nodes of type T provides the
 *  following two functions for 0 <= i <= N:
 *
 *      prefix(int i) -> prefix_iterator<T>
 *      suffix(int i) -> suffix_iterator<T>
 *
 *  such that size(suffix(i) intersect prefix(j)) = (1 if i < j else 0).
 *  Furthermore, the resulting lists always have size at most log_2(N).
 *
 *  This can be used to implement either point-update/(prefix|suffix)-query or
 *  (prefix|suffix)-update/point-query over a virtual array of size N of a
 *  commutative monoid. This can be generalized to implement
 *  point-update/range-query or range-update/point-query over a virtual array
 *  of size N of a commutative group.
 *
 *  With 0-indexed data, prefixes are more natural:
 *   * For range update/query, use for_prefix for the ranges and for_suffix for the points.
 *   * For prefix update/query, no change.
 *   * For suffix update/query, use for_prefix(point + 1); 1-index the data.
 */
template <typename T> class binary_indexed_tree {
private:
	std::vector<T> dat;
public:
	binary_indexed_tree() {}
	explicit binary_indexed_tree(size_t N) : dat(N) {}
	binary_indexed_tree(size_t N, const T& t) : dat(N, t) {}

	size_t size() const { return dat.size(); }
	const std::vector<T>& data() const { return dat; }
	std::vector<T>& data() { return dat; }

private:
	template <typename I, typename S = I> struct iterator_range {
	private:
		I begin_;
		S end_;
	public:
		iterator_range() : begin_(), end_() {}
		iterator_range(const I& begin__, const S& end__) : begin_(begin__), end_(end__) {}
		iterator_range(I&& begin__, S&& end__) : begin_(begin__), end_(end__) {}
		I begin() const { return begin_; }
		S end() const { return end_; }
	};

public:
	class const_suffix_iterator {
	private:
		const T* dat;
		int a;
		const_suffix_iterator(const T* dat_, int a_) : dat(dat_), a(a_) {}
		friend class binary_indexed_tree;
	public:
		friend bool operator != (const const_suffix_iterator& i, const const_suffix_iterator& j) {
			assert(j.dat == nullptr);
			return i.a < j.a;
		}
		const_suffix_iterator& operator ++ () {
			a |= a+1;
			return *this;
		}
		const T& operator * () const {
			return dat[a];
		}
	};
	using const_suffix_range = iterator_range<const_suffix_iterator>;
	const_suffix_range suffix(int a) const {
		assert(0 <= a && a <= int(dat.size()));
		return const_suffix_range{const_suffix_iterator{dat.data(), a}, const_suffix_iterator{nullptr, int(dat.size())}};
	}

	class suffix_iterator {
	private:
		T* dat;
		int a;
		suffix_iterator(T* dat_, int a_) : dat(dat_), a(a_) {}
		friend class binary_indexed_tree;
	public:
		friend bool operator != (const suffix_iterator& i, const suffix_iterator& j) {
			assert(j.dat == nullptr);
			return i.a < j.a;
		}
		suffix_iterator& operator ++ () {
			a |= a+1;
			return *this;
		}
		T& operator * () const {
			return dat[a];
		}
	};
	using suffix_range = iterator_range<suffix_iterator>;
	suffix_range suffix(int a) {
		assert(0 <= a && a <= int(dat.size()));
		return suffix_range{suffix_iterator{dat.data(), a}, suffix_iterator{nullptr, int(dat.size())}};
	}

	class const_prefix_iterator {
	private:
		const T* dat;
		int a;
		const_prefix_iterator(const T* dat_, int a_) : dat(dat_), a(a_) {}
		friend class binary_indexed_tree;
	public:
		friend bool operator != (const const_prefix_iterator& i, const const_prefix_iterator& j) {
			assert(j.dat == nullptr);
			return i.a > 0;
		}
		const_prefix_iterator& operator ++ () {
			a &= a-1;
			return *this;
		}
		const T& operator * () const {
			return dat[a-1];
		}
	};
	using const_prefix_range = iterator_range<const_prefix_iterator>;
	const_prefix_range prefix(int a) const {
		return const_prefix_range{const_prefix_iterator{dat.data(), a}, const_prefix_iterator{nullptr, 0}};
	}

	class prefix_iterator {
	private:
		T* dat;
		int a;
		prefix_iterator(T* dat_, int a_) : dat(dat_), a(a_) {}
		friend class binary_indexed_tree;
	public:
		friend bool operator != (const prefix_iterator& i, const prefix_iterator& j) {
			assert(j.dat == nullptr);
			return i.a > 0;
		}
		prefix_iterator& operator ++ () {
			a &= a-1;
			return *this;
		}
		T& operator * () const {
			return dat[a-1];
		}
	};
	using prefix_range = iterator_range<prefix_iterator>;
	prefix_range prefix(int a) {
		return prefix_range{prefix_iterator{dat.data(), a}, prefix_iterator{nullptr, 0}};
	}
};

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

	int N, Q; cin >> N >> Q;
	vector<vector<int>> adj(N);
	for (int e = 0; e < N-1; e++) {
		int u, v; cin >> u >> v; u--, v--;
		adj[u].push_back(v);
		adj[v].push_back(u);
	}

	std::y_combinator([&](auto self, int cur) -> int {
		int res = 1;
		int best_child = -1;
		int best_sz = 0;
		for (int z = 0; z < int(adj[cur].size()); z++) {
			int nxt = adj[cur][z];
			adj[nxt].erase(std::find(adj[nxt].begin(), adj[nxt].end(), cur));
			int ch = self(nxt);
			if (ch > best_sz) {
				best_child = z;
				best_sz = ch;
			}
			res += ch;
		}
		if (!adj[cur].empty()) {
			swap(adj[cur][0], adj[cur][best_child]);
		}
		return res;
	})(0);

	vector<int> preorder_idx(N);
	vector<int> depth(N);
	vector<int> heavy_root(N);
	vector<int> par(N);
	{
		par[0] = -1;
		depth[0] = 0;
		heavy_root[0] = 0;
		int nxt_idx = 1;
		std::y_combinator([&](auto self, int cur, int cur_idx) -> void {
			preorder_idx[cur] = cur_idx;
			for (int z = 0; z < int(adj[cur].size()); z++) {
				int nxt = adj[cur][z];
				par[nxt_idx] = cur_idx;
				depth[nxt_idx] = depth[cur_idx]+1;
				heavy_root[nxt_idx] = z ? nxt_idx : heavy_root[cur_idx];
				self(nxt, nxt_idx++);
			}
		})(0, 0);
		assert(nxt_idx == N);
	}

	vector<int> V(N);
	binary_indexed_tree<int> bit(N);

	while (Q--) {
		int op; cin >> op;
		if (op == 1) {
			int u; cin >> u; u--; u = preorder_idx[u];
			int delta = V[u] ? -1 : +1;
			V[u] += delta;
			assert(V[u] == 0 || V[u] == 1);
			if (par[u] != -1) {
				for (auto& a : bit.suffix(par[u])) a += delta;
			}
		} else if (op == 2) {
			int u, v; cin >> u >> v; u--, v--; u = preorder_idx[u], v = preorder_idx[v];
			int ans = 0;
			while (true) {
				if (u > v) swap(u, v);
				if (u >= heavy_root[v]) {
					// We're at the end of the chain
					for (auto& a : bit.prefix(v+1)) ans += a;
					for (auto& a : bit.prefix(u)) ans -= a;
					ans += V[u];
					if (par[u] != -1) ans += V[par[u]];
					break;
				} else {
					for (auto& a : bit.prefix(v+1)) ans += a;
					for (auto& a : bit.prefix(heavy_root[v])) ans -= a;
					v = par[heavy_root[v]];
				}
			}
			cout << ans << '\n';
		}
	}

	return 0;
}
