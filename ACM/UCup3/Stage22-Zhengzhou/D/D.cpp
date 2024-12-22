#include <bits/stdc++.h>

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

struct static_forest_t {
	int N;
	// original label to preorder
	std::vector<int> idx;

	// all keys/values are by preorder relabelling
	std::vector<int> preorder;
	std::vector<int> depth;
	std::vector<int> par;
	std::vector<int> sz;
	std::vector<int> heavy_par;
	std::vector<int> heavy_dist;

	std::vector<int> depth_val_to_idx;
	RangeMinQuery<int> depth_val_rmq;

	static_forest_t() : N(0) {}
	static_forest_t(const std::vector<std::vector<int>>& adj, const std::vector<int>& roots = {}) :
		N(int(adj.size())),
		idx(N, -1),
		preorder(N, -1),
		depth(N, -1),
		par(N, -1),
		sz(N, -1),
		heavy_par(N, -1),
		heavy_dist(N, -1),
		depth_val_to_idx(N, -1)
	{
		{
			int nxt_idx = 0;
			std::vector<int> depth_freq(N, 0);
			std::vector<int> depth_val(N, -1);
			std::vector<int> heavy_child(N, -1);
			auto build_one_tree = [&](int rt) -> void {
				std::y_combinator([&](auto self, int cur, int prv) -> int {
					int sz = 1;
					int cur_heavy = -1;
					int cur_heavy_weight = 0;
					for (int nxt : adj[cur]) {
						if (nxt == prv) continue;
						int n_sz = self(nxt, cur);
						if (n_sz > cur_heavy_weight) {
							cur_heavy = nxt;
							cur_heavy_weight = n_sz;
						}
						sz += n_sz;
					}
					heavy_child[cur] = cur_heavy;
					return sz;
				})(rt, -1);
				assert(idx[rt] == -1);
				std::y_combinator([&](auto&& self, int cur, int prv, int par_idx, int d, bool is_heavy_root) -> void {
					int cur_idx = idx[cur] = nxt_idx++;
					preorder[cur_idx] = cur;
					par[cur_idx] = par_idx;
					depth[cur_idx] = d;
					depth_val[cur_idx] = ++depth_freq[d];
					assert(is_heavy_root == (par_idx == -1 || cur_idx != par_idx + 1));
					if (is_heavy_root) {
						heavy_par[cur_idx] = par_idx;
						heavy_dist[cur_idx] = 1;
					} else {
						assert(par_idx == cur_idx - 1);
						heavy_par[cur_idx] = heavy_par[cur_idx - 1];
						heavy_dist[cur_idx] = heavy_dist[cur_idx - 1] + 1;
					}
					if (heavy_child[cur] != -1) {
						int nxt = heavy_child[cur];
						self(nxt, cur, cur_idx, d+1, false);
					}
					for (int nxt : adj[cur]) {
						if (nxt == prv) continue;
						if (nxt == heavy_child[cur]) continue;
						self(nxt, cur, cur_idx, d+1, true);
					}
					sz[cur_idx] = nxt_idx - cur_idx;
				})(rt, -1, -1, 0, true);
			};
			if (!roots.empty()) {
				for (int r : roots) build_one_tree(r);
			} else {
				for (int rt = 0; rt < N; rt++) {
					if (idx[rt] == -1) {
						build_one_tree(rt);
					}
				}
			}
			for (int i = 0; i < N; i++) {
				assert(idx[i] != -1);
			}
			for (int i = 1; i < N; i++) {
				depth_freq[i] += depth_freq[i-1];
			}
			for (int i = 0; i < N; i++) {
				depth_val[i] = depth_freq[depth[i]] - depth_val[i];
				assert(depth_val_to_idx[depth_val[i]] == -1);
				depth_val_to_idx[depth_val[i]] = i;
			}
			depth_val_rmq = RangeMinQuery<int>(depth_val);
		}
	}

	int dist(int a, int b) const {
		if (a == b) return 0;
		a = idx[a], b = idx[b];
		if (a > b) std::swap(a, b);
		int o = depth_val_to_idx[depth_val_rmq.query(a+1, b)];
		return depth[a] + depth[b] - 2 * (depth[o] - 1);
	}

	int lca(int a, int b) const {
		if (a == b) return a;
		a = idx[a], b = idx[b];
		if (a > b) std::swap(a, b);
		int o = depth_val_to_idx[depth_val_rmq.query(a+1, b)];
		return preorder[par[o]];
	}

	// query which subtree of a contains b; b must be inside a
	// returns -1 if a == b
	int get_subtree(int a, int b) const {
		if (a == b) return -1;
		a = idx[a], b = idx[b];
		assert(a < b && b < a + sz[a]);
		return preorder[depth_val_to_idx[depth_val_rmq.query(a+1, b)]];
	}

	// next from a to b, a and b must be in the same tree
	int get_next(int a, int b) const {
		if (a == b) return -1;
		a = idx[a], b = idx[b];
		if (a < b && b < a + sz[a]) {
			return preorder[depth_val_to_idx[depth_val_rmq.query(a+1, b)]];
		} else {
			return preorder[par[a]];
		}
	}

	int get_ancestor(int a, int k) const {
		assert(k >= 0);
		a = idx[a];
		if (k > depth[a]) return -1;
		while (a != -1 && k > 0) {
			if (k >= heavy_dist[a]) {
				k -= heavy_dist[a];
				assert(heavy_par[a] <= a - heavy_dist[a]);
				a = heavy_par[a];
			} else {
				a -= k;
				k = 0;
			}
		}
		return preorder[a];
	}
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int Q; cin >> Q;
	const int VA = int(1e5);
	const int VB = int(1e5);
	const int V = VA + VB;
	std::vector<std::array<int, 2>> queries(Q);
	for (auto& [a, b] : queries) {
		cin >> a >> b; a--, b--; b += VA;
	}

	std::vector<std::vector<int>> adj(V);
	{
		std::vector<int> par(V, -1);
		auto get_par = [&](int a) -> int {
			while (par[a] >= 0) {
				if (par[par[a]] >= 0) par[a] = par[par[a]];
				a = par[a];
			}
			return a;
		};
		auto merge = [&](int a, int b) -> bool {
			a = get_par(a), b = get_par(b);
			if (a == b) return false;
			if (par[a] > par[b]) std::swap(a, b);
			par[a] += par[b];
			par[b] = a;
			return true;
		};
		for (auto [a, b] : queries) {
			if (merge(a, b)) {
				adj[a].push_back(b);
				adj[b].push_back(a);
			}
		}
	}

	static_forest_t la(adj);

	std::vector<int> par(V, -1);
	auto get_par = [&](int a) -> int {
		while (par[a] >= 0) {
			if (par[par[a]] >= 0) par[a] = par[par[a]];
			a = par[a];
		}
		return a;
	};
	auto merge = [&](int a, int b) -> bool {
		a = get_par(a), b = get_par(b);
		if (a == b) return false;
		if (par[a] > par[b]) std::swap(a, b);
		par[a] += par[b];
		par[b] = a;
		return true;
	};

	auto get_dist = [&](int a, int b) -> std::pair<int, std::array<int, 2>> {
		int d = la.dist(a, b) + (a >= VA) + (b >= VA);
		assert(d % 2 == 0);
		return {d, {a, b}};
	};

	std::array<int, 2> tot_ans{0, 0};
	struct comp_data_t {
		std::array<int, 2> cur_ans;
		std::array<int, 2> alive_freq;
		int bad_vert = -1;
		std::pair<int, std::array<int, 2>> diam;
	};
	std::vector<comp_data_t> dat(V);
	auto update_tot_ans = [&](int v) -> void {
		auto& d = dat[get_par(v)];
		assert((d.cur_ans == std::array<int, 2>{0, 0}));
		d.cur_ans = d.alive_freq;
		if (d.bad_vert == -1) {
			assert(d.diam.first % 2 == 0);
			d.cur_ans[(d.diam.first / 2) & 1] --;
		}
		tot_ans[0] += d.cur_ans[0];
		tot_ans[1] += d.cur_ans[1];
	};
	auto remove_tot_ans = [&](int v) -> void {
		auto& d = dat[get_par(v)];
		tot_ans[0] -= d.cur_ans[0];
		tot_ans[1] -= d.cur_ans[1];
		d.cur_ans = {0, 0};
	};

	for (int v = 0; v < V; v++) {
		dat[v].cur_ans = {0, 0};
		dat[v].alive_freq = {0, 0};
		dat[v].alive_freq[v >= VA]++;
		dat[v].diam = get_dist(v, v);
		dat[v].bad_vert = -1;
		update_tot_ans(v);
		assert(dat[v].cur_ans[0] == 0);
		assert(dat[v].cur_ans[1] == 0);
	}

	auto get_next = [&](int a, int r) -> int {
		assert(a != r);
		int l = la.lca(a, r);
		int res;
		if (l == a) {
			res = la.get_ancestor(r, la.depth[r] - la.depth[a] - 1);
		} else {
			res = la.get_ancestor(a, 1);
		}
		assert(res == la.get_next(a, r));
	};

	std::vector<bool> is_alive(V, true);

	auto mark_bad = [&](comp_data_t& d, int a) -> void {
		assert(d.bad_vert != -1);
		while (true) {
			if (!is_alive[a]) break;
			is_alive[a] = false;
			d.alive_freq[a >= VA] --;
			if (a == d.bad_vert) break;
			a = get_next(a, d.bad_vert);
		}
	};

	for (auto [a, b] : queries) {
		int pa = get_par(a);
		int pb = get_par(b);
		if (pa == pb) {
			remove_tot_ans(pa);
			auto& d = dat[pa];
			if (d.bad_vert == -1) {
				d.bad_vert = a;
			}
			mark_bad(d, a);
			mark_bad(d, b);
			update_tot_ans(pa);
		} else {
			remove_tot_ans(pa);
			remove_tot_ans(pb);
			auto& da = dat[pa];
			auto& db = dat[pb];
			if (da.bad_vert != -1 && db.bad_vert != -1) {
				mark_bad(da, a);
				mark_bad(db, b);
			}
			comp_data_t nd;
			nd.cur_ans = {0, 0};
			nd.alive_freq[0] = da.alive_freq[0] + db.alive_freq[0];
			nd.alive_freq[1] = da.alive_freq[1] + db.alive_freq[1];
			nd.bad_vert = (da.bad_vert != -1 ? da.bad_vert : db.bad_vert);
			nd.diam = std::max({
				get_dist(da.diam.second[0], db.diam.second[0]),
				get_dist(da.diam.second[0], db.diam.second[1]),
				get_dist(da.diam.second[1], db.diam.second[0]),
				get_dist(da.diam.second[1], db.diam.second[1]),
				da.diam, db.diam
			});
			merge(a, b);
			dat[get_par(pa)] = nd;
			update_tot_ans(pa);
		}
		cout << tot_ans[0] << ' ' << tot_ans[1] << '\n';
	}

	return 0;
}
