#include <bits/stdc++.h>

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

using std::cerr;

std::vector<int> solve(std::string S, std::string T, std::string P) {

	std::vector<int> pf_fail(P.size()+1);
	{
		pf_fail[0] = -1;
		for (int i = 0; i < int(P.size()); i++) {
			int cnd = pf_fail[i];
			while (cnd >= 0 && P[cnd] != P[i]) cnd = pf_fail[cnd];
			pf_fail[i+1] = cnd + 1;
		}
	}

	std::vector<std::array<int, 2>> pf_range(P.size()+1);
	{
		std::vector<std::vector<int>> ch(P.size()+1);
		for (int i = 1; i <= int(P.size()); i++) {
			ch[pf_fail[i]].push_back(i);
		}
		int idx = 0;
		std::y_combinator([&](auto self, int cur) -> void {
			pf_range[cur][0] = idx;
			idx++;
			for (int nxt : ch[cur]) {
				self(nxt);
			}
			pf_range[cur][1] = idx;
		})(0);
		assert(idx == int(P.size())+1);
	}

	std::vector<int> pr_fail(P.size()+1);
	{
		pr_fail[int(P.size())] = int(P.size())+1;
		for (int i = int(P.size()); i > 0; i--) {
			int cnd = pr_fail[i];
			while (cnd <= int(P.size()) && P[cnd-1] != P[i-1]) cnd = pr_fail[cnd];
			pr_fail[i-1] = cnd - 1;
		}
	}

	std::vector<std::array<int, 2>> pr_range(P.size()+1);
	{
		std::vector<std::vector<int>> ch(P.size()+1);
		for (int i = 0; i < int(P.size()); i++) {
			ch[pr_fail[i]].push_back(i);
		}
		int idx = 0;
		std::y_combinator([&](auto self, int cur) -> void {
			pr_range[cur][0] = idx;
			idx++;
			for (int nxt : ch[cur]) {
				self(nxt);
			}
			pr_range[cur][1] = idx;
		})(int(P.size()));
		assert(idx == int(P.size())+1);
	}

	std::vector<int> ans(int(S.size())+1);

	int t_for_state;
	{
		int cur = 0;
		for (int i = 0; i < int(T.size()); i++) {
			while (cur >= 0 && (cur == int(P.size()) || P[cur] != T[i])) cur = pf_fail[cur];
			cur++;
		}
		t_for_state = cur;
	}

	int t_rev_state;
	{
		int cur = int(P.size());
		for (int i = int(T.size()) - 1; i >= 0; i--) {
			while (cur <= int(P.size()) && (cur == 0 || P[cur-1] != T[i])) cur = pr_fail[cur];
			cur--;
		}
		t_rev_state = cur;
	}

	std::vector<int> s_for_state(int(S.size())+1);
	{
		int cur = 0;
		s_for_state[0] = cur;
		for (int i = 0; i < int(S.size()); i++) {
			while (cur >= 0 && (cur == int(P.size()) || P[cur] != S[i])) cur = pf_fail[cur];
			cur++;
			s_for_state[i+1] = cur;
		}
	}
	std::vector<int> s_rev_state(int(S.size())+1);
	{
		int cur = int(P.size());
		s_rev_state[int(S.size())] = cur;
		for (int i = int(S.size()) - 1; i >= 0; i--) {
			while (cur <= int(P.size()) && (cur == 0 || P[cur-1] != S[i])) cur = pr_fail[cur];
			cur--;
			s_rev_state[i] = cur;
		}
	}

	int t_contains_p = 0;
	{
		int cur = 0;
		for (int i = 0; i < int(T.size()); i++) {
			while (cur >= 0 && (cur == int(P.size()) || P[cur] != T[i])) cur = pf_fail[cur];
			cur++;
			t_contains_p += (cur == int(P.size()));
		}
	}


	std::vector<bool> p_has_t(int(P.size())+1);
	{
		std::vector<int> t_fail(T.size()+1);
		t_fail[0] = -1;
		for (int i = 0; i < int(T.size()); i++) {
			int cnd = t_fail[i];
			while (cnd >= 0 && T[i] != T[cnd]) cnd = t_fail[cnd];
			t_fail[i+1] = cnd + 1;
		}

		int cur = 0;
		p_has_t[0] = cur == int(T.size());
		for (int i = 0; i < int(P.size()); i++) {
			while (cur >= 0 && (cur == int(T.size()) || T[cur] != P[i])) cur = t_fail[cur];
			cur++;
			p_has_t[i+1] = (cur == int(T.size()));
		}
	}

	{
		int cnt = 0;
		for (int i = 0; i <= int(S.size()); i++) {
			cnt += (s_for_state[i] == int(P.size()));
			ans[i] += cnt;
		}
	}
	{
		int cnt = 0;
		for (int i = int(S.size()); i >= 0; i--) {
			cnt += (s_rev_state[i] == 0);
			ans[i] += cnt;
		}
	}

	for (int i = 0; i <= int(S.size()); i++) {
		ans[i] += t_contains_p;
	}

	{
		std::vector<int> pref(pf_fail.size()+1);
		for (int cur = t_rev_state; cur <= int(P.size()); cur = pr_fail[cur]) {
			if (0 < cur && cur < int(P.size())) {
				pref[pf_range[cur][0]]++;
				pref[pf_range[cur][1]]--;
			}
		}
		for (int i = 0; i+1 < int(pref.size()); i++) {
			pref[i+1] += pref[i];
		}
		assert(pref.back() == 0);
		for (int i = 0; i <= int(S.size()); i++) {
			ans[i] += pref[pf_range[s_for_state[i]][0]];
		}
	}

	{
		std::vector<int> pref(pf_fail.size()+1);
		for (int cur = t_for_state; cur >= 0; cur = pf_fail[cur]) {
			if (0 < cur && cur < int(P.size())) {
				pref[pr_range[cur][0]]++;
				pref[pr_range[cur][1]]--;
			}
		}
		for (int i = 0; i+1 < int(pref.size()); i++) {
			pref[i+1] += pref[i];
		}
		assert(pref.back() == 0);
		for (int i = 0; i <= int(S.size()); i++) {
			ans[i] += pref[pr_range[s_rev_state[i]][0]];
		}
	}

	std::vector<std::vector<std::pair<std::array<int, 2>, int>>> updates(pf_fail.size()+1);
	for (int i = 1, j = int(T.size())+1; j < int(P.size()); i++, j++) {
		if (p_has_t[j]) {
			auto xs = pf_range[i];
			auto ys = pr_range[j];
			updates[xs[0]].push_back({ys, +1});
			updates[xs[1]].push_back({ys, -1});
		}
	}

	std::vector<std::vector<std::pair<int, int>>> queries(pf_fail.size());

	for (int i = 0; i <= int(S.size()); i++) {
		int x = pf_range[s_for_state[i]][0];
		int y = pr_range[s_rev_state[i]][0];
		queries[x].push_back({y, i});
	}

	binary_indexed_tree<int> bit(pr_fail.size());
	for (int x = 0; x < int(pf_fail.size()); x++) {
		for (auto [ys, diff] : updates[x]) {
			for (auto& v : bit.prefix(ys[1])) v += diff;
			for (auto& v : bit.prefix(ys[0])) v -= diff;
		}
		for (auto [y, i] : queries[x]) {
			int contrib = 0;
			for (auto v : bit.suffix(y)) contrib += v;
			ans[i] += contrib;
		}
	}

	return ans;
}

std::vector<int> solve_slow(std::string S, std::string T, std::string P) {
	std::vector<int> ans(S.size()+1);
	for (int i = 0; i <= int(S.size()); i++) {
		std::string cur = S.substr(0, i) + T + S.substr(i);
		for (int z = 0; z + int(P.size()) <= int(cur.size()); z++) {
			if (cur.substr(z, int(P.size())) == P) {
				ans[i]++;
			}
		}
	}
	return ans;
}

void test() {
	std::mt19937 mt(48);
	for (int iter = 0; true; iter++) {
		if (iter % 1000 == 0) {
			cerr << "iter " << iter << '\n';
		}

		int LS = std::uniform_int_distribution<int>(1, 10)(mt);
		std::string S(LS, '\0');
		for (auto& c : S) c = std::uniform_int_distribution<char>('a', 'c')(mt);
		int LT = std::uniform_int_distribution<int>(1, 10)(mt);
		std::string T(LT, '\0');
		for (auto& c : T) c = std::uniform_int_distribution<char>('a', 'c')(mt);
		int LP = std::uniform_int_distribution<int>(1, 10)(mt);
		std::string P(LP, '\0');
		for (auto& c : P) c = std::uniform_int_distribution<char>('a', 'c')(mt);

		auto ans_slow = solve_slow(S, T, P);
		auto ans_fast = solve(S, T, P);
		if (ans_slow != ans_fast) {
			cerr << S << '\n';
			cerr << T << '\n';
			cerr << P << '\n';
			for (auto v : ans_slow) { cerr << v << ' '; } cerr << '\n';
			for (auto v : ans_fast) { cerr << v << ' '; } cerr << '\n';
			exit(1);
		}
	}
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	if (false) test();

	string S, T, P; cin >> S >> T >> P;

	std::vector<int> ans = solve(S, T, P);
	assert(ans.size() == S.size()+1);

	int best_ans = *std::max_element(ans.begin(), ans.end());
	int cnt_best = 0;
	int first_best = -1;
	int last_best = -1;
	for (int i = 0; i <= int(S.size()); i++) {
		if (ans[i] == best_ans) {
			cnt_best++;
			if (first_best == -1) first_best = i;
			last_best = i;
		}
	}
	cout << best_ans << ' ' << cnt_best << ' ' << first_best << ' ' << last_best << '\n';

	return 0;
}
