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

	std::string S; cin >> S;

	int N = int(S.size()) + 1;
	std::vector<int> H(N);
	H[0] = 0;
	for (int i = 0; i < int(S.size()); i++) {
		assert(S[i] == 'A' || S[i] == 'B');
		H[i+1] = H[i] + (S[i] == 'B' ? -1 : 1);
	}
	RangeMinQuery<int> H_min(H);

	std::array<std::vector<std::array<int, 2>>, 2> nxt; 
	// 0 is going right, 1 is going left
	for (int z = 0; z < 2; z++) {
		if (!z) {
			std::reverse(H.begin(), H.end());
		}
		nxt[z].resize(N);
		std::vector<std::pair<int, int>> stk; stk.reserve(N);
		for (int i = 0; i < N; i++) {
			while (stk.size() >= 2 && stk.end()[-2].first <= H[i]) { stk.pop_back(); stk.pop_back(); }
			nxt[z][i][1] = stk.empty() ? -1 : stk.back().second;
			stk.push_back({H[i], i});
			while (stk.size() >= 2 && stk.end()[-2].first >= H[i]) { stk.pop_back(); stk.pop_back(); }
			assert(!stk.empty());
			nxt[z][i][0] = stk.back().second;
			stk.push_back({H[i], i});
		}
		if (!z) {
			std::reverse(H.begin(), H.end());
			std::reverse(nxt[z].begin(), nxt[z].end());
			for (auto& vs : nxt[z]) for (auto& v : vs) v = v == -1 ? v : N-1-v;
		}
	}

	int Q; cin >> Q;
	for (int q = 0; q < Q; q++) {
		int l, r; cin >> l >> r; l--;
		assert(l < r);
		int v_min = H_min.query(l, r);
		auto get_vals = [&](int st, int z) -> std::vector<int> {
			std::vector<int> heights;
			for (int cur = st, dir = 0; true; cur = nxt[z][cur][dir], dir = !dir) {
				if (cur == -1 || H[cur] <= v_min) {
					assert(dir);
					break;
				}
				heights.push_back(H[cur]);
			}
			return heights;
		};
		auto lvals = get_vals(l, 0);
		auto rvals = get_vals(r, 1);

		int sum = H[l] + H[r];
		int ans = -1;
		for (int z = 0; z < 2; z++, std::swap(lvals, rvals)) {
			int cnd = sum - v_min - v_min;
			auto va = lvals;
			auto vb = rvals;
			int cur = v_min;
			bool good = true;
			while (true) {
				if (va.empty()) {
					break;
				}
				if (va.back() + cur < sum) {
					good = false;
					break;
				}
				while (va.size() >= 4 && va.end()[-3] + cur >= sum) {
					va.pop_back();
					va.pop_back();
				}
				va.pop_back();
				cnd += 2 * (sum - cur - va.back());
				cur = va.back();
				va.pop_back();
				std::swap(va, vb);
			}
			if (!good) continue;
			if (ans == -1 || cnd < ans) ans = cnd;
		}
		cout << ans << '\n';
	}

	return 0;
}
