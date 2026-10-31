#include <bits/stdc++.h>

/** Binary-indexed tree
 *
 *  A binary indexed tree with N nodes of type T provides the
 *  following two functions for 0 <= i <= N
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

template <typename T> void setmin(T& a, T b) { if (b < a) a = b; }

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, Q; cin >> N >> Q;

	vector<array<int, 3>> holes(N);
	for (auto& h : holes) cin >> h[0] >> h[1] >> h[2];
	vector<pair<array<int, 3>, int>> queries(Q);
	for (auto& [h, _] : queries) cin >> h[0] >> h[1] >> h[2];
	for (int q = 0; q < Q; q++) queries[q].second = q;

	const int INF = 1e9;
	vector<int> ans(Q, INF);

	struct pareto_map {
		std::map<int, int> mp;
		void insert(int a, int v) {
			auto it = mp.upper_bound(a);
			if (it != mp.begin() && std::prev(it)->second >= v) {
				return;
			}
			if (it != mp.begin() && std::prev(it)->first == a) {
				std::prev(it)->second = v;
			} else {
				mp.insert(it, {a, v});
			}
			while (it != mp.end() && it->second <= v) {
				it = mp.erase(it);
			}
		}
		int query(int a) const {
			auto it = mp.upper_bound(a);
			if (it == mp.begin()) {
				return -INF;
			}
			--it;
			return it->second;
		}
	};

	auto do_solve = [&]() -> void {
		std::sort(holes.begin(), holes.end());
		std::sort(queries.begin(), queries.end());
		std::vector<int> vals; vals.reserve(N);
		for (int i = 0; i < N; i++) {
			vals.push_back(holes[i][1] - holes[i][0]);
		}
		std::sort(vals.begin(), vals.end());
		vals.resize(std::unique(vals.begin(), vals.end()) - vals.begin());
		binary_indexed_tree<pareto_map> bit(vals.size());

		auto hit = holes.begin();
		auto qit = queries.begin();
		for (; qit != queries.end(); ++qit) {
			for (; hit != holes.end() && (*hit)[0] <= qit->first[0]; ++hit) {
				int p = int(std::lower_bound(vals.begin(), vals.end(), (*hit)[1] - (*hit)[0]) - vals.begin());
				for (auto& mp : bit.suffix(p)) mp.insert((*hit)[2] - (*hit)[1], (*hit)[2]);
			}
			int p = int(std::upper_bound(vals.begin(), vals.end(), qit->first[1] - qit->first[0]) - vals.begin());
			int v = INF;
			for (const auto& mp : bit.prefix(p)) v = min(v, qit->first[2] - mp.query(qit->first[2] - qit->first[1]));
			setmin(ans[qit->second], v);
		}
	};

	do_solve();
	for (auto& h : holes) std::swap(h[1], h[2]);
	for (auto& [h, _] : queries) std::swap(h[1], h[2]);
	do_solve();
	for (auto& h : holes) std::swap(h[0], h[2]);
	for (auto& [h, _] : queries) std::swap(h[0], h[2]);
	do_solve();
	for (auto& h : holes) std::swap(h[1], h[2]);
	for (auto& [h, _] : queries) std::swap(h[1], h[2]);
	do_solve();
	for (auto& h : holes) std::swap(h[0], h[2]);
	for (auto& [h, _] : queries) std::swap(h[0], h[2]);
	do_solve();
	for (auto& h : holes) std::swap(h[1], h[2]);
	for (auto& [h, _] : queries) std::swap(h[1], h[2]);
	do_solve();

	for (int q = 0; q < Q; q++) {
		if (ans[q] == INF) cout << -1 << '\n';
		else cout << ans[q] << '\n';
	}

	return 0;
}
