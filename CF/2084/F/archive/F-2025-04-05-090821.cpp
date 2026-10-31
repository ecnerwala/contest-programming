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
public:
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

template <typename T> using min_pq = std::priority_queue<T, std::vector<T>, std::greater<T>>;

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> A(N); for (auto& a : A) { cin >> a; a--; }
		std::vector<int> C(N); for (auto& c : C) { cin >> c; c--; }
		std::vector<int> invA(N);
		for (int i = 0; i < N; i++) invA[A[i]] = i;
		std::vector<int> invC(N, -1);
		for (int i = 0; i < N; i++) if (C[i] != -1) invC[C[i]] = i;


		bool good = [&]() -> bool {
			// store value -> range
			std::vector<int> mi(N);
			std::vector<int> ma(N);

			{
				binary_indexed_tree<int> bit(N, 0);
				for (int i = 0; i < N; i++) {
					int v = A[i];
					if (invC[v] != -1) {
						int r = invC[v];
						for (auto& a : bit.suffix(v)) a = std::max(a, r);
					}
					{
						int r = 0;
						for (auto a : bit.prefix(v+1)) r = std::max(r, a);
						mi[v] = r;
					}
				}
			}
			{
				binary_indexed_tree<int> bit(N, N-1);
				for (int i = N-1; i >= 0; i--) {
					int v = A[i];
					if (invC[v] != -1) {
						int r = invC[v];
						for (auto& a : bit.prefix(v+1)) a = std::max(a, r);
					}
					{
						int r = N-1;
						for (auto a : bit.suffix(v)) r = std::max(r, a);
						ma[v] = r;
					}
				}
			}
			std::vector<std::vector<int>> by_mi(N);
			for (int v = 0; v < N; v++) {
				cerr << v << ' ' << mi[v] << '-' << ma[v] << '\n';
				by_mi[mi[v]].push_back(v);
			}
			min_pq<std::pair<int, int>> pq;
			std::vector<int> B(N);
			for (int i = 0; i < N; i++) {
				for (auto v : by_mi[i]) {
					pq.push({ma[v], v});
				}
				{
					if (pq.empty()) return false;
					auto [cnd, v] = pq.top(); pq.pop();
					if (cnd < i) return false;
					if (C[i] != -1 && v != C[i]) return false;
					C[i] = v;
				}
			}
			return true;
		}();

		if (good) {
			for (int i = 0; i < N; i++) {
				cout << C[i]+1 << " \n"[i+1==N];
			}
		} else {
			cout << -1 << '\n';
		}
	}

	return 0;
}
