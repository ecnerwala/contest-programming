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

std::vector<int> solve_1s(const std::string& S, const std::vector<std::pair<std::array<int, 2>, int>>& queries) {
	int N = int(S.size());
	int Q = int(queries.size());
	std::vector<int> ans(Q);

	std::vector<std::vector<int>> q_left(N+1);
	std::vector<std::vector<int>> q_right(N+1);
	for (int q = 0; q < int(queries.size()); q++) {
		q_left[queries[q].first[0]].push_back(q);
		q_right[queries[q].first[1]].push_back(q);
	}

	std::vector<int> pref(N+1);
	for (int i = 0; i < N; i++) {
		assert(S[i] == '0' || S[i] == '1');
		pref[i+1] = pref[i] + (S[i] == '1' ? +1 : -1);
	}

	std::vector<int> whole_height(N, 0);
	{
		std::vector<std::pair<int, int>> stk; stk.reserve(N);
		for (int i = 0; i < N || !stk.empty(); i++) {
			if (i < N && S[i] == '1') {
				stk.push_back({i, 1});
			} else if (!stk.empty()) {
				auto [l, h] = stk.back();
				whole_height[l] = h;
				stk.pop_back();
				if (!stk.empty()) stk.back().second = std::max(stk.back().second, h + 1);
			}
		}
	}

	binary_indexed_tree<int> bit(N+1);
	std::vector<int> stk_idx; stk_idx.reserve(N);
	std::vector<int> stk_whole; stk_whole.reserve(N);
	std::vector<std::pair<int, std::array<int, 2>>> stk_suff; stk_suff.reserve(N);
	for (int i = 0; i < N; i++) {
		for (int q : q_left[i]) {
			int res = 0;
			for (auto a : bit.suffix(queries[q].second)) {
				res += a;
			}
			ans[q] -= res;
		}

		if (S[i] == '1') {
			stk_idx.push_back(i);

			for (auto& a : bit.prefix(whole_height[i])) a++;

			if (!stk_whole.empty()) assert(stk_whole.back() > whole_height[i]);
			stk_whole.push_back(whole_height[i]);

			int h = int(stk_idx.size());
			if (stk_suff.empty() || stk_suff.back().first > stk_suff.back().second[1]) {
				stk_suff.push_back({h-1, {h-1, h-1}});
			}
			stk_suff.back().first++;
			stk_suff.back().second[1]++;
			if (stk_suff.size() >= 2 && stk_suff.end()[-2].first == stk_suff.end()[-1].first) {
				assert(stk_suff.end()[-2].second[1] == stk_suff.end()[-1].second[0]);
				stk_suff.end()[-2].second[1] = stk_suff.end()[-1].second[1];
				stk_suff.pop_back();
			}
		} else if (!stk_idx.empty()) {
			stk_idx.pop_back();
			stk_whole.pop_back();
			stk_suff.back().second[1] --;
			if (stk_suff.back().second[1] == stk_suff.back().second[0]) {
				stk_suff.pop_back();
			}
		}

		for (int q : q_right[i+1]) {
			{
				int res = 0;
				for (auto a : bit.suffix(queries[q].second)) {
					res += a;
				}
				ans[q] += res;
			}

			int l = queries[q].first[0];

			int first_unclosed;
			{
				int mi = -1;
				int ma = int(stk_idx.size());
				while (ma - mi > 1) {
					int md = (mi + ma) / 2;
					if (stk_idx[md] < l) {
						mi = md;
					} else {
						ma = md;
					}
				}
				first_unclosed = ma;
			}

			int k = queries[q].second;

			int first_whole_keep;
			{
				int mi = -1;
				int ma = int(stk_whole.size());
				while (ma - mi > 1) {
					int md = (mi + ma) / 2;
					if (stk_whole[md] > k) {
						mi = md;
					} else {
						ma = md;
					}
				}
				first_whole_keep = ma;
			}
			first_whole_keep = std::max(first_whole_keep, first_unclosed);

			int first_suff_keep;
			{
				int mi = -1;
				int ma = int(stk_suff.size());
				while (ma - mi > 1) {
					int md = (mi + ma) / 2;
					if (stk_suff[md].first - stk_suff[md].second[0] > k) {
						mi = md;
					} else {
						ma = md;
					}
				}

				if (mi >= 0) {
					int h = stk_suff[mi].first;
					auto [x, y] = stk_suff[mi].second;
					assert(h - x > k);
					first_suff_keep = std::min(y, h-k);
				} else {
					first_suff_keep = 0;
				}
			}
			first_suff_keep = std::max(first_suff_keep, first_unclosed);

			ans[q] += first_suff_keep - first_whole_keep;
		}
	}

	return ans;
}

std::vector<int> solve_1s_slow(const std::string& S, const std::vector<std::pair<std::array<int, 2>, int>>& queries) {
	int Q = int(queries.size());
	std::vector<int> ans(Q);

	for (int q = 0; q < Q; q++) {
		auto [l, r] = queries[q].first;
		int k = queries[q].second;
		std::string cur(S.begin()+l, S.begin()+r);
		for (int z = 0; z < k && !cur.empty(); z++) {
			int prv = -1;
			int n_len = 0;
			for (int i = 0; i < int(cur.size()); i++) {
				if (i > 0 && cur[i] == prv) {
					cur[n_len++] = cur[i];
				} else {
					prv = cur[i];
				}
			}
			cur.resize(n_len);
		}
		for (char c : cur) ans[q] += (c == '1');
	}

	return ans;
}

void test() {
	for (int N = 1; N <= 10; N++) {
		for (int m = 0; m < (1 << N); m++) {
			std::string S(N, '\0');
			for (int i = 0; i < N; i++) {
				S[i] = char('0' + ((m >> i) & 1));
			}
			std::vector<std::pair<std::array<int, 2>, int>> queries;
			for (int l = 0; l < N; l++) {
				for (int r = l+1; r <= N; r++) {
					for (int k = 0; k <= N; k++) {
						queries.push_back({{l, r}, k});
					}
				}
			}
			auto res = solve_1s(S, queries);
			auto slo = solve_1s_slow(S, queries);
			if (res != slo) {
				std::cerr << "Mismatch!" << '\n';
				std::cerr << S << '\n';
				for (int q = 0; q < int(queries.size()); q++) {
					if (res[q] != slo[q]) {
						auto [l, r] = queries[q].first;
						int k = queries[q].second;
						std::cerr << l << ' ' << r << ' ' << k << ": " << slo[q] << ' ' << res[q] << '\n';
					}
				}
				exit(0);
			}
		}
	}
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	//test();

	int N, Q; cin >> N >> Q;
	std::string S; cin >> S;
	assert(int(S.size()) == N);
	std::vector<std::pair<std::array<int, 2>, int>> queries(Q);
	for (auto& [x, k] : queries) {
		cin >> x[0] >> x[1] >> k; x[0]--;
		assert(0 <= x[0] && x[0] < x[1] && x[1] <= N);
	}

	std::vector<int> ans(Q);

	for (int z = 0; z < 2; z++) {
		if (z == 1) {
			std::reverse(S.begin(), S.end());
			for (auto& c : S) c ^= 1;
			for (auto& [x, k] : queries) {
				x = {N - x[1], N - x[0]};
				assert(0 <= x[0] && x[0] < x[1] && x[1] <= N);
			}
		}

		auto a1 = solve_1s(S, queries);
		for (int q = 0; q < Q; q++) {
			ans[q] += a1[q];
		}
	}

	for (int q = 0; q < Q; q++) {
		cout << ans[q] << '\n';
	}

	return 0;
}
