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

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		//cerr << "\n" << "start case" << '\n';
		int N, M; cin >> N >> M;
		std::vector<std::vector<int>> adj(N);
		for (int e = 0; e < N-1; e++) {
			int u, v; cin >> u >> v; u--, v--;
			adj[u].push_back(v);
			adj[v].push_back(u);
		}

		std::vector<int> depth(N, -1);
		depth[0] = 0;
		std::vector<int> par(N, -1);
		std::vector<int> st(N, -1);
		std::vector<std::pair<int, int>> et; et.reserve(N);
		std::y_combinator([&](auto self, int cur, int prv) -> void {
			par[cur] = prv;
			st[cur] = int(et.size());
			et.push_back({depth[cur], cur});
			for (int nxt : adj[cur]) {
				if (nxt == prv) continue;
				depth[nxt] = depth[cur] + 1;
				self(nxt, cur);
				et.back() = {depth[cur], cur};
			}
		})(0, -1);
		assert(int(et.size()) == N);
		RangeMinQuery<std::pair<int, int>> rmq(et);
		auto get_lca = [&](int x, int y) -> std::pair<int, int> {
			if (x == y) return {depth[x], x};
			int a = st[x], b = st[y];
			if (a > b) std::swap(a, b);
			return rmq.query(a, b-1);
		};

		std::vector<std::pair<int, int>> best(N);
		for (int i = 0; i < N; i++) {
			best[i] = {depth[i], i};
		}

		std::vector<std::array<int, 2>> routes(M);
		for (int m = 0; m < M; m++) {
			int x, y; cin >> x >> y; x--, y--;
			routes[m] = {x, y};
			if (x == y) continue;
			auto v = get_lca(x, y);
			best[x] = std::min(best[x], v);
			best[y] = std::min(best[y], v);
		}
		std::y_combinator([&](auto self, int cur, int prv) -> std::pair<int, int> {
			std::pair<int, int> v = best[cur];
			for (int nxt : adj[cur]) {
				if (nxt == prv) continue;
				v = std::min(v, self(nxt, cur));
			}
			return best[cur] = v;
		})(0, -1);

		auto worst_it = std::max_element(best.begin(), best.end());
		int sub_ch = int(worst_it - best.begin());
		int a = worst_it->second;
		int b = par[a];
		//cerr << "sub_ch = " << sub_ch << '\n';
		//cerr << "a = " << a << '\n';
		//cerr << "b = " << b << '\n';
		if (b == -1) {
			cout << "YES" << '\n';
			continue;
		}
		std::vector<bool> in_subtree(N);
		std::y_combinator([&](auto self, int cur, int prv) -> void {
			in_subtree[cur] = true;
			for (int nxt : adj[cur]) {
				if (nxt == prv) continue;
				self(nxt, cur);
			}
		})(a, b);

		std::vector<bool> is_good(N);
		for (auto [x, y] : routes) {
			if (in_subtree[x] != in_subtree[y]) {
				is_good[x] = true;
				is_good[y] = true;
			}
		}
		int sup_ch = -1;
		auto check_good = std::y_combinator([&](auto self, int cur, int prv) -> bool {
			bool g = is_good[cur];
			for (int nxt : adj[cur]) {
				if (nxt == prv) continue;
				if (self(nxt, cur)) {
					g = true;
				} else {
					return false;
				}
			}
			if (!g) {
				sup_ch = cur;
				return false;
			}
			return true;
		})(b, a);
		if (!check_good) {
			cout << "NO" << '\n';
			cout << sub_ch+1 << ' ' << sup_ch+1 << '\n';
		} else {
			cout << "YES" << '\n';
		}
	}

	return 0;
}
