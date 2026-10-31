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

std::vector<int64_t> solve(std::vector<int> A) {
	int N = int(A.size());

	std::vector<int64_t> res(N);

	binary_indexed_tree<int> bit(N);
	std::vector<bool> seen(N);

	int max2 = -1;
	int64_t ans = 0;

	{
		// i = 0
		assert(!seen[A[0]]);
		seen[A[0]] = true;
		for (auto& a : bit.prefix(A[0])) a++;
		res[0] = ans;
	}

	for (int i = 1; i < N; i++) {
		if (A[0] < A[i]) {
			std::swap(A[0], A[i]);
			ans ++;

			if (max2 == -1) {
				max2 = i;
			}
			ans += i - max2 + 1;
			max2 = -1;

			assert(!seen[A[0]]);
			seen[A[0]] = true;
			for (auto& a : bit.prefix(A[0])) a++;
		} else {
			if (max2 == -1 && A[i] == A[0]) max2 = i;

			for (auto a : bit.suffix(A[i])) ans += a;
			if (!seen[A[i]]) {
				seen[A[i]] = true;
				for (auto& a : bit.prefix(A[i])) a++;
			}
		}

		res[i] = ans;
	}

	return res;
}

std::vector<int64_t> solve_slow(std::vector<int> A) {
	std::vector<int64_t> res(A.size());
	std::vector<int> tmp(A.size());
	for (int k = 1; k <= int(A.size()); k++) {
		for (int i = 0; i < k; i++) tmp[i] = A[i];
		int64_t ans = 0;
		for (int i = 0; i < k; i++) {
			for (int j = 0; j < k; j++) {
				if (tmp[i] < tmp[j]) {
					std::swap(tmp[i], tmp[j]);
					ans++;
				}
			}
		}
		res[k-1] = ans;
	}
	return res;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	mt19937 mt(48);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> A(N);
		for (auto& a : A) { cin >> a; a--; }

		std::vector<int64_t> ans = solve(A);
		for (int i = 0; i < N; i++) {
			cout << ans[i] << " \n"[i+1==N];
		}

		/*
		cerr << T << '\n';
		int N = 5;
		std::vector<int> A(N);
		for (auto& a : A) { a = mt() % N; }

		std::vector<int64_t> slow = solve_slow(A);
		std::vector<int64_t> ans = solve(A);
		if (slow != ans) {
			for (int i = 0; i < N; i++) {
				cerr << A[i] << " \n"[i+1==N];
			}
			for (int i = 0; i < N; i++) {
				cerr << slow[i] << " \n"[i+1==N];
			}
			for (int i = 0; i < N; i++) {
				cerr << ans[i] << " \n"[i+1==N];
			}
			exit(0);
		}
		*/
	}

	return 0;
}
