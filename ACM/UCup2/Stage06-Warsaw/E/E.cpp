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
	std::vector<std::vector<int>> orig_adj(N);
	for (int e = 0; e < N-1; e++) {
		int u, v; cin >> u >> v; u--, v--;
		orig_adj[u].push_back(v);
		orig_adj[v].push_back(u);
	}

	std::vector<int> preorder_idx(N);
	std::vector<int> preorder; preorder.reserve(N);

	std::vector<std::pair<int, int>> rmq_vals; rmq_vals.reserve(N);
	std::vector<int> small_depth(N);

	std::vector<int> small_par(N);
	std::vector<std::vector<int>> small_ch(N);
	int nxt_idx = 0;
	std::vector<int> small_sz(N);
	small_par[0] = -1;
	std::y_combinator([&](auto self, int cur, int prv, int d) -> void {
		int idx = nxt_idx++;
		preorder_idx[cur] = idx;
		preorder.push_back(cur);
		small_depth[idx] = d;
		small_ch[idx].reserve(orig_adj[cur].size());
		for (int nxt : orig_adj[cur]) {
			if (nxt == prv) continue;
			rmq_vals.push_back({d, idx});
			small_ch[idx].push_back(nxt_idx);
			small_par[nxt_idx] = idx;
			self(nxt, cur, d+1);
		}
		small_sz[idx] = nxt_idx - idx;
	})(0, -1, 0);
	assert(nxt_idx == N);
	assert(int(rmq_vals.size()) == N-1);

	RangeMinQuery<std::pair<int, int>> rmq(rmq_vals);
	auto lca = [&](int a, int b) -> int {
		if (a == b) return a;
		return rmq.query(std::min(a, b), std::max(a, b) - 1).second;
	};

	/*
	std::y_combinator([&](auto self, int cur) -> void {
		cerr << cur << ' ' << preorder[cur] << '\n';
		for (int nxt : small_ch[cur]) {
			cerr << ' ' << cur << " -> " << nxt << '\n';
			self(nxt);
		}
	})(0);
	*/

	int M; cin >> M;
	std::vector<std::array<int, 2>> paths(M);
	for (auto& [a, b] : paths) {
		cin >> a >> b; a--, b--;
		a = preorder_idx[a];
		b = preorder_idx[b];
	}

	std::vector<int> long_par(N);
	std::iota(long_par.begin(), long_par.end(), 0);
	std::vector<std::vector<int>> starting_paths(N);
	for (auto [a, b] : paths) {
		int c = lca(a, b);
		//cerr << a << ' ' << b << ' ' << c << '\n';
		assert(c <= a && a < c + small_sz[c]);
		assert(c <= b && b < c + small_sz[c]);
		long_par[a] = std::min(long_par[a], c);
		long_par[b] = std::min(long_par[b], c);
		starting_paths[a].push_back(b);
		starting_paths[b].push_back(a);
	}
	for (int i = N-1; i > 0; i--) {
		long_par[small_par[i]] = std::min(long_par[small_par[i]], long_par[i]);
		assert(long_par[i] < i);
	}
	assert(long_par[0] == 0);
	long_par[0] = -1;

	std::vector<int> long_depth(N);
	long_depth[0] = 0;
	for (int i = 1; i < N; i++) {
		long_depth[i] = long_depth[long_par[i]] + 1;
	}

	std::vector<int> long_sz(N, 1);
	for (int i = N-1; i >= 1; i--) {
		long_sz[long_par[i]] += long_sz[i];
	}

	std::vector<int64_t> anc_long_sz(N);
	anc_long_sz[0] = long_sz[0];
	for (int i = 1; i < N; i++) {
		anc_long_sz[i] = anc_long_sz[small_par[i]] + long_sz[i];
	}

	int64_t tot_long_sz = std::accumulate(long_sz.begin(), long_sz.end(), int64_t(0));

	std::vector<int64_t> up_cost(N);
	up_cost[0] = 0;
	for (int i = 1; i < N; i++) {
		up_cost[i] = up_cost[long_par[i]] + (N - small_sz[i]);
	}

	std::vector<int64_t> path_ends_val(N);
	std::vector<std::set<int>> path_ends(N);
	auto path_val = [&](int a, int b) -> int64_t {
		return anc_long_sz[b] - anc_long_sz[lca(a, b)];
	};
	auto insert_node = [&](int a, int v) -> void {
		auto [it, did_insert] = path_ends[a].insert(v);
		if (!did_insert) return;
		assert(it != path_ends[a].end());
		if (it != path_ends[a].begin() && std::next(it) != path_ends[a].end()) {
			path_ends_val[a] -= path_val(*std::prev(it), *std::next(it));
		}
		if (it != path_ends[a].begin()) {
			path_ends_val[a] += path_val(*std::prev(it), *it);
		}
		if (std::next(it) != path_ends[a].end()) {
			path_ends_val[a] += path_val(*it, *std::next(it));
		}
	};
	auto erase_node = [&](int a, int v) -> void {
		auto it = path_ends[a].find(v);
		assert(it != path_ends[a].end());
		if (it != path_ends[a].begin() && std::next(it) != path_ends[a].end()) {
			path_ends_val[a] += path_val(*std::prev(it), *std::next(it));
		}
		if (it != path_ends[a].begin()) {
			path_ends_val[a] -= path_val(*std::prev(it), *it);
		}
		if (std::next(it) != path_ends[a].end()) {
			path_ends_val[a] -= path_val(*it, *std::next(it));
		}
		path_ends[a].erase(it);
	};

	std::vector<int64_t> long_savings(N);
	for (int i = N-1; i >= 0; i--) {
		insert_node(i, 0);
		insert_node(i, i);

		for (int b : starting_paths[i]) {
			if (i <= b && b < i + small_sz[i]) continue;
			insert_node(i, b);
		}

		/*
		cerr << "at " << i << '\n';
		cerr << " ";
		for (int v : path_ends[i]) { cerr << v << ' '; } cerr << '\n';
		cerr << "v " << path_ends_val[i] << '\n';
		*/

		long_savings[i] = path_ends_val[i] - (anc_long_sz[i] - anc_long_sz[0]);

		if (i > 0) {
			int p = small_par[i];
			while (true) {
				auto it = path_ends[i].lower_bound(p+1);
				if (it != path_ends[i].end() && *it < p + small_sz[p]) {
					erase_node(i, *it);
				} else break;
			}
			if (path_ends[i].size() > path_ends[p].size()) {
				std::swap(path_ends[i], path_ends[p]);
				std::swap(path_ends_val[i], path_ends_val[p]);
			}
			for (int v : path_ends[i]) {
				insert_node(p, v);
			}
		}
	}

	std::vector<int64_t> anc_long_savings(N);
	anc_long_savings[0] = long_savings[0];
	for (int i = 1; i < N; i++) {
		anc_long_savings[i] = anc_long_savings[long_par[i]] + long_savings[i];
	}

	std::vector<int64_t> ans(N);
	for (int i = 0; i < N; i++) {
		ans[i] = tot_long_sz - anc_long_sz[i] + up_cost[i] - anc_long_savings[i];
	}

	/*
	for (int i = 0; i < N; i++) {
		cerr << i << ' ' << "small_par=" << small_par[i] << ' ' << "small_sz=" << small_sz[i] << ' ' << "long_par=" << long_par[i] << ' ' << "long_depth=" << long_depth[i] << ' ' << "long_sz=" << long_sz[i] << ' ' << "tot_long_sz=" << tot_long_sz << ' ' << "anc_long_sz=" << anc_long_sz[i] << ' ' << "up_cost=" << up_cost[i] << ' ' << "long_savings=" << long_savings[i] << ' ' << "anc_long_savings=" << anc_long_savings[i] << '\n';
	}
	*/

	for (int i = 0; i < N; i++) {
		cout << ans[preorder_idx[i]] << " \n"[i+1==N];
	}

	return 0;
}
