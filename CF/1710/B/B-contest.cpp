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

template <typename T> using RangeMaxQuery = RangeMinQuery<T, std::greater<T>>;

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; int64_t M; cin >> N >> M;
		struct rain_t {
			int64_t x, v;
		};
		std::vector<rain_t> R(N); for (auto& r : R) cin >> r.x >> r.v;

		std::vector<std::pair<int64_t, int64_t>> coords; coords.reserve(3*N);
		for (auto r : R) {
			coords.push_back({r.x - r.v, +1});
			coords.push_back({r.x, -2});
			coords.push_back({r.x + r.v, +1});
		}
		std::sort(coords.begin(), coords.end());
		{
			int L = 0;
			for (int i = 0; i < int(coords.size()); i++) {
				if (L > 0 && coords[i].first == coords[L-1].first) coords[L-1].second += coords[i].second;
				else coords[L++] = coords[i];
			}
			coords.resize(L);
		}
		int L = int(coords.size());

		{
			int64_t cur_pos = 0;
			int64_t cur_val = 0;
			int64_t cur_slope = 0;
			for (int i = 0; i < L; i++) {
				cur_val += (coords[i].first - cur_pos) * cur_slope;
				cur_pos = coords[i].first;
				cur_slope += coords[i].second;
				coords[i].second = cur_val;
				//cerr << coords[i].first << ' ' << coords[i].second << '\n';
			}
		}

		RangeMaxQuery<int64_t> flat;
		{
			std::vector<int64_t> flat_vec(L);
			for (int i = 0; i < L; i++) flat_vec[i] = coords[i].second;
			flat = RangeMaxQuery<int64_t>(flat_vec);
		}
		RangeMaxQuery<int64_t> incr;
		{
			std::vector<int64_t> incr_vec(L);
			for (int i = 0; i < L; i++) incr_vec[i] = coords[i].second - coords[i].first;
			incr = RangeMaxQuery<int64_t>(incr_vec);
		}
		RangeMaxQuery<int64_t> decr;
		{
			std::vector<int64_t> decr_vec(L);
			for (int i = 0; i < L; i++) decr_vec[i] = coords[i].second + coords[i].first;
			decr = RangeMaxQuery<int64_t>(decr_vec);
		}

		for (auto r : R) {
			int x = int(std::lower_bound(coords.begin(), coords.end(), std::pair<int64_t, int64_t>{r.x - r.v, -1}) - coords.begin());
			int y = int(std::lower_bound(coords.begin(), coords.end(), std::pair<int64_t, int64_t>{r.x, -1}) - coords.begin());
			int z = int(std::lower_bound(coords.begin(), coords.end(), std::pair<int64_t, int64_t>{r.x + r.v, -1}) - coords.begin());

			int64_t h0 = flat.query(0, x);
			int64_t h1 = incr.query(x, y) + (r.x - r.v);
			int64_t h2 = decr.query(y, z) - (r.x + r.v);
			int64_t h3 = flat.query(z, L - 1);
			//cerr << r.x << ' ' << r.v << ' ' << x << ' ' << y << ' ' << z << ' ' << h0 << ' ' << h1 << ' ' << h2 << ' ' << h3 << '\n';

			int64_t h = std::max({h0, h1, h2, h3});
			if (h > M) {
				cout << '0';
			} else {
				cout << '1';
			}
		}
		cout << '\n';
	}

	return 0;
}
