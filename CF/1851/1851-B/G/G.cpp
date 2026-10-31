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

	int T; cin >> T;
	while (T--) {
		int N, M; cin >> N >> M;
		std::vector<int> H(N);
		for (auto& h : H) cin >> h;
		std::vector<std::vector<int>> adj(N);
		for (int e = 0; e < M; e++) {
			int u, v; cin >> u >> v; u--, v--;
			adj[u].push_back(v);
			adj[v].push_back(u);
		}

		std::vector<int> par(N, -1);
		auto follow_par = [&](int a) -> int {
			while (par[a] >= 0) {
				if (par[par[a]] >= 0) par[a] = par[par[a]];
				a = par[a];
			}
			return a;
		};
		std::vector<int> par_label(N);
		std::iota(par_label.begin(), par_label.end(), 0);
		auto get_par = [&](int a) -> int {
			return par_label[follow_par(a)];
		};
		// keep b's parent
		auto merge_into = [&](int a, int b) -> bool {
			a = follow_par(a);
			b = follow_par(b);
			if (a == b) return false;
			par_label[a] = par_label[b];
			if (par[a] > par[b]) std::swap(a, b);
			par[a] += par[b];
			par[b] = a;
			return true;
		};

		std::vector<std::pair<int, int>> heights(N);
		for (int i = 0; i < N; i++) {
			heights[i] = {H[i], i};
		}
		std::sort(heights.begin(), heights.end());

		std::vector<std::vector<int>> merge_ch(N);
		{
			std::vector<bool> alive(N);
			for (auto [_, i] : heights) {
				alive[i] = true;
				for (int j : adj[i]) {
					if (!alive[j]) continue;
					j = follow_par(j);
					if (par_label[j] == i) continue;
					merge_ch[i].push_back(par_label[j]);
					bool did_merge = merge_into(j, i);
					assert(did_merge);
				}
			}
		}

		std::vector<int> rmq_dat; rmq_dat.reserve(N);
		std::vector<int> idx(N, -1);
		int nxt_idx = 0;
		std::vector<int> root(N, -1);
		for (int i = 0; i < N; i++) {
			if (get_par(i) == i) {
				rmq_dat.push_back(-1);
				std::y_combinator([&](auto self, int cur) -> void {
					root[cur] = i;
					idx[cur] = nxt_idx++;
					for (int nxt : merge_ch[cur]) {
						assert(H[cur] >= H[nxt]);
						rmq_dat.push_back(H[cur]);
						self(nxt);
					}
				})(i);
			}
		}
		assert(nxt_idx == N);
		RangeMaxQuery<int> rmq(rmq_dat);
		auto get_lca = [&](int a, int b) -> int {
			if (a == b) return H[a];
			a = idx[a];
			b = idx[b];
			assert(a != b);
			if (a > b) std::swap(a, b);
			return rmq.query(a+1, b);
		};
		int Q; cin >> Q;
		for (int q = 0; q < Q; q++) {
			int a, b, e; cin >> a >> b >> e; a--, b--;
			cout << (root[a] == root[b] && get_lca(a, b) <= H[a] + e ? "YES" : "NO") << '\n';
		}
		if (T) cout << '\n';
	}

	return 0;
}
