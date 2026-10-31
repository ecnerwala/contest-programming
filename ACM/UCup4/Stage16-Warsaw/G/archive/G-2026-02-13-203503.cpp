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

// these intervals are open
struct interval_t {
	int lo, hi;
	friend interval_t operator & (interval_t a, interval_t b) { return {std::max(a.lo, b.lo), std::min(a.hi, b.hi) }; }
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int L_tot, Q; cin >> L_tot >> Q;
	std::vector<std::pair<int, int>> pts; pts.reserve(L_tot);
	for (int i = 0; i < L_tot; i++) {
		int a; cin >> a;
		if (a != -1) pts.push_back({i, a-1});
	}

	int P = int(pts.size());
	std::vector<int> run_id(P);
	using group_t = std::array<std::pair<int, int>, 2>;
	std::vector<group_t> runs; runs.reserve(P);
	for (int i = 0; i < P; i++) {
		if (i == 0 || pts[i].first + pts[i].second != pts[i-1].first + pts[i-1].second) {
			runs.push_back({pts[i], pts[i]});
		} else {
			runs.back()[1] = pts[i];
		}
		run_id[i] = int(runs.size()) - 1;
	}

	int R = int(runs.size());

	auto get_interval = [&](group_t a, group_t b) -> interval_t {
		assert(a[1].first < b[0].first);
		if (a[0].second >= b[1].second) return {0, 0};
		return {a[1].first - b[1].second, b[0].first - a[0].second};
	};

	std::vector<int> los; los.reserve(R);
	std::vector<int> his; his.reserve(R);
	for (int i = 0; i+1 < R; i++) {
		auto [lo, hi] = get_interval(runs[i], runs[i+1]);
		los.push_back(lo);
		his.push_back(hi);
	}
	RangeMaxQuery<int> q_los(los);
	RangeMinQuery<int> q_his(his);

	// inclusive everything
	auto solve = [&](int l, int r) -> bool {
		int p_st = int(std::lower_bound(pts.begin(), pts.end(), std::pair<int, int>{l, -1}) - pts.begin());
		int p_en = int(std::lower_bound(pts.begin(), pts.end(), std::pair<int, int>{r+1, -1}) - pts.begin()) - 1;
		// no constraints
		if (p_st > p_en) return true;

		assert(p_st <= p_en);
		int r_st = run_id[p_st];
		int r_en = run_id[p_en];
		assert(r_st <= r_en);
		interval_t valid{l-1, l+1};
		group_t g_st; g_st[0] = g_st[1] = {l-1, -1};
		group_t g_en; g_en[0] = g_en[1] = {r+1, r-l+1};
		if (r_st == r_en) {
			// all 1 big run
			group_t mid{pts[p_st], pts[p_en]};
			valid = valid & get_interval(g_st, mid);
			valid = valid & get_interval(mid, g_en);
			return valid.lo < l && l < valid.hi;
		}
		assert(r_st < r_en);
		{
			group_t ng_st = {pts[p_st], runs[r_st][1]};
			valid = valid & get_interval(g_st, ng_st);
			g_st = ng_st;
		}
		{
			group_t ng_en = {runs[r_en][0], pts[p_en]};
			valid = valid & get_interval(ng_en, g_en);
			g_en = ng_en;
		}
		if (r_en - r_st == 1) {
			valid = valid & get_interval(g_st, g_en);
			return valid.lo < l && l < valid.hi;
		}
		assert(r_en - r_st >= 2);
		valid = valid & get_interval(g_st, runs[r_st+1]);
		valid = valid & get_interval(runs[r_en-1], g_en);
		if (r_en - r_st >= 3) {
			// get the intersection of r_st+1..r_en-1
			int lo = q_los.query(r_st + 1, r_en - 2);
			int hi = q_his.query(r_st + 1, r_en - 2);
			valid = valid & interval_t{lo, hi};
		}
		return valid.lo < l && l < valid.hi;
	};

	for (int q = 0; q < Q; q++) {
		int l, r; cin >> l >> r; l--, r--;

		bool good = solve(l, r);
		cout << (good ? "YES" : "NO") << '\n';
	}

	return 0;
}
