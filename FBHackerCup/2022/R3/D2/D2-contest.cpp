#include<bits/stdc++.h>

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
	for (int case_num = 1; case_num <= T; case_num ++) {
		cerr << "Case #" << case_num << '\n';

		int N, Q; cin >> N >> Q;

		std::vector<std::pair<int, int>> nxt(N, {-1, -1});
		std::vector<std::pair<int, int>> color_nodes(N);
		for (int i = 0; i < N; i++) {
			color_nodes[i] = {i, i};
		}

		for (int q = 1; q <= Q; q++) {
			int a, b; cin >> a >> b; a--, b--;
			if (color_nodes[a].first == -1) continue;
			if (color_nodes[b].first == -1) {
				color_nodes[b] = color_nodes[a];
			} else {
				nxt[color_nodes[b].second] = {color_nodes[a].first, q};
				color_nodes[b].second = color_nodes[a].second;
			}
			color_nodes[a] = {-1, -1};
		}

		const int INF = 1e9;
		assert(INF > Q);
		std::pair<int, int> lst{-1, -1};
		for (int c = 0; c < N; c++) {
			if (color_nodes[c].first == -1) continue;
			if (lst.first == -1) {
				lst = color_nodes[c];
			} else {
				nxt[std::exchange(lst.second, color_nodes[c].second)] = {color_nodes[c].first, INF};
			}
		}

		std::vector<int> node_idx(N, -1);
		std::vector<int> deltas; deltas.reserve(N);
		{
			int cur = lst.first;
			for (int i = 0; i < N; i++) {
				assert(cur != -1);
				assert(node_idx[cur] == -1);
				node_idx[cur] = i;
				deltas.push_back(nxt[cur].second);
				cur = nxt[cur].first;
			}
			assert(cur == -1);
		}
		RangeMaxQuery<int> rmq(deltas);
		std::vector<std::pair<int, int>> times; times.reserve(N-1);
		for (int i = 1; i < N; i++) {
			int x = node_idx[i-1], y = node_idx[i];
			assert(x != y);
			if (x > y) std::swap(x, y);
			assert(x < y);
			int t = rmq.query(x, y-1);
			times.push_back({t, i});
		}
		assert(int(times.size()) == N-1);
		std::sort(times.begin(), times.end());
		for (int i = N-3; i >= 0; i--) {
			times[i].second = std::gcd(times[i].second, times[i+1].second);
		}
		int64_t tot_ans = 0;
		for (int K = 1; K <= N; K++) {
			int mi = -1, ma = N-1;
			while (ma - mi > 1) {
				int md = (mi + ma) / 2;
				if (times[md].second % K == 0) ma = md;
				else mi = md;
			}
			int ans;
			if (mi == -1) {
				assert(K == 1);
				ans = 0;
			} else {
				ans = times[mi].first;
			}
			if (ans == INF) ans = -1;
			else assert(0 <= ans && ans <= Q);
			tot_ans += ans;
		}

		cout << "Case #" << case_num << ": " << tot_ans << '\n';
	}

	return 0;
}
