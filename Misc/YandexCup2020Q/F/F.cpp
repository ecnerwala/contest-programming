#include<bits/stdc++.h>

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

std::vector<int> build_failure(std::string S) {
	using namespace std;
	vector<int> res(S.size()+1);
	res[0] = -1;
	for (int i = 0; i < int(S.size()); i++) {
		int cur = res[i];
		while (cur >= 0 && S[i] != S[cur]) cur = res[cur];
		res[i+1] = cur+1;
	}
	return res;
}

std::vector<std::array<int, 2>> build_preorder(std::vector<int> par) {
	using namespace std;
	int N = int(par.size());
	assert(par[0] == -1);
	vector<vector<int>> ch(N);
	for (int i = 1; i < N; i++) ch[par[i]].push_back(i);
	vector<array<int, 2>> res(N);
	int idx = 0;
	std::y_combinator([&](auto self, int cur) -> void {
		res[cur][0] = idx;
		idx++;
		for (int nxt : ch[cur]) self(nxt);
		res[cur][1] = idx;
	})(0);
	assert(idx == N);
	return res;
}

/** Binary-indexed tree
 *
 *  A binary indexed tree with N nodes of type T implicitly provides the
 *  following two functions for 0 <= i <= N:
 *
 *      prefix(int i) -> list<T&>
 *      suffix(int i) -> list<T&>
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
template <typename T> struct binary_indexed_tree {
private:
	std::vector<T> dat;
public:
	binary_indexed_tree() {}
	explicit binary_indexed_tree(size_t N) : dat(N) {}
	binary_indexed_tree(size_t N, const T& t) : dat(N, t) {}

	template <typename F> void for_prefix(int i, F f) const {
		assert(0 <= i && i <= int(dat.size()));
		for (int a = i; a; a &= a-1) {
			f(dat[a-1]);
		}
	}

	template <typename F> void for_prefix(int i, F f) {
		assert(0 <= i && i <= int(dat.size()));
		for (int a = i; a; a &= a-1) {
			f(dat[a-1]);
		}
	}

	template <typename F> void for_suffix(int i, F f) const {
		assert(0 <= i && i <= int(dat.size()));
		for (int a = i; a < int(dat.size()); a |= a+1) {
			f(dat[a]);
		}
	}

	template <typename F> void for_suffix(int i, F f) {
		assert(0 <= i && i <= int(dat.size()));
		for (int a = i; a < int(dat.size()); a |= a+1) {
			f(dat[a]);
		}
	}
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M; cin >> N >> M;
	string S; cin >> S;
	vector<string> G(N); for (auto& r : G) cin >> r;

	int L = int(S.size());
	auto fail_f = build_failure(S);
	auto preorder_f = build_preorder(fail_f);
	auto fail_r = build_failure(string(S.rbegin(), S.rend()));
	auto preorder_r = build_preorder(fail_r);

	assert(int(preorder_f.size()) == L+1);
	assert(int(preorder_r.size()) == L+1);

	int64_t ans = 0;

	vector<vector<int>> updates(L+1);
	vector<vector<pair<int, int>>> queries(L+2);

	for (int transposed = 0; transposed < 2; transposed ++) {
		vector<vector<int>> row_len(N, vector<int>(M));
		vector<vector<int>> col_len(N, vector<int>(M));

		for (int flip_row = 0; flip_row < 2; flip_row++) {
			for (int i = 0; i < N; i++) {
				int cur = 0;
				for (int j = 0; j < M; j++) {
					while (cur != -1 && (cur == L || S[cur] != G[i][j])) {
						cur = fail_f[cur];
					}
					cur++;
					row_len[i][j] = cur;

					if (cur == L) {
						ans++;
					}
				}
			}

			for (int flip_col = 0; flip_col < 2; flip_col++) {
				for (int i = 0; i < N; i++) {
					for (int j = 0; j < M; j++) {
						int cur = (i ? col_len[i-1][j] : 0);
						while (cur != -1 && (cur == L || S[L-1-cur] != G[i][j])) {
							cur = fail_r[cur];
						}
						cur++;
						col_len[i][j] = cur;

						if (row_len[i][j] >= 2 && col_len[i][j] >= 2 && row_len[i][j] + col_len[i][j] >= L+1) {
							int x = preorder_f[row_len[i][j]][0];
							int y = preorder_r[col_len[i][j]][0];
							updates[x].push_back(y);
						}
					}
				}

				reverse(G.begin(), G.end());
				reverse(row_len.begin(), row_len.end());
			}
			for (auto& r : G) reverse(r.begin(), r.end());
		}

		vector<string> G_(M, string(N, '\0'));
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M; j++) {
				G_[j][i] = G[i][j];
			}
		}
		G = std::move(G_);
		swap(N, M);
	}

	for (int i = 1; i+1 < L; i++) {
		auto x = preorder_f[i+1];
		auto y = preorder_r[L-i];
		queries[x[0]].push_back({y[0], +1});
		queries[x[1]].push_back({y[1], +1});
		queries[x[1]].push_back({y[0], -1});
		queries[x[0]].push_back({y[1], -1});
	}

	binary_indexed_tree<int> bit(L+1);
	for (int i = 0; i <= L+1; i++) {
		for (auto q : queries[i]) {
			bit.for_prefix(q.first, [&](int v) { ans += v * q.second; });
		}
		if (i <= L) {
			for (auto u : updates[i]) {
				bit.for_suffix(u, [&](int& v) { v++; });
			}
		}
	}

	cout << ans << '\n';

	return 0;
}
