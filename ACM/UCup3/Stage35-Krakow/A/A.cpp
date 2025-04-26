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

namespace std {

template<class Fun>
class y_combinator_result {
	Fun fun_;
public:
	template<class T>
	explicit y_combinator_result(T &&fun): fun_(std::forward<T>(fun)) {}

	template<class ...Args>
	decltype(auto) operator()(Args &&...args) {
		return fun_(std::ref(*this), std::forward<Args>(args)...);
	}
};

template<class Fun>
decltype(auto) y_combinator(Fun &&fun) {
	return y_combinator_result<std::decay_t<Fun>>(std::forward<Fun>(fun));
}

} // namespace std

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, K; cin >> N >> K;

		std::vector<std::string> S(N);
		int L = 0;
		for (auto& s : S) { cin >> s; L += int(s.size()); }
		std::sort(S.begin(), S.end(), [&](const auto& a, const auto& b) { return a.size() < b.size(); });

		L++;

		std::vector<std::array<int, 26>> t_pref; t_pref.reserve(L+1);
		t_pref.emplace_back(); t_pref.back().fill(-1);
		std::vector<std::array<int, 26>> t_suff; t_suff.reserve(L+1);
		struct occ_t {
			int i;
			int len;
			int z;
		};
		std::vector<std::vector<occ_t>> suff_occs; suff_occs.reserve(L+1);
		t_suff.emplace_back(); t_suff.back().fill(-1);
		suff_occs.emplace_back();

		std::vector<std::vector<int>> pref_vals(N);

		for (int i = 0; i < N; i++) {
			{
				pref_vals[i].reserve(S[i].size() + 1);
				int cur = 0;
				pref_vals[i].push_back(cur);
				for (char c : S[i]) {
					if (t_pref[cur][c - 'A'] == -1) {
						t_pref[cur][c - 'A'] = int(t_pref.size());
						t_pref.emplace_back(); t_pref.back().fill(-1);
					}
					cur = t_pref[cur][c - 'A'];
					pref_vals[i].push_back(cur);
				}
			}

			{
				int cur = 0;
				suff_occs[cur].push_back({i, int(S[i].size()), int(S[i].size())});
				for (int z = int(S[i].size()) - 1; z >= 0; z--) {
					char c = S[i][z];
					if (t_suff[cur][c - 'A'] == -1) {
						t_suff[cur][c - 'A'] = int(t_suff.size());
						t_suff.emplace_back(); t_suff.back().fill(-1);
						suff_occs.emplace_back();
					}
					cur = t_suff[cur][c - 'A'];
					suff_occs[cur].push_back({i, int(S[i].size()), z});
				}
			}
		}

		// First, preprocess the prefix trie
		int V = int(t_pref.size());
		std::vector<std::array<int, 2>> st(V);
		{
			int nxt_idx = 0;
			std::y_combinator([&](auto&& self, int cur) -> void {
				if (cur == -1) return;
				st[cur][0] = nxt_idx;
				nxt_idx++;
				for (int nxt : t_pref[cur]) self(nxt);
				st[cur][1] = nxt_idx;
			})(0);
			assert(nxt_idx == V);
		}

		int64_t ans = 0;
		binary_indexed_tree<int> bit(V);
		for (auto occs : suff_occs) {
			for (int l = 0, r = 0; l < int(occs.size()); ) {
				if (r == int(occs.size()) || (l < r && occs[l].len + K < occs[r].len)) {
					// i is too early, so delete it
					for (auto& a : bit.suffix(st[pref_vals[occs[l].i].back()][0])) a--;
					l++;
				} else {
					int i = occs[r].i;
					int z = occs[r].z;
					if (z >= K) {
						// add in subtrees of pref_vals[i][z - K];
						for (auto a : bit.prefix(st[pref_vals[i][z - K]][1])) ans += a;
						for (auto a : bit.prefix(st[pref_vals[i][z - K]][0])) ans -= a;
					}
					if (z >= K+1) {
						// subtract out subtrees of pref_vals[i][z - K - 1]
						for (auto a : bit.prefix(st[pref_vals[i][z - K - 1]][1])) ans -= a;
						for (auto a : bit.prefix(st[pref_vals[i][z - K - 1]][0])) ans += a;
					}
					for (auto& a : bit.suffix(st[pref_vals[occs[r].i].back()][0])) a--;
					r++;
				}
			}
		}
		cout << ans << '\n';
	}

	return 0;
}
