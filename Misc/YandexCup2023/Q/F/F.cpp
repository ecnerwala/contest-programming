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

	int N, M, Q; cin >> N >> M >> Q;
	std::vector<int> X(N);
	for (auto& x : X) cin >> x;
	std::vector<int> Y(M);
	for (auto& y : Y) cin >> y;

	bool should_swap = N > M;
	if (should_swap) {
		std::swap(N, M);
		std::swap(X, Y);
	}
	assert(N <= M);

	const int V = int(1e6);
	binary_indexed_tree<int> bit(V+1);
	for (int y : Y) {
		for (auto& a : bit.prefix(y)) a++;
	}

	auto get_lt = [&](int num, int den) -> int64_t {
		// we need x / y < num / den
		int64_t cnt = 0;
		for (int x : X) {
			// y > x * den / num
			// y > floor(x * den / num)
			for (auto a : bit.suffix(
				int(std::min<int64_t>(V, int64_t(den) * int64_t(x) / int64_t(num)))
			)) {
				cnt += a;
			}
		}
		return cnt;
	};

	auto bsearch = [&](auto&& is_good) -> int {
		assert(is_good(1));
		int mi = 1;
		int ma = 2;
		while (is_good(ma)) {
			mi = ma;
			ma *= 2;
		}
		while (ma - mi > 1) {
			int md = mi + (ma - mi) / 2;
			if (is_good(md)) mi = md;
			else ma = md;
		}
		return mi;
	};

	for (int q = 0; q < Q; q++) {
		int op; cin >> op;
		if (op == 1) {
			int64_t k; cin >> k; k--;
			if (should_swap) k = int64_t(N) * int64_t(M) - 1 - k;
			int num_lo = 0, den_lo = 1;
			int num_hi = 1, den_hi = 0;
			while (num_lo + num_hi <= V && den_lo + den_hi <= V) {
				if (get_lt(num_lo + num_hi, den_lo + den_hi) <= k) {
					// go upwards
					int w = bsearch([&](int cnd) {
						return num_lo + int64_t(cnd) * num_hi <= V && den_lo + int64_t(cnd) * den_hi <= V && get_lt(num_lo + cnd * num_hi, den_lo + cnd * den_hi) <= k;
					});
					num_lo += w * num_hi;
					den_lo += w * den_hi;
				} else {
					// go downwards
					int w = bsearch([&](int cnd) {
						return int64_t(cnd) * num_lo + num_hi <= V && int64_t(cnd) * den_lo + den_hi <= V && get_lt(cnd * num_lo + num_hi, cnd * den_lo + den_hi) > k;
					});
					num_hi += w * num_lo;
					den_hi += w * den_lo;
				}
			}
			if (should_swap) {
				cout << den_lo << ' ' << num_lo << '\n';
			} else {
				cout << num_lo << ' ' << den_lo << '\n';
			}
		} else if (op == (should_swap ? 3 : 2)) {
			int i, v; cin >> i >> v; i--;
			X[i] = v;
		} else if (op == (should_swap ? 2 : 3)) {
			int i, v; cin >> i >> v; i--;
			for (auto& a : bit.prefix(Y[i])) a--;
			Y[i] = v;
			for (auto& a : bit.prefix(Y[i])) a++;
		} else assert(false);
	}

	return 0;
}
