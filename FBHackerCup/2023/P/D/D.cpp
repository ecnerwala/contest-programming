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

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		int N, M; cin >> N >> M;
		std::vector<std::vector<std::pair<int, int>>> adj(N);
		for (int e = 0; e < M; e++) {
			int u, v; cin >> u >> v; u--, v--;
			adj[u].push_back({v, e});
			adj[v].push_back({u, e});
		}

		const int INF = N+1;
		std::vector<int> depth(N, -1);
		std::vector<int> lowval(N, -1);
		std::vector<int> odd_dist(N, INF);
		depth[0] = 1;
		std::vector<std::vector<std::pair<int, int>>> tree_ch(N);
		std::y_combinator([&](auto self, int cur, int prv_e) -> int {
			lowval[cur] = depth[cur];
			tree_ch[cur].reserve(adj[cur].size());
			int tot_sz = 1;
			int max_ch_sz = 0;
			int max_ch_idx = -1;
			for (auto [nxt, e] : adj[cur]) {
				if (e == prv_e) continue;
				if (depth[nxt] == -1) {
					depth[nxt] = depth[cur] + 1;
					int ch_sz = self(nxt, e);
					tot_sz += ch_sz;
					if (ch_sz > max_ch_sz) {
						max_ch_sz = ch_sz;
						max_ch_idx = int(tree_ch[cur].size());
					}

					lowval[cur] = std::min(lowval[cur], lowval[nxt]);
					int edge_len = (lowval[nxt] == depth[nxt] ? 1 : 0);

					tree_ch[cur].push_back({nxt, edge_len});
					odd_dist[cur] = std::min(odd_dist[cur], odd_dist[nxt] + edge_len);
				} else if (depth[nxt] < depth[cur]) {
					lowval[cur] = std::min(lowval[cur], depth[nxt]);
					if ((depth[nxt] & 1) == (depth[cur] & 1)) {
						odd_dist[cur] = 0;
					}
				} else {
					// back-edge, do nothing
				}
			}
			if (max_ch_idx > 0) {
				std::swap(tree_ch[cur][0], tree_ch[cur][max_ch_idx]);
			}
			return tot_sz;
		})(0, -1);

		struct dat_t {
			int heavy_par;
			int heavy_root;
			int heavy_val;
		};
		std::vector<dat_t> dat; dat.reserve(N);
		std::vector<int> idx(N);
		std::vector<int> vals; vals.reserve(N);
		std::y_combinator([&](auto self, int cur, int heavy_par, int heavy_root, int v) -> void {
			int cur_idx = idx[cur] = int(dat.size());
			int cur_dist = odd_dist[cur];
			if (heavy_root == -1) heavy_root = cur_idx;
			v = std::min(v, cur_dist);
			dat.push_back({heavy_par, heavy_root, v});
			vals.push_back(cur_dist);
			//cerr << "cur " << cur << " idx " << cur_idx << " val " << odd_dist[cur] << " heavy_par " << heavy_par << " path val " << v << '\n';
			for (auto [nxt, l] : tree_ch[cur]) {
				//cerr << "edge " << cur << "-" << nxt << ' ' << l << '\n';
				odd_dist[nxt] = std::min(odd_dist[nxt], cur_dist + l);
				self(nxt, heavy_par, heavy_root, v);
				heavy_par = cur_idx;
				heavy_root = -1;
				v = INF;
			}
		})(0, -1, -1, INF);
		assert(int(dat.size()) == N);
		assert(int(vals.size()) == N);

		RangeMinQuery<int> rmq(vals);

		auto query = [&](int a, int b) -> int {
			//cerr << "query " << a << ' ' << b << '\n';
			a = idx[a];
			b = idx[b];
			int ans = INF;
			while (true) {
				if (a > b) std::swap(a, b);
				if (dat[a].heavy_root != dat[b].heavy_root) {
					ans = std::min(ans, dat[b].heavy_val);
					b = dat[b].heavy_par;
				} else {
					//cerr << " root rmq " << a << ' ' << b << '\n';
					assert(a <= b);
					ans = std::min(ans, rmq.query(a, b));
					break;
				}
			}
			//cerr << ans << '\n';
			if (ans == INF) return -1;
			return ans;
		};

		int Q; cin >> Q;
		int64_t tot_ans = 0;
		for (int q = 0; q < Q; q++) {
			int u, v; cin >> u >> v; u--, v--;
			int ans = query(u, v);
			tot_ans += ans;
		}

		cout << "Case #" << case_num << ": " << tot_ans << '\n';
	}

	return 0;
}
