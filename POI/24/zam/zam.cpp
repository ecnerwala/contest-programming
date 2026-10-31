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

	int W, H, N, M; cin >> W >> H >> N >> M;
	int stx, sty, enx, eny; cin >> stx >> sty >> enx >> eny;

	int st = -1, en = -1;

	vector<tuple<int, int, int>> corners; corners.reserve(4*N);
	vector<int> ys; ys.reserve(2*N);
	for (int i = 0; i < N; i++) {
		int x1, y1, x2, y2; cin >> x1 >> y1 >> x2 >> y2;
		if (x1 > x2) swap(x1, x2);
		if (y1 > y2) swap(y1, y2);
		if (x1 < stx && stx < x2 && y1 < sty && sty < y2) { assert(st == -1); st = i; }
		if (x1 < enx && enx < x2 && y1 < eny && eny < y2) { assert(en == -1); en = i; }

		corners.push_back({x1, y1, i});
		corners.push_back({x1, y2, i});
		corners.push_back({x2, y1, i});
		corners.push_back({x2, y2, i});

		ys.push_back(y1);
		ys.push_back(y2);
	}

	assert(st != -1 && en != -1);

	vector<bool> bad(N, false);

	vector<array<int, 2>> marks(M);
	for (auto& v : marks) cin >> v[0] >> v[1];
	sort(marks.begin(), marks.end());

	sort(corners.begin(), corners.end());

	sort(ys.begin(), ys.end());
	ys.resize(unique(ys.begin(), ys.end()) - ys.begin());
	// Now, identify the bad regions
	{
		binary_indexed_tree<int> bit(ys.size());
		auto it = corners.begin();
		for (auto [x, y] : marks) {
			for (; it != corners.end() && get<0>(*it) < x; ++it) {
				int idx = int(lower_bound(ys.begin(), ys.end(), get<1>(*it)) - ys.begin());
				for (auto& v : bit.prefix(idx)) v ^= get<2>(*it);
			}
			int idx = int(lower_bound(ys.begin(), ys.end(), y) - ys.begin()) - 1;
			int cell = 0;
			for (const auto& v : bit.suffix(idx)) cell ^= v;
			bad[cell] = true;
		}
	}

	vector<vector<int>> adj(N);
	// Now build the graph
	for (int z = 0; z < 2; z++) {
		if (z) {
			for (auto& v : corners) swap(get<0>(v), get<1>(v));
			sort(corners.begin(), corners.end());
		}

		vector<int> cur_set; cur_set.reserve(6);
		for (int a = 0, b; a < int(corners.size()); a = b) {
			assert(int(cur_set.size()) <= 2);
			b = a;
			while (b < int(corners.size())
				&& get<0>(corners[b]) == get<0>(corners[a]) && get<1>(corners[b]) == get<1>(corners[a])) {
				cur_set.push_back(get<2>(corners[b]));
				b++;
			}

			// Nothing occurs 3 times, so this is fine
			sort(cur_set.begin(), cur_set.end());
			int nsz = 0;
			for (int i = 0; i < int(cur_set.size()); i++) {
				if (nsz && cur_set[i] == cur_set[nsz-1]) {
					nsz--;
				} else {
					cur_set[nsz++] = cur_set[i];
				}
			}
			cur_set.resize(nsz);

			if (int(cur_set.size()) == 2) {
				adj[cur_set[0]].push_back(cur_set[1]);
				adj[cur_set[1]].push_back(cur_set[0]);
			}
		}
	}

	vector<int> dist(N, -1);
	vector<int> q; q.reserve(N);
	dist[st] = 1;
	q.push_back(st);
	for (int z = 0; z < int(q.size()); z++) {
		int cur = q[z];
		for (int nxt : adj[cur]) {
			if (!bad[nxt] && dist[nxt] == -1) {
				dist[nxt] = dist[cur] + 1;
				q.push_back(nxt);
			}
		}
	}
	cout << dist[en] << '\n';

	return 0;
}
