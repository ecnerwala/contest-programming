#include <bits/stdc++.h>
#include <cassert>
#include <ext/pb_ds/assoc_container.hpp>

namespace wala {

struct splitmix64_hash {
	static uint64_t splitmix64(uint64_t x) {
		// http://xorshift.di.unimi.it/splitmix64.c
		x += 0x9e3779b97f4a7c15;
		x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
		x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
		return x ^ (x >> 31);
	}

	size_t operator()(uint64_t x) const {
		static const uint64_t FIXED_RANDOM = std::chrono::steady_clock::now().time_since_epoch().count();
		return splitmix64(x + FIXED_RANDOM);
	}
};

template <typename K, typename V, typename Hash = splitmix64_hash>
using hash_map = __gnu_pbds::gp_hash_table<K, V, Hash>;

template <typename K, typename Hash = splitmix64_hash>
using hash_set = hash_map<K, __gnu_pbds::null_type, Hash>;

} // namespace wala

namespace wala {

template <typename F> struct reverse_comparator_t {
	F f;
	template <typename Arg1, typename Arg2> constexpr bool operator() (Arg1&& arg1, Arg2&& arg2) & {
		return f(std::forward<Arg2>(arg2), std::forward<Arg1>(arg1));
	}
	template <typename Arg1, typename Arg2> constexpr bool operator() (Arg1&& arg1, Arg2&& arg2) const& {
		return f(std::forward<Arg2>(arg2), std::forward<Arg1>(arg1));
	}
	template <typename Arg1, typename Arg2> constexpr bool operator() (Arg1&& arg1, Arg2&& arg2) && {
		return std::move(f)(std::forward<Arg2>(arg2), std::forward<Arg1>(arg1));
	}
	template <typename Arg1, typename Arg2> constexpr bool operator() (Arg1&& arg1, Arg2&& arg2) const&& {
		return std::move(f)(std::forward<Arg2>(arg2), std::forward<Arg1>(arg1));
	}
};

template <typename F> constexpr reverse_comparator_t<std::decay_t<F>> reverse_comparator(F&& f) {
	return { std::forward<F>(f) };
}

} // namespace wala

namespace wala {

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

} // namespace wala

namespace wala {

class CartesianTree {
public:
	struct Node {
		int l, m, r; // inclusive ranges
		std::array<int, 2> c;
		int p;
	};
	std::vector<Node> nodes;
	int root = -1;

	CartesianTree() {}

	Node& operator [] (int idx) { return nodes[idx]; }
	const Node& operator [] (int idx) const { return nodes[idx]; }

	int size() const { return int(nodes.size()); }

private:
	CartesianTree(std::vector<Node>&& nodes_, int root_) : nodes(std::move(nodes_)), root(root_) {}

public:
	// min-cartesian-tree, with earlier cells tiebroken earlier
	template <typename T, typename Comp = std::less<T>>
	static CartesianTree build_min_tree(const std::vector<T>& v, Comp comp = Comp()) {
		std::vector<Node> nodes(v.size()*2+1);
		std::vector<int> stk; stk.reserve(v.size());
		int root = -1;
		for (int i = 0; i <= int(v.size()); i++) {
			int cur = 2*i;
			nodes[cur].l = i;
			nodes[cur].r = i-1;
			nodes[cur].m = i-1;
			nodes[cur].c = {-1, -1};
			while (!stk.empty() && (i == int(v.size()) || comp(v[i], v[nodes[stk.back()].m]))) {
				int nxt = stk.back(); stk.pop_back();
				nodes[cur].p = nxt;
				nodes[nxt].c[1] = cur;
				nodes[nxt].r = nodes[cur].r;
				cur = nxt;
			}
			if (i == int(v.size())) {
				root = cur;
				break;
			}
			nodes[2*i+1].l = nodes[cur].l;
			nodes[2*i+1].m = i;
			nodes[cur].p = 2*i+1;
			nodes[2*i+1].c[0] = cur;
			stk.push_back(2*i+1);
		}
		nodes[root].p = -1;
		return {std::move(nodes), root};
	}

	// max-cartesian-tree, with earlier cells tiebroken earlier
	template <typename T, typename Comp = std::less<T>>
	static CartesianTree build_max_tree(const std::vector<T>& v, Comp comp = Comp()) {
		return build_min_tree(v, reverse_comparator(comp));
	}
};

} // namespace wala

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int T; std::cin >> T;
	while (T--) {
		int N; std::cin >> N;
		std::vector<int> A(N); for (auto& a : A) std::cin >> a;

		auto rmq = wala::RangeMaxQuery<int>(A);

		auto is_good = [&](int v) -> bool {
			assert(v);
			std::vector<int> inds(N+1);
			std::vector<int> cnts; cnts.reserve(N+2);
			{
				std::vector<std::pair<int, int>> pref(N+1);
				wala::hash_map<int, int> mp;
				for (int i = 0; i <= N; i++) {
					if (i < N) pref[i+1].first = pref[i].first ^ (A[i] & v);
					pref[i].second = 2 * i;
					if (pref[i].first > (pref[i].first ^ v)) {
						pref[i].first ^= v;
						pref[i].second++;
					}
					auto [a, did_insert] = mp.insert({pref[i].first, int(cnts.size())});
					if (did_insert) cnts.push_back(0);

					pref[i].first = a->second;

					cnts[pref[i].first]++;
				}
				{
					int loc = 0;
					for (int i = 0; i < int(cnts.size()); i++) {
						loc += std::exchange(cnts[i], loc);
					}
					assert(loc == N+1);
				}
				for (int i = 0; i <= N; i++) {
					inds[cnts[pref[i].first]++] = pref[i].second;
				}
				cnts.insert(cnts.begin(), 0);
			}

			std::vector<int> vals; vals.reserve(N);
			bool good = false;

			using dp_t = std::array<std::array<int, 2>, 2>;
			auto build_dp = [&](int idx) -> dp_t {
				std::array<std::array<int, 2>, 2> res{std::array<int, 2>{-1, -1}, {-1, -1}};
				res[idx & 1] = {idx >> 1, idx >> 1};
				return res;
			};
			auto merge_dp = [&](dp_t v0, dp_t v1, int mx) -> dp_t {
				if ((mx & v) == v) {
					if (v0[0][0] != -1 && v1[1][1] != -1 && v1[1][1] - v0[0][0] > 1) {
						good = true;
					}
					if (v0[1][0] != -1 && v1[0][1] != -1 && v1[0][1] - v0[1][0] > 1) {
						good = true;
					}
				}
				std::array<std::array<int, 2>, 2> res{std::array<int, 2>{-1, -1}, {-1, -1}};
				for (int z = 0; z < 2; z++) {
					if (v0[z][0] == -1) res[z] = v1[z];
					else if (v1[z][0] == -1) res[z] = v0[z];
					else res[z] = {v0[z][0], v1[z][1]};
				}
				return res;
			};
			for (int rng = 0; rng+1 < int(cnts.size()); rng++) {
				int l = cnts[rng], r = cnts[rng+1];
				assert(r > l);
				int S = r - l - 1;
				vals.resize(S);
				for (int i = 0; i < S; i++) vals[i] = rmq.query(inds[l+i] >> 1, (inds[l+i+1] >> 1) - 1);
				auto ct = wala::CartesianTree::build_max_tree(vals);

				[&](this auto&& self, int cur) -> std::array<std::array<int, 2>, 2> {
					if (!(cur & 1)) {
						return build_dp(inds[l + (cur >> 1)]);
					} else {
						auto v0 = self(ct[cur].c[0]);
						auto v1 = self(ct[cur].c[1]);
						return merge_dp(v0, v1, vals[cur >> 1]);
					}
				}(ct.root);
			}
			return good;
		};

		int ans = 0;
		for (int b = 17; b >= 0; b--) {
			if (is_good(ans + (1 << b))) ans += (1 << b);
		}
		std::cout << ans << '\n';
	}

	return 0;
}
