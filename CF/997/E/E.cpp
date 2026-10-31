#include <bits/stdc++.h>

class PermTree {
public:
	enum class NodeType {
		LEAF,
		INCR,
		DECR,
		FULL,
		PARTIAL,
	};

	struct Node {
		std::array<int, 2> c;
		NodeType type;
		int l, r, lo, hi;
	};

	std::vector<Node> nodes;
	int root = -1;

	PermTree() {}
	Node& operator [] (int idx) { return nodes[idx]; }
	const Node& operator [] (int idx) const { return nodes[idx]; }

	int size() const { return int(nodes.size()); }

	PermTree(const std::vector<int>& A) : nodes(int(A.size())*2-1) {
		int N = int(A.size());
		std::vector<int> nxt_earlier(N);
		std::vector<int> prv_earlier(N);
		for (int i = 0; i < N; i++) {
			nxt_earlier[i] = i+1;
			prv_earlier[i] = i-1;
		}
		for (int i = N-1; i >= 0; i--) {
			int a = A[i];
			int p = prv_earlier[a];
			int n = nxt_earlier[a];
			if (p != -1) nxt_earlier[p] = n;
			if (n != N) prv_earlier[n] = p;
		}

		struct cnd_t {
			int left;
			int lo;
			int lo_gap;
			int hi;
			int hi_gap;
			int node;
		};

		std::vector<cnd_t> stk; stk.reserve(N);

		for (int i = 0; i < N; i++) {
			int a = A[i];
			while (true) {
				if (!stk.empty() && (a < stk.back().lo_gap || a > stk.back().hi_gap)) {
					assert(stk.size() >= 2);
					stk.end()[-2].lo = std::min(stk.end()[-2].lo, stk.back().lo);
					stk.end()[-2].hi = std::max(stk.end()[-2].hi, stk.back().hi);

					int n = 2 * stk.back().left - 1;
					nodes[n].c = {stk.end()[-2].node, stk.end()[-1].node};
					nodes[n].type = NodeType::PARTIAL;
					nodes[n].l = stk.end()[-2].left;
					nodes[n].r = i-1;
					nodes[n].lo = stk.end()[-2].lo;
					nodes[n].hi = stk.end()[-2].hi;

					stk.pop_back();

					stk.back().node = n;
				} else {
					break;
				}
			}

			stk.push_back({i, a, prv_earlier[a]+1, a, nxt_earlier[a]-1, 2*i});
			nodes[2*i].type = NodeType::LEAF;
			nodes[2*i].c = {-1, -1};
			nodes[2*i].l = nodes[2*i].r = i;
			nodes[2*i].lo = nodes[2*i].hi = a;

			while (stk.size() >= 2 && std::max(stk.back().hi, stk.end()[-2].hi) - std::min(stk.back().lo, stk.end()[-2].lo) == i - stk.end()[-2].left) {
				// merge these two nodes into one
				stk.end()[-2].lo = std::min(stk.end()[-2].lo, stk.back().lo);
				stk.end()[-2].hi = std::max(stk.end()[-2].hi, stk.back().hi);

				int n = 2 * stk.back().left - 1;
				nodes[n].c = {stk.end()[-2].node, stk.end()[-1].node};
				if (stk.end()[-2].lo == stk.end()[-1].lo) {
					nodes[n].type = NodeType::DECR;
				} else if (stk.end()[-2].hi == stk.end()[-1].hi) {
					nodes[n].type = NodeType::INCR;
				} else {
					nodes[n].type = NodeType::FULL;
				}
				nodes[n].l = stk.end()[-2].left;
				nodes[n].r = i;
				nodes[n].lo = stk.end()[-2].lo;
				nodes[n].hi = stk.end()[-2].hi;

				stk.pop_back();
				stk.back().node = n;
			}
		}

		assert(stk.size() == 1);
		root = stk.back().node;
	}
};

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

	const bool TESTING = false;
	int N; cin >> N;
	std::vector<int> A(N);
	if (!TESTING) {
		for (auto& a : A) { cin >> a; a--; }
	} else {
		int seed; cin >> seed;
		std::mt19937 mt(seed);
		std::iota(A.begin(), A.end(), 0);
		std::shuffle(A.begin(), A.end(), mt);
	}

	PermTree tree(A);
	int64_t tot_ways = 0;
	std::vector<int64_t> num_start(N);
	std::vector<int64_t> num_end(N);

	std::vector<int64_t> par_ways(2*N-1);
	std::vector<int> depth(2*N-1);
	std::vector<std::array<int, 2>> q_idx(2*N-1, {0, 0});

	auto c2 = [&](int a) -> int64_t {
		assert(a >= 0);
		return int64_t(a) * int64_t(a-1) / 2;
	};

	std::y_combinator([&](auto self, int cur, int cur_depth, int64_t cur_ways) -> void {
		auto& n = tree[cur];
		if (n.type == PermTree::NodeType::INCR || n.type == PermTree::NodeType::DECR) {
			cur_depth++;
			int sz = 1;
			for (int v = cur; tree[v].type == n.type; v = tree[v].c[0]) {
				sz++;
			}
			tot_ways += c2(sz);
			num_start[n.l] += sz - 1;
			num_end[n.r] += sz - 1;
			int idx = 0;
			int v;
			for (v = cur; tree[v].type == n.type; v = tree[v].c[0], idx++) {
				assert(idx <= sz - 2);
				assert(idx >= 0);

				q_idx[v] = {sz - 1 - idx, idx + 1};
				par_ways[v] = cur_ways;
				depth[v] = cur_depth;

				assert(v & 1);
				num_end[(v-1)/2] += sz - 2 - idx;
				num_start[(v+1)/2] += idx;

				self(tree[v].c[1], cur_depth, cur_ways + int64_t(sz - idx) * int64_t(idx + 1) - 1);
			}
			assert(idx == sz-1);
			assert(tree[v].type != PermTree::NodeType::PARTIAL);
			self(v, cur_depth, cur_ways + int64_t(sz - idx) * int64_t(idx + 1) - 1);
		} else {
			if (n.type != PermTree::NodeType::PARTIAL) {
				cur_depth ++;
				tot_ways ++;
				cur_ways ++;
				num_start[n.l]++;
				num_end[n.r]++;
			}
			par_ways[cur] = cur_ways;
			depth[cur] = cur_depth;
			if (n.type != PermTree::NodeType::LEAF) {
				self(n.c[0], cur_depth, cur_ways);
				self(n.c[1], cur_depth, cur_ways);
			}
		}
	})(tree.root, 0, 0);

	for (int i = 0; i+1 < N; i++) num_start[i+1] += num_start[i];
	assert(num_start[N-1] == tot_ways);
	for (int i = N-1; i > 0; i--) num_end[i-1] += num_end[i];
	assert(num_end[0] == tot_ways);

	RangeMinQuery<std::pair<int, int>> left_split;
	{
		std::vector<std::pair<int, int>> a(N-1);
		for (int i = 0; i < N-1; i++) {
			a[i] = {depth[2*i+1], 2*i+1};
		}
		left_split = RangeMinQuery<std::pair<int, int>>(a);
	}
	RangeMinQuery<std::pair<int, int>> right_split;
	{
		std::vector<std::pair<int, int>> a(N-1);
		for (int i = 0; i < N-1; i++) {
			a[i] = {depth[2*i+1], -(2*i+1)};
		}
		right_split = RangeMinQuery<std::pair<int, int>>(a);
	}

	auto query = [&](int l, int r) -> int64_t {
		assert(0 <= l && l <= r && r < N);
		int64_t ans = tot_ways;
		//cerr << "query " << l << ' ' << r << '\n';
		//cerr << "tot_ways " << tot_ways << '\n';
		if (l-1 >= 0) {
			ans -= num_start[l-1];
			//cerr << "too early " << num_start[l-1] << '\n';
		}
		if (r+1 < N) {
			ans -= num_end[r+1];
			//cerr << "too late " << num_end[r+1] << '\n';
		}
		if (l-1 >= 0 && r+1 < N) {
			auto ls = left_split.query(l-1, r+1 - 1).second;
			auto rs = -right_split.query(l-1, r+1 - 1).second;
			assert(depth[ls] == depth[rs]);
			assert(par_ways[ls] == par_ways[rs]);
			//cerr << "parent " << par_ways[ls] << '\n';
			//cerr << "own " << int64_t(q_idx[ls][0]) * int64_t(q_idx[rs][1]) << '\n';
			ans += par_ways[ls] + int64_t(q_idx[ls][0]) * int64_t(q_idx[rs][1]);
		}
		return ans;
	};

	if (TESTING) {
		auto solve_slow = [&](int l, int r) -> int64_t {
			int64_t ans = 0;
			for (int x = l; x <= r; x++) {
				int lo = A[x], hi = A[x];
				for (int y = x; y <= r; y++) {
					lo = std::min(lo, A[y]);
					hi = std::max(hi, A[y]);
					if (hi - lo == y - x) {
						ans++;
					}
				}
			}
			return ans;
		};
		for (int l = 0; l < N; l++) {
			for (int r = l; r < N; r++) {
				auto slo = solve_slow(l, r);
				auto res = query(l, r);
				if (slo != res) {
					for (int a : A) { cerr << a << ' '; } cerr << '\n';
					cerr << l << ' ' << r << '\n';
					cerr << slo << ' ' << res << '\n';
					assert(false);
				}
			}
		}
	} else {
		int Q; cin >> Q;
		while (Q--) {
			int l, r; cin >> l >> r; l--, r--;
			cout << query(l, r) << '\n';
		}
	}

	return 0;
}
