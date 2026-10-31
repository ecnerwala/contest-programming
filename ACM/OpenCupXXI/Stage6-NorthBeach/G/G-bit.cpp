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

	for (int N, Q; cin >> N >> Q; ) {
		using pt_t = array<int, 2>;
		vector<pt_t> coords(N+1);
		for (int i = 0; i < N; i++) {
			cin >> coords[i][0] >> coords[i][1];
		}
		coords[N] = coords[0];
		vector<pt_t> queries(Q);
		for (int q = 0; q < Q; q++) {
			cin >> queries[q][0] >> queries[q][1];
		}

		// evts by which axis is sweeping
		array<vector<pair<int, pair<bool, int>>>, 2> evts;
		evts[0].reserve(N);
		evts[1].reserve(N);

		array<vector<int>, 2> vals;
		vals[0].reserve(N/2);
		vals[1].reserve(N/2);

		for (int i = 0; i < N; i++) {
			pt_t p = coords[i], q = coords[i+1];
			int z = (p[1] != q[1]);
			assert(p[z] != q[z]);
			assert(p[!z] == q[!z]);
			if (p[z] > q[z]) swap(p, q);
			evts[z].push_back({p[z], {0, p[!z]}});
			evts[z].push_back({q[z], {1, q[!z]}});
			vals[z].push_back(p[!z]);
		}
		array<binary_indexed_tree<int>, 2> bit;
		for (int z = 0; z < 2; z++) {
			assert(int(evts[z].size()) == N);
			sort(evts[z].begin(), evts[z].end());
			assert(int(vals[z].size())*2 == N);
			sort(vals[z].begin(), vals[z].end());
			for (auto& e : evts[z]) {
				e.second.second = int(lower_bound(vals[z].begin(), vals[z].end(), e.second.second) - vals[z].begin());
			}
			bit[z] = binary_indexed_tree<int>(vals[z].size());
		}

		array<vector<tuple<int, pair<int, int>, int>>, 2> qevents;
		qevents[0].reserve(2*Q);
		qevents[1].reserve(2*Q);

		vector<int> ans(Q);
		for (int lg = 1 << 27; lg >= 1; lg /= 2) {
			qevents[0].clear();
			qevents[1].clear();
			for (int q = 0; q < Q; q++) {
				auto pt = queries[q];
				int len = (ans[q] += lg);
				for (int z = 0; z < 2; z++) {
					qevents[z].push_back({pt[z], {pt[!z], pt[!z]+len}, q});
					qevents[z].push_back({pt[z]+len, {pt[!z], pt[!z]+len}, q});
				}
			}
			for (int z = 0; z < 2; z++) {
				sort(qevents[z].begin(), qevents[z].end());
				auto it1 = evts[z].begin();
				for (auto [x, bounds, q] : qevents[z]) {
					for (; it1 != evts[z].end() && it1->first <= x; ++it1) {
						auto [d, v] = it1->second;
						int change = (d ? -1 : +1);
						for (int& a : bit[z].suffix(v)) {
							a += change;
						}
					}
					auto l = int(upper_bound(vals[z].begin(), vals[z].end(), bounds.first) - vals[z].begin());
					auto r = int(upper_bound(vals[z].begin(), vals[z].end(), bounds.second) - vals[z].begin());
					int cnt = 0;
					for (int a : bit[z].prefix(r)) {
						cnt += a;
					}
					for (int a : bit[z].prefix(l)) {
						cnt -= a;
					}
					assert(cnt >= 0);
					if (cnt) {
						ans[q] &= ~lg;
					}
				}
				for (; it1 != evts[z].end(); ++it1) {
					auto [d, v] = it1->second;
					int change = (d ? -1 : +1);
					for (int& a : bit[z].suffix(v)) {
						a += change;
					}
				}
			}
		}

		for (int q = 0; q < Q; q++) {
			cout << (ans[q]+1) << '\n';
		}
	}

	return 0;
}
