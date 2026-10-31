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

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	const int B = 60;

	int N, M; cin >> N >> M;
	vector<array<int, 2>> edges(M);
	for (auto& e : edges) { cin >> e[0] >> e[1]; e[0]--, e[1]--; }

	vector<int> par(N, -1);
	auto get_par = [&](int a) {
		while (par[a] >= 0) {
			if (par[par[a]] >= 0) par[a] = par[par[a]];
			a = par[a];
		}
		return a;
	};
	auto merge = [&](int a, int b) {
		a = get_par(a), b = get_par(b);
		if (a == b) return false;
		if (par[a] > par[b]) swap(a, b);
		par[a] += par[b];
		par[b] = a;
		return true;
	};

	vector<vector<pair<int, int>>> adj(N);

	int num_bad = 0;
	vector<uint64_t> bad_node_msk(N, 0);
	vector<uint64_t> bad_edge_msk(M, 0);
	for (int e = 0; e < M; e++) {
		if (merge(edges[e][0], edges[e][1])) {
			adj[edges[e][0]].push_back({e, edges[e][1]});
			adj[edges[e][1]].push_back({e, edges[e][0]});
		} else {
			int bad_idx = num_bad++;
			if (bad_idx < B) {
				bad_node_msk[edges[e][0]] ^= uint64_t(1) << bad_idx;
				bad_node_msk[edges[e][1]] ^= uint64_t(1) << bad_idx;
				bad_edge_msk[e] ^= uint64_t(1) << bad_idx;
			}
		}
	}

	assert(num_bad == M - (N-1));

	vector<uint8_t> P(N); for (auto& p : P) { int v; cin >> v; assert(v == 0 || v == 1); p = bool(v); }
	vector<int> node_idx(N, -1);
	vector<array<int, 2>> edge_idx(M, {-1,-1});
	vector<uint8_t> edge_val(M);

	{
		int cur_idx = 0;
		auto root = std::y_combinator([&](auto self, int cur, int prvE) -> pair<bool, uint64_t> {
			node_idx[cur] = cur_idx++;
			bool tot = P[cur];
			uint64_t node_msk = bad_node_msk[cur];

			for (auto [e, nxt] : adj[cur]) {
				if (e == prvE) continue;
				edge_idx[e][0] = cur_idx;
				auto ch = self(nxt, e);
				edge_idx[e][1] = cur_idx;

				edge_val[e] = ch.first;
				bad_edge_msk[e] = ch.second;

				tot ^= ch.first;
				node_msk ^= ch.second;
			}

			return {tot, node_msk};
		})(0, -1);
		assert(cur_idx == N);
		assert(root.first == 0);
		assert(root.second == 0);
	}

	uint64_t K; cin >> K; K--;
	assert(K < (uint64_t(1) << min(B, num_bad)));

	for (int e = 0; e < M; e++) {
		bool res = edge_val[e] ^ (__builtin_popcountll(bad_edge_msk[e] & K) & 1);
		cout << res << " \n"[e+1==M];
	}

	binary_indexed_tree<uint8_t> bit(N);

	bool tot_flip = 0;

	int Q; cin >> Q;
	while (Q--) {
		char op; cin >> op;
		if (op == 'M') {
			int v; cin >> v; v--;
			for (auto& a : bit.suffix(node_idx[v])) a ^= 1;
			tot_flip ^= 1;
		} else if (op == 'K') {
			cin >> K; K--;
		} else if (op == 'D') {
			int e; cin >> e; e--;
			if (tot_flip || (K >= (uint64_t(1) << min(B, num_bad)))) {
				cout << -1 << '\n';
			} else {
				bool res = edge_val[e] ^ (__builtin_popcountll(bad_edge_msk[e] & K) & 1);
				if (edge_idx[e][0] != edge_idx[e][1]) {
					for (int b : edge_idx[e]) {
						for (auto a : bit.prefix(b)) res ^= a;
					}
				}
				cout << res << '\n';
			}
		} else assert(false);
	}

	return 0;
}
