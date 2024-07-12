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

using std::cerr;

template <typename T> void setmin(T& a, T b) { if (b < a) a = b; }
template <typename T> void setmax(T& a, T b) { if (b > a) a = b; }

int brute_force(std::vector<int> P) {
	int N = int(P.size());
	auto eval = [&](int msk) -> int {
		int val = 0;
		for (int i = 0; i < N; i++) {
			for (int j = i+1; j < N; j++) {
				int vi = P[i] + N * ((msk >> i) & 1);
				int vj = P[j] + N * ((msk >> j) & 1);
				if (vi > vj) val++;
			}
		}
		return val;
	};
	std::vector<int> vals(1 << N);
	std::vector<int> best_val(N+1, N*N);
	for (int msk = 0; msk < (1 << N); msk++) {
		vals[msk] = eval(msk);
		setmin(best_val[__builtin_popcount(msk)], vals[msk]);
	}
	int cur_msk = 0;
	for (int i = 1; i <= N; i++) {
		int nxt_msk = -1;
		for (int j = 0; j < N; j++) {
			if (cur_msk & (1 << j)) continue;
			int cnd_msk = cur_msk | (1 << j);
			if (nxt_msk == -1 || vals[cnd_msk] < vals[nxt_msk]) nxt_msk = cnd_msk;
		}
		assert(__builtin_popcount(nxt_msk) == i);
		assert(best_val[i] == vals[nxt_msk]);
		int j = __builtin_ctz(nxt_msk ^ cur_msk);
		assert(nxt_msk == (cur_msk + (1 << j)));
		//cerr << (vals[nxt_msk] - vals[cur_msk]) - (P[j] - 2*j + N-i) << '\n';
		cur_msk = nxt_msk;
	}
	return *std::min_element(vals.begin(), vals.end());
}

int64_t solve(std::vector<int> P) {
	int N = int(P.size());
	binary_indexed_tree<int> bit(N);
	int64_t inv_cnt = 0;
	for (int v : P) {
		for (auto& a : bit.suffix(v)) inv_cnt += a;
		for (auto& a : bit.prefix(v)) a++;
	}
	std::vector<int> items(N);
	for (int j = 0; j < N; j++) {
		items[j] = P[j] - 2 * j;
	}
	std::sort(items.begin(), items.end());
	int64_t ans = inv_cnt;
	for (int i = 1; i <= N; i++) {
		inv_cnt += items[i-1] + (N-i);
		ans = std::min(ans, inv_cnt);
	}
	return ans;
}

void test() {
	for (int N = 1; N <= 10; N++) {
		cerr << "testing " << N << '\n';
		std::vector<int> P(N);
		std::iota(P.begin(), P.end(), 0);
		do {
			auto v = brute_force(P);
			assert(v == solve(P));
		} while (std::next_permutation(P.begin(), P.end()));
	}
	exit(0);
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	//test();

	int N; cin >> N;
	std::vector<int> P(N); for (auto& p : P) { cin >> p; p--; }
	cout << solve(P) << '\n';
	
	return 0;
}
