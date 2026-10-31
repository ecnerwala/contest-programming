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

	int N, K, Q; cin >> N >> K >> Q;
	vector<int> A(N); for (auto& a : A) { cin >> a; a--; }

	vector<int> freq(K); for (auto a : A) freq[a]++;

	int64_t cur_ans = 0;

	vector<int> cur_target(K); iota(cur_target.begin(), cur_target.end(), 0);
	vector<vector<pair<int, int>>> queries(K);

	for (int q = 0; q < Q; q++) {
		int j; cin >> j; j--;
		int a = cur_target[j];
		int b = cur_target[j+1];
		swap(cur_target[j], cur_target[j+1]);
		int weight = 1;
		if (make_pair(freq[a], a) < make_pair(freq[b], b)) {
			swap(a, b);
			weight = -1;
		}

		queries[b].push_back({a, weight == -1 ? ~q : q});
	}

	vector<int> ans_loc(Q);

	vector<int64_t> results; results.reserve(Q);
	for (int k = 0; k < K; k++) {
		sort(queries[k].begin(), queries[k].end());
		int j = 0;
		for (int i = 0; i < int(queries[k].size()); ) {
			int v = queries[k][i].first;
			int loc = int(results.size());
			results.push_back(-int64_t(freq[v]) * int64_t(freq[k]));
			for (; i < int(queries[k].size()) && queries[k][i].first == v; i++) {
				int q = queries[k][i].second;
				if (q < 0) ans_loc[~q] = ~loc;
				else ans_loc[q] = loc;
			}
			queries[k][j] = {v, loc};
			j++;
		}
		queries[k].resize(j);
	}

	binary_indexed_tree<int> bit(K);
	vector<int> cur_cnt(K);
	for (int a : A) {
		for (auto& v : bit.suffix(a)) cur_ans += v;
		for (auto [b, v] : queries[a]) {
			results[v] += 2 * cur_cnt[b];
		}
		for (auto& v : bit.prefix(a)) v++;
		cur_cnt[a]++;
	}

	for (int q = 0; q < Q; q++) {
		cur_ans += ans_loc[q] < 0 ? -results[~ans_loc[q]] : results[ans_loc[q]];
		cout << cur_ans << '\n';
	}

	return 0;
}

// Say there <= K frequent colors of size N/K
// Then, we can do N * K + Q * (N/K)
// What is sum of min(cnt a_i, cnt a_j)?
// It's 0 * max_0 + 1 * max_1 + 2 * max_2 + 3 * max_3 + ... sqrt(Q) * max_Q
// worst case is when there are sqrt(Q) colors and then it's N / sqrt(2Q) * Q = N sqrt(Q) / 2
