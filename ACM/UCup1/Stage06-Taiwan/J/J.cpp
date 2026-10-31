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

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, K, M, Q; cin >> N >> K >> M >> Q;

	struct line_t {
		std::vector<int> w;
		std::vector<int> on;
		binary_indexed_tree<int64_t> bit;
		int64_t tot;
	};
	std::array<line_t, 3> lines;
	lines[0].w.resize(K-1);
	for (auto& w : lines[0].w) cin >> w;

	lines[1].w.resize(N+1-K);
	for (auto& w : lines[1].w) cin >> w;
	std::reverse(lines[1].w.begin(), lines[1].w.end());

	lines[2].w.resize(M+1);
	for (auto& w : lines[2].w) cin >> w;

	for (int z = 0; z < 3; z++) {
		lines[z].on.assign(lines[z].w.size(), true);
		lines[z].tot = std::accumulate(lines[z].w.begin(), lines[z].w.end(), int64_t(0));

		lines[z].bit = binary_indexed_tree<int64_t>(lines[z].w.size());
		for (int i = 0; i < int(lines[z].w.size()); i++) {
			for (auto& a : lines[z].bit.suffix(i)) a += lines[z].w[i];
		}
	}

	const int64_t INF = 1e10;

	auto toggle_edge = [&](int z, int i) -> void {
		assert(0 <= i && i < int(lines[z].w.size()));
		int64_t v_old = lines[z].on[i] ? lines[z].w[i] : INF;
		lines[z].on[i] = !lines[z].on[i];
		int64_t v_new = lines[z].on[i] ? lines[z].w[i] : INF;

		int64_t diff = v_new - v_old;
		lines[z].tot += diff;
		for (auto& a : lines[z].bit.suffix(i)) a += diff;
	};

	for (int q = 0; q < Q; q++) {
		char op; cin >> op;
		if (op == 'q') {
			int u, v; cin >> u >> v;

			int uz, ui;
			if (1 <= u && u <= K) {
				uz = 0, ui = u - 1;
			} else if (K < u && u <= N) {
				uz = 1, ui = N + 1 - u;
			} else {
				uz = 2, ui = u - N;
			}
			int vz, vi;
			if (1 <= v && v <= K) {
				vz = 0, vi = v - 1;
			} else if (K < v && v <= N) {
				vz = 1, vi = N + 1 - v;
			} else {
				vz = 2, vi = v - N;
			}

			int64_t upref = 0;
			for (auto a : lines[uz].bit.prefix(ui)) upref += a;
			int64_t usuff = lines[uz].tot - upref;

			int64_t vpref = 0;
			for (auto a : lines[vz].bit.prefix(vi)) vpref += a;
			int64_t vsuff = lines[vz].tot - vpref;

			int64_t ans = INF;
			if (uz == vz) {
				ans = std::min(ans, std::abs(upref - vpref));
			}

			// you can go through 1
			ans = std::min(ans, upref + vpref);
			ans = std::min(ans, usuff + vsuff);

			int64_t d = std::min({lines[0].tot, lines[1].tot, lines[2].tot});
			ans = std::min(ans, upref + d + vsuff);
			ans = std::min(ans, usuff + d + vpref);

			if (ans >= INF) {
				cout << "impossible" << '\n';
			} else {
				cout << ans << '\n';
			}
		} else if (op == 'c') {
			int e; cin >> e;
			if (e < K) {
				toggle_edge(0, e-1);
			} else {
				toggle_edge(1, N-e);
			}
		} else if (op == 'x') {
			int e; cin >> e;
			toggle_edge(2, e);
		} else assert(false);
	}

	return 0;
}
