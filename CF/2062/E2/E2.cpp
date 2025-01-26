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

	// biggest thing outside of global max's subtree
	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> W_orig(N); for (auto& w : W_orig) { cin >> w; w--; }
		std::vector<std::vector<int>> adj_orig(N);
		for (int e = 0; e < N-1; e++) {
			int u, v; cin >> u >> v; u--, v--;
			adj_orig[u].push_back(v);
			adj_orig[v].push_back(u);
		}

		std::vector<int> orig_label(N, -1);
		std::vector<int> W(N, -1);
		std::vector<int> sz(N, -1);
		std::vector<int> max_par(N, -1);
		int nxt_idx = 0;
		[&](this auto&& self, int cur, int prv, std::pair<int, int> max_w) -> int {
			int cur_idx = nxt_idx++;
			orig_label[cur_idx] = cur;
			int w = W[cur_idx] = W_orig[cur];
			if (w >= max_w.first) {
				max_par[cur_idx] = max_w.second;
				max_w = {w, cur_idx};
			}
			int s = 1;
			for (int nxt : adj_orig[cur]) {
				if (nxt == prv) continue;
				s += self(nxt, cur, max_w);
			}
			return sz[cur_idx] = s;
		}(0, -1, {-1, N});

		std::vector<std::vector<int>> by_w(N);
		for (int i = 0; i < N; i++) by_w[W[i]].push_back(i);

		struct max_val_t {
			int w;
			int l, r;
			int cnt;
		};
		std::vector<max_val_t> pref_max(N+1);
		{
			max_val_t cur = pref_max[0] = {-1, -1, -1, 0};
			for (int i = 0; i < N; i++) {
				if (W[i] > cur.w) {
					cur = {W[i], i, i, 1};
				} else if (W[i] == cur.w) {
					cur.r = i;
					cur.cnt++;
				}
				pref_max[i+1] = cur;
			}
		}
		std::vector<max_val_t> suff_max(N+1);
		{
			max_val_t cur = suff_max[N] = {-1, -1, -1, 0};
			for (int i = N-1; i >= 0; i--) {
				if (W[i] > cur.w) {
					cur = {W[i], i, i, 1};
				} else if (W[i] == cur.w) {
					cur.l = i;
					cur.cnt++;
				}
				suff_max[i] = cur;
			}
		}

		binary_indexed_tree<int> bit_sub(N);
		binary_indexed_tree<int> bit_par(N);

		int cnt_ans = 0;
		std::vector<bool> good(N);
		int cnt_bigger = 0;
		for (int w = N-1; w >= 0; w--) {
			for (int i : by_w[w]) {
				int j = i + sz[i];

				max_val_t mx;
				{
					auto pm = pref_max[i];
					auto sm = suff_max[j];
					if (pm.w == sm.w) {
						mx = {pm.w, pm.l, sm.r, pm.cnt + sm.cnt};
					} else if (pm.w > sm.w) {
						mx = pm;
					} else if (sm.w > pm.w) {
						mx = sm;
					} else assert(false);
				}

				if (mx.w > w) {
					// we're a candidate, check stuff
					int cnt = 0;
					for (int v : bit_sub.prefix(j)) cnt += v;
					for (int v : bit_sub.prefix(i)) cnt -= v;

					cnt += mx.cnt;
					int lp = max_par[mx.l];
					if (lp != N && lp + sz[lp] > mx.r) {
						for (int v : bit_par.suffix(lp)) cnt += v;
					}
					assert(cnt <= cnt_bigger);
					if (cnt == cnt_bigger) {
						cnt_ans++;
						good[orig_label[i]] = true;
					}
				}
			}
			for (int i : by_w[w]) {
				int j = i + sz[i];
				if (max_par[i] != -1) {
					for (int& v : bit_par.prefix(j)) v++;
					for (int& v : bit_par.prefix(i)) v--;
				}
				for (int& v : bit_sub.suffix(i)) v++;
				cnt_bigger++;
			}
		}
		cout << cnt_ans; for (int i = 0; i < N; i++) { if (good[i]) cout << ' ' << i; } cout << '\n';
	}

	return 0;
}
