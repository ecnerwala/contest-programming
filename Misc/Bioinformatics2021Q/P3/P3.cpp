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

template <typename T, class Compare = std::less<T>> class RangeMinQuery : private Compare {
	static const int BUCKET_SIZE = 32;
	static const int BUCKET_SIZE_LOG = 5;
	static_assert(BUCKET_SIZE == (1 << BUCKET_SIZE_LOG), "BUCKET_SIZE should be a power of 2");
	static const int CACHE_LINE_ALIGNMENT = 64;
	int n = 0;
	std::vector<T> data;
	std::vector<T> pref_data;
	std::vector<T> suff_data;
	std::vector<T> sparse_table;
	std::vector<uint32_t> range_mask;

private:
	int num_buckets() const {
		return n >> BUCKET_SIZE_LOG;
	}
	int num_levels() const {
		return num_buckets() ? 32 - __builtin_clz(num_buckets()) : 0;
	}
	int sparse_table_size() const {
		return num_buckets() * num_levels();
	}
private:
	const T& min(const T& a, const T& b) const {
		return Compare::operator()(a, b) ? a : b;
	}
	void setmin(T& a, const T& b) const {
		if (Compare::operator()(b, a)) a = b;
	}

	template <typename Vec> static int get_size(const Vec& v) { using std::size; return int(size(v)); }

public:
	RangeMinQuery() {}
	template <typename Vec> explicit RangeMinQuery(const Vec& data_, const Compare& comp_ = Compare())
		: Compare(comp_)
		, n(get_size(data_))
		, data(n)
		, pref_data(n)
		, suff_data(n)
		, sparse_table(sparse_table_size())
		, range_mask(n)
	{
		for (int i = 0; i < n; i++) data[i] = data_[i];
		for (int i = 0; i < n; i++) {
			if (i & (BUCKET_SIZE-1)) {
				uint32_t m = range_mask[i-1];
				while (m && !Compare::operator()(data[(i | (BUCKET_SIZE-1)) - __builtin_clz(m)], data[i])) {
					m -= uint32_t(1) << (BUCKET_SIZE - 1 - __builtin_clz(m));
				}
				m |= uint32_t(1) << (i & (BUCKET_SIZE - 1));
				range_mask[i] = m;
			} else {
				range_mask[i] = 1;
			}
		}
		for (int i = 0; i < n; i++) {
			pref_data[i] = data[i];
			if (i & (BUCKET_SIZE-1)) {
				setmin(pref_data[i], pref_data[i-1]);
			}
		}
		for (int i = n-1; i >= 0; i--) {
			suff_data[i] = data[i];
			if (i+1 < n && ((i+1) & (BUCKET_SIZE-1))) {
				setmin(suff_data[i], suff_data[i+1]);
			}
		}
		for (int i = 0; i < num_buckets(); i++) {
			sparse_table[i] = data[i * BUCKET_SIZE];
			for (int v = 1; v < BUCKET_SIZE; v++) {
				setmin(sparse_table[i], data[i * BUCKET_SIZE + v]);
			}
		}
		for (int l = 0; l+1 < num_levels(); l++) {
			for (int i = 0; i + (1 << (l+1)) <= num_buckets(); i++) {
				sparse_table[(l+1) * num_buckets() + i] = min(sparse_table[l * num_buckets() + i], sparse_table[l * num_buckets() + i + (1 << l)]);
			}
		}
	}

	T query(int l, int r) const {
		assert(l <= r);
		int bucket_l = (l >> BUCKET_SIZE_LOG);
		int bucket_r = (r >> BUCKET_SIZE_LOG);
		if (bucket_l == bucket_r) {
			uint32_t msk = range_mask[r] & ~((uint32_t(1) << (l & (BUCKET_SIZE-1))) - 1);
			int ind = (l & ~(BUCKET_SIZE-1)) + __builtin_ctz(msk);
			return data[ind];
		} else {
			T ans = min(suff_data[l], pref_data[r]);
			bucket_l++;
			if (bucket_l < bucket_r) {
				int level = (32 - __builtin_clz(bucket_r - bucket_l)) - 1;
				setmin(ans, sparse_table[level * num_buckets() + bucket_l]);
				setmin(ans, sparse_table[level * num_buckets() + bucket_r - (1 << level)]);
			}
			return ans;
		}
	}
};

template <typename T> using RangeMaxQuery = RangeMinQuery<T, std::greater<T>>;

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int N; cin >> N;

	vector<int> P(N);
	vector<vector<int>> ch(N);
	P[0] = -1;
	for (int i = 1; i < N; i++) { cin >> P[i]; P[i]--; assert(P[i] < i); ch[P[i]].push_back(i); }

	vector<int> V(N); for (auto& v : V) cin >> v;

	int M; cin >> M;
	vector<vector<int>> diseases(M);
	int tot_disease = 0;
	for (auto& d : diseases) {
		int L; cin >> L;
		tot_disease += L;
		d.resize(L);
		for (auto& v : d) { cin >> v; v--; }
	}

	int Q; cin >> Q;
	vector<vector<int>> queries(Q);
	int tot_query = 0;
	for (auto& q : queries) {
		int L; cin >> L;
		tot_query += L;
		q.resize(L);
		for (auto& v : q) { cin >> v; v--; }
	}

	cerr << N << ' ' << M << ' ' << tot_disease << ' ' << Q << ' ' << tot_query << '\n';

	assert(V[0] > 0);

	vector<int> depth(N);
	for (int i = 1; i < N; i++) depth[i] = depth[P[i]]+1;
	cerr << *max_element(depth.begin(), depth.end()) << '\n';
	cerr << accumulate(depth.begin(), depth.end(), int64_t(0)) / N << '\n';

	if (tot_disease == M && tot_query == Q) {
		vector<int> dp(N, -1);
		for (int i = 0; i < M; i++) {
			const auto& d = diseases[i];
			assert(d.size() == 1);
			int v = d[0];
			dp[v] = i;
		}
		for (int i = N-1; i > 0; i--) {
			if (dp[i] != -1) dp[P[i]] = dp[i];
		}
		for (int i = 0; i < N; i++) {
			dp[i] = dp[i] != -1 ? dp[i] : dp[P[i]];
		}
		for (auto& q : queries) {
			assert(q.size() == 1);
			int v = q[0];
			cout << dp[v]+1 << '\n';
		}
		exit(0);
	}

	vector<int> preorder_idx(N);
	int nxt_idx = 0;
	int cur_lca = V[0];
	vector<int> lca_depths; lca_depths.reserve(N-1);
	std::y_combinator([&](auto self, int cur) -> void {
		preorder_idx[cur] = nxt_idx++;
		if (cur) {
			lca_depths.push_back(cur_lca);
		}
		cur_lca = V[cur];
		for (int nxt : ch[cur]) {
			assert(nxt > cur);
			assert(V[nxt] >= V[cur]);
			cur_lca = min(cur_lca, V[cur]);
			self(nxt);
		}
	})(0);
	assert(nxt_idx == N);

	{
		vector<int> preorder_V(N);
		for (int i = 0; i < N; i++) preorder_V[preorder_idx[i]] = V[i];
		V = std::move(preorder_V);
	}
	{
		vector<int> preorder_P(N);
		preorder_P[0] = -1;
		for (int i = 1; i < N; i++) preorder_P[preorder_idx[i]] = preorder_idx[P[i]];
		P = std::move(preorder_P);
	}
	for (auto& d : diseases) {
		for (auto& v : d) v = preorder_idx[v];
		sort(d.begin(), d.end());
	}
	for (auto& q : queries) {
		for (auto& v : q) v = preorder_idx[v];
		sort(q.begin(), q.end());
	}

	RangeMinQuery<int> lca_query(lca_depths);
	auto get_lca = [&](int a, int b) -> int {
		if (a == b) return V[a];
		if (a > b) swap(a, b);
		assert(a < b);
		return lca_query.query(a, b-1);
	};

	auto evaluate = [&](const vector<int>& d, const vector<int>& q) -> int64_t {
		int64_t v = 0;
		for (int x = 0, y = 0; x < int(q.size()); x++) {
			while (y < int(d.size()) && d[y] < q[x]) y++;
			int lca = -1;
			if (y > 0) lca = max(lca, get_lca(q[x], d[y-1]));
			if (y < int(d.size())) lca = max(lca, get_lca(q[x], d[y]));
			v += lca;
		}
		return v;
	};

	if (*max_element(depth.begin(), depth.end()) <= 5) {
		vector<vector<int>> vert_diseases(N);
		for (int i = 0; i < M; i++) {
			for (int v : diseases[i]) {
				for (int cur = v; cur != 0; cur = P[cur]) {
					vert_diseases[cur].push_back(i);
				}
			}
		}
		for (const auto& q : queries) {
			vector<int> cnds;
			for (int v : q) {
				for (int cur = v; cur != 0; cur = P[cur]) {
					cnds.insert(cnds.end(), vert_diseases[cur].begin(), vert_diseases[cur].end());
				}
			}
			sort(cnds.begin(), cnds.end());
			cnds.resize(unique(cnds.begin(), cnds.end()) - cnds.begin());
			if (cnds.empty()) cnds.push_back(0);
			pair<int64_t, int> best_ans{-1, -1};
			for (int i : cnds) {
				int64_t v = evaluate(diseases[i], q);
				if (v >= best_ans.first) best_ans = {v, i};
			}
			cout << best_ans.second+1 << '\n';
		}
		exit(0);
	}
	if (tot_disease < int(1e5) || true) {
		for (int j = 0; j < Q; j++) {
			const auto& q = queries[j];
			pair<int64_t, int> best_ans{-1, -1};
			for (int i = 0; i < M; i++) {
				int64_t v = evaluate(diseases[i], q);
				if (v >= best_ans.first) {
					best_ans = {v, i};
				}
			}
			cout << best_ans.second+1 << '\n';
			if (j % 100 == 0) cerr << j << '\n';
		}
		exit(0);
	}
	assert(false);

	return 0;
}
