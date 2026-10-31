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

	int N; cin >> N;
	std::vector<int> P(N);
	for (auto& a : P) { cin >> a; a--; }

	std::vector<int> invP(N);
	for (int i = 0; i < N; i++) invP[P[i]] = i;

	int Q; cin >> Q;
	std::vector<std::array<int, 2>> queries(Q);
	std::vector<int64_t> ans(Q, 0);
	std::vector<std::vector<std::tuple<int, bool>>> query_pts(N);

	for (int q = 0; q < Q; q++) {
		auto& [x, y] = queries[q];
		cin >> x >> y; x--, y--;
		if (x == y) continue;

		if (P[x] > P[y]) std::swap(x, y);
		assert(P[x] < P[y]);
		query_pts[P[x]].push_back({q, 0});
		query_pts[P[y]].push_back({q, 1});
	}

	int64_t base_tot = 0;

	binary_indexed_tree<int> pref_bit(N);
	binary_indexed_tree<int> move_left_bit(N);
	binary_indexed_tree<int> move_right_bit(N);

	for (int v = 0; v < N; v++) {
		int i = invP[v];
		int cnt_left = 0;
		for (auto a : pref_bit.prefix(i)) cnt_left += a;
		int cnt_right = v - cnt_left;
		int base_C = std::min(cnt_left, cnt_right);

		for (auto [q, is_big] : query_pts[v]) {
			bool moves_right = queries[q][0] < queries[q][1];
			//cerr << "query " << q << ' ' << queries[q][0] << "->" << queries[q][1] << ' ' << moves_right << '\n';

			ans[q] -= base_C;
			{
				int new_cnt_left = 0;
				for (auto a : pref_bit.prefix(queries[q][!is_big])) new_cnt_left += a;
				if (is_big) {
					if (moves_right) {
						//new_cnt_left--;
					} else {
						new_cnt_left++;
					}
				}
				int new_cnt_right = v - new_cnt_left;
				ans[q] += std::min(new_cnt_left, new_cnt_right);
				//cerr << "delta " << "base_C " << ' ' << base_C << ' ' << std::min(new_cnt_left, new_cnt_right) << '\n';
			}

			{
				const auto& bit = moves_right ? move_right_bit : move_left_bit;
				int val = 0;
				for (auto a : bit.prefix(std::min(queries[q][0], queries[q][1]) + 1)) val -= a;
				for (auto a : bit.prefix(std::max(queries[q][0], queries[q][1]))) val += a;
				val *= (is_big ? 1 : -1);
				ans[q] += val;
				//cerr << "delta " << "val " << val << '\n';
			}
		}

		//cerr << "v = " << v << ' ' << "i = " << i << '\n';
		//cerr << "base C" << base_C << '\n';
		base_tot += base_C;
		for (auto& a : pref_bit.suffix(i)) a++;

		{
			int move_left_C = std::min(cnt_left + 1, cnt_right - 1);
			//cerr << "move left C" << move_left_C << '\n';
			for (auto& a : move_left_bit.suffix(i)) a += move_left_C - base_C;
		}
		{
			int move_right_C = std::min(cnt_left - 1, cnt_right + 1);
			//cerr << "move right C" << move_right_C << '\n';
			for (auto& a : move_right_bit.suffix(i)) a += move_right_C - base_C;
		}
	}

	for (int q = 0; q < Q; q++) {
		ans[q] += base_tot;
		cout << ans[q] << '\n';
	}


	return 0;
}
