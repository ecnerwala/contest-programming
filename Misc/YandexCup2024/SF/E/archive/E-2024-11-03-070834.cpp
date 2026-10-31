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

	const int MAXV = int(1e7) + 10;
	std::vector<int> primes; primes.reserve(MAXV);
	std::vector<int> pfac(MAXV+1);
	for (int i = 2; i <= MAXV; i++) {
		if (!pfac[i]) {
			primes.push_back(i);
			pfac[i] = i;
		}
		for (int p : primes) {
			if (int64_t(i) * int64_t(p) > MAXV) break;
			pfac[i * p] = p;
			if (p == pfac[i]) break;
		}
	}
	struct diam_t {
		int len = -1;
		std::array<int, 2> coords{-1, -1};
	};
	std::vector<diam_t> diams(MAXV+1);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> A(N);
		for (auto& a : A) cin >> a;
		std::vector<std::vector<int>> adj(N);
		for (int e = 0; e < N-1; e++) {
			int u, v; cin >> u >> v; u--, v--;
			adj[u].push_back(v);
			adj[v].push_back(u);
		}

		std::vector<int> preorder_idx(N, -1);
		std::vector<int> heights; heights.reserve(N);
		std::vector<int> depths; depths.reserve(N);
		std::vector<int> vals; vals.reserve(N);
		{
			int nxt_idx = 0;
			std::y_combinator([&](auto&& self, int cur, int prv, int d) -> void {
				preorder_idx[cur] = nxt_idx++;
				depths.push_back(d);
				heights.push_back(d);
				vals.push_back(A[cur]);
				for (int nxt : adj[cur]) {
					if (nxt == prv) continue;
					self(nxt, cur, d+1);
					heights.back() = d;
				}
			})(0, -1, 0);
			assert(int(vals.size()) == N);
			assert(heights.back() == 0);
			heights.pop_back();
		}
		RangeMinQuery<int> rmq(heights);
		auto get_dist = [&](int u, int v) -> int {
			if (u == v) return 0;
			if (u > v) std::swap(u, v);
			return depths[u] + depths[v] - 2 * rmq.query(u, v-1);
		};
		auto ins = [&](diam_t& d, int i) -> void {
			if (d.len == -1) {
				d.len = 0;
				d.coords = {i, i};
			} else {
				int d0 = get_dist(d.coords[0], i);
				int d1 = get_dist(d.coords[1], i);
				if (std::max(d0, d1) > d.len) {
					d.len = std::max(d0, d1);
					d.coords = {(d0 > d1) ? d.coords[0] : d.coords[1], i};
				}
			}
		};
		for (int i = 0; i < N; i++) {
			for (int val = vals[i]; val > 1; ) {
				int p = pfac[val]; while (val % p == 0) { val /= p; }

				ins(diams[p], i);
			}
		}

		for (int z = 0; z < N; z++) {
			int i = preorder_idx[z];
			int ans = 0;
			for (int val = vals[i]; val > 1; ) {
				int p = pfac[val]; while (val % p == 0) { val /= p; }

				const auto& d = diams[p];
				ans = std::max({ans, get_dist(d.coords[0], i), get_dist(d.coords[1], i)});
			}
			cout << ans << '\n';
		}

		for (int i = 0; i < N; i++) {
			for (int val = vals[i]; val > 1; ) {
				int p = pfac[val]; while (val % p == 0) { val /= p; }

				diams[p] = diam_t{};
			}
		}
	}

	return 0;
}
