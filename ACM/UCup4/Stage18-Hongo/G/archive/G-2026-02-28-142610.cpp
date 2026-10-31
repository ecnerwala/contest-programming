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

	int N, Q; std::cin >> N >> Q;
	std::string S; std::cin >> S;
	assert(int(S.size()) == N);
	std::vector<int> pref(N+1);
	for (int i = 0; i < N; i++) {
		assert(S[i] == '(' || S[i] == ')');
		pref[i+1] = pref[i] + (S[i] == '(' ? +1 : -1);
	}
	std::vector<std::pair<int, int>> pts(N+1);
	for (int i = 0; i <= N; i++) {
		pts[i] = {pref[i], i};
	}
	std::sort(pts.begin(), pts.end());

	RangeMinQuery<int> rmq(pref);

	for (int q = 0; q < Q; q++) {
		int l, r; std::cin >> l >> r; l--;
		if (pref[l] == pref[r]) {
			std::cout << -1 << '\n';
		} else if (pref[l] < pref[r]) {
			int mi = rmq.query(l, r);
			auto it = std::lower_bound(pts.begin(), pts.end(), std::pair<int, int>{mi, r+1});
			assert(it != pts.begin());
			--it;
			assert(it->first == mi && l <= it->second && it->second <= r);
			auto jt = std::lower_bound(pts.begin(), pts.end(), std::pair<int, int>{mi + std::abs(pref[r] - pref[l]), r+1});
			assert(jt != pts.begin());
			--jt;
			assert(jt->first == mi + std::abs(pref[r] - pref[l]) && l <= jt->second && jt->second <= r);
			std::cout << (it->second - l) + (r - jt->second - 1) << '\n';
		} else if (pref[l] > pref[r]) {
			int mi = rmq.query(l, r);
			auto it = std::lower_bound(pts.begin(), pts.end(), std::pair<int, int>{mi, l});
			assert(it != pts.end());
			assert(it->first == mi && l <= it->second && it->second <= r);
			auto jt = std::lower_bound(pts.begin(), pts.end(), std::pair<int, int>{mi + std::abs(pref[r] - pref[l]), l});
			assert(jt != pts.end());
			assert(jt->first == mi + std::abs(pref[r] - pref[l]) && l+1 <= jt->second && jt->second <= r);
			std::cout << (r - it->second) + (jt->second - l - 1) << '\n';
		} else assert(false);
	}

	return 0;
}
