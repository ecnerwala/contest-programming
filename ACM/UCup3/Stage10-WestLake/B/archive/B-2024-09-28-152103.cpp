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

class CartesianTree {
public:
	struct Node {
		int l, m, r; // inclusive ranges
		std::array<int, 2> c;
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
			nodes[2*i+1].c[0] = cur;
			stk.push_back(2*i+1);
		}
		return {std::move(nodes), root};
	}

	// max-cartesian-tree, with earlier cells tiebroken earlier
	template <typename T, typename Comp = std::less<T>>
	static CartesianTree build_max_tree(const std::vector<T>& v, Comp comp = Comp()) {
		return build_min_tree(v, reverse_comparator(comp));
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

template<class T> int sz(T&& arg) { using std::size; return int(size(std::forward<T>(arg))); }

class SuffixArray {
public:
	using index_t = int;
	int N;
	std::vector<index_t> sa;
	std::vector<index_t> rank;
	// lcp[i] = get_lcp(sa[i], sa[i+1])
	std::vector<index_t> lcp;
	RangeMinQuery<std::pair<index_t, index_t>> rmq;

	SuffixArray() {}

	template <typename String> static SuffixArray construct_raw(const String& S, index_t sigma) {
		int N = sz(S);
		SuffixArray sa(N);

		sa.build_sa(S, sigma);
		sa.build_rank();
		sa.build_lcp(S);
		sa.build_rmq();

		return sa;
	}

	// Pass a function which returns a value in [0, sigma)
	template <typename String, typename F> static SuffixArray map_and_construct(const String& S, const F& f, int sigma) {
		std::vector<decltype((f(S[0])))> mapped(sz(S));
		for (int i = 0; i < sz(S); i++) {
			mapped[i] = f(S[i]);
			assert(0 <= int(mapped[i]) && int(mapped[i]) < sigma);
		}
		return construct_raw(mapped, sigma);
	}

	// Sorts the elements of S and then runs suffix array. This takes O(N log N) time with no dependence on sigma.
	template <typename String> static SuffixArray sort_and_construct(const String& S) {
		using std::begin;
		using std::end;
		using value_type = typename std::iterator_traits<decltype(begin(S))>::value_type;
		using compressed_value_type = typename std::conditional<
			sizeof(value_type) < sizeof(index_t),
			value_type,
			index_t
		>::type;

		std::vector<compressed_value_type> compressed_s(sz(S));
		int sigma = 0;

		{
			std::vector<value_type> vals(begin(S), end(S));
			std::sort(vals.begin(), vals.end());
			vals.resize(unique(vals.begin(), vals.end()) - vals.begin());
			for (int i = 0; i < sz(S); i++) {
				compressed_s[i] = compressed_value_type(index_t(std::lower_bound(vals.begin(), vals.end(), S[i]) - vals.begin()));
			}
			sigma = int(vals.size());
		}

		return construct_raw(compressed_s, sigma);
	}

	// Shifts the elements so that sigma = max(S) - min(S) + 1
	template <typename String> static SuffixArray shift_and_construct(const String& S) {
		using std::begin;
		using std::end;
		using value_type = typename std::iterator_traits<decltype(begin(S))>::value_type;

		std::vector<value_type> compressed_s(sz(S));
		int sigma = 0;

		if (sz(S) > 0) {
			value_type lo = *begin(S), hi = *begin(S);
			for (const auto& x : S) {
				if (x < lo) lo = x;
				if (x > hi) hi = x;
			}

			for (int i = 0; i < sz(S); i++) {
				compressed_s[i] = value_type(S[i] - lo);
			}
			sigma = int(hi - lo + 1);
		}

		return construct_raw(compressed_s, sigma);
	}

	// Renumber/filter to only the used elements with bucket sorting. Still takes O(max(S) - min(S) + 1) memory/time,
	// but should be less memory than `shift_and_construct` when sigma ~ N and max(S) - min(S) + 1 > N.
	template <typename String> static SuffixArray bucket_and_construct(const String& S) {
		using std::begin;
		using std::end;
		using value_type = typename std::iterator_traits<decltype(begin(S))>::value_type;
		using compressed_value_type = typename std::conditional<
			sizeof(value_type) < sizeof(index_t),
			value_type,
			index_t
		>::type;

		std::vector<compressed_value_type> compressed_s(sz(S));
		int sigma = 0;

		if (sz(S) > 0) {
			value_type lo = *begin(S), hi = *begin(S);
			for (const auto& x : S) {
				if (x < lo) lo = x;
				if (x > hi) hi = x;
			}

			std::vector<compressed_value_type> buckets(hi - lo + 1, 0);
			for (const auto& x : S) {
				buckets[x - lo] = 1;
			}
			for (int v = 0; v < int(buckets.size()); v++) {
				if (buckets[v]) buckets[v] = compressed_value_type(sigma++);
			}

			for (int i = 0; i < sz(S); i++) {
				compressed_s[i] = buckets[S[i] - lo];
			}
		}

		return construct_raw(compressed_s, sigma);
	}

	index_t get_lcp(index_t a, index_t b) const {
		if (a == b) return N-a;
		a = rank[a], b = rank[b];
		if (a > b) std::swap(a, b);
		return rmq.query(a, b-1).first;
	}

	// Get the split in the suffix tree, using half-open intervals
	// Returns len, idx
	std::pair<index_t, index_t> get_split(index_t l, index_t r) const {
		assert(r - l > 1);
		return rmq.query(l, r-2);
	}

private:
	explicit SuffixArray(int N_) : N(N_) {}

	template <typename String> void build_sa(const String& S, index_t sigma) {
		sa = std::vector<index_t>(N+1);
		assert(sigma >= 0);
		for (auto s : S) assert(0 <= index_t(s) && index_t(s) < sigma);
		std::vector<index_t> tmp(sigma + std::max(N, sigma));
		SuffixArray::sais<String>(N, S, sa.data(), sigma, tmp.data());
	}

	template <typename String> static void sais(int N, const String& S, index_t* sa, int sigma, index_t* tmp) {
		if (N == 0) {
			sa[0] = 0;
			return;
		} else if (N == 1) {
			sa[0] = 1;
			sa[1] = 0;
			return;
		}

		// Phase 1: Initialize the frequency array, which will let us lookup buckets.
		index_t* freq = tmp; tmp += sigma;
		memset(freq, 0, sizeof(*freq) * sigma);
		for (int i = 0; i < N; i++) {
			++freq[index_t(S[i])];
		}
		auto build_bucket_start = [&]() {
			int cur = 1;
			for (int v = 0; v < sigma; v++) {
				tmp[v] = cur;
				cur += freq[v];
			}
		};
		auto build_bucket_end = [&]() {
			int cur = 1;
			for (int v = 0; v < sigma; v++) {
				cur += freq[v];
				tmp[v] = cur;
			}
		};

		int num_pieces = 0;

		int first_endpoint = 0;
		// Phase 2: find the right-endpoints of the pieces
		{
			build_bucket_end();

			// Initialize the final endpoint out-of-band this way so that we don't try to look up tmp[-1].
			// This doesn't count towards num_pieces.
			sa[0] = N;

			index_t c0 = S[N-1], c1 = -1; bool isS = false;
			for (int i = N-2; i >= 0; i--) {
				c1 = c0;
				c0 = S[i];
				if (c0 < c1) {
					isS = true;
				} else if (c0 > c1 && isS) {
					isS = false;
					// insert i+1
					sa[first_endpoint = --tmp[c1]] = i+1;
					++num_pieces;
				}
			}
		}

		// If num_pieces <= 1, we don't need to actually run the recursion, it's just sorted automatically
		// Otherwise, we're going to rebucket
		if (num_pieces > 1) {
			// Remove the first endpoint, we don't need to run the IS on this
			sa[first_endpoint] = 0;

			// Run IS for L-type
			{
				build_bucket_start();
				for (int z = 0; z <= N; z++) {
					int v = sa[z];
					if (!v) continue;

					// Leave for the S-round
					if (v < 0) continue;

					// clear out our garbage
					sa[z] = 0;

					--v;
					index_t c0 = S[v-1], c1 = S[v];
					sa[tmp[c1]++] = (c0 < c1) ? ~v : v;
				}
			}

			index_t* const sa_end = sa + N + 1;

			index_t* pieces = sa_end;
			// Run IS for S-type and compactify
			{
				build_bucket_end();
				for (int z = N; z >= 0; z--) {
					int v = sa[z];
					if (!v) continue;

					// clear our garbage
					sa[z] = 0;

					if (v > 0) {
						*--pieces = v;
						continue;
					}

					v = ~v;

					--v;
					index_t c0 = S[v-1], c1 = S[v];
					sa[--tmp[c1]] = (c0 > c1) ? v : ~v;
				}
			}

			// Compute the lengths of the pieces in preparation for equality
			// comparison, and store them in sa[v/2]. We set the length of the
			// final piece to 0; it compares unequal to everything because of
			// the sentinel.
			{
				int prv_start = N;
				index_t c0 = S[N-1], c1 = -1; bool isS = false;
				for (int i = N-2; i >= 0; i--) {
					c1 = c0;
					c0 = S[i];
					if (c0 < c1) {
						isS = true;
					} else if (c0 > c1 && isS) {
						isS = false;

						// insert i+1
						int v = i+1;
						sa[v>>1] = prv_start == N ? 0 : prv_start - v;
						prv_start = v;
					}
				}
			}

			// Compute the alphabet, storing the result into sa[v/2].
			int next_sigma = 0;
			{
				int prv_len = -1, prv_v = 0;
				for (int i = 0; i < num_pieces; i++) {
					int v = pieces[i];
					int len = sa[v>>1];

					bool eq = prv_len == len;
					for (int a = 0; eq && a < len; ++a) {
						eq = S[v+a] == S[prv_v+a];
					}
					if (!eq) {
						next_sigma++;
						prv_len = len;
						prv_v = v;
					}

					sa[v>>1] = next_sigma; // purposely leave this 1 large to check != 0
				}
			}

			if (next_sigma == num_pieces) {
				sa[0] = N;
				memcpy(sa+1, pieces, sizeof(*sa) * num_pieces);
			} else {
				index_t* next_S = sa_end;

				// Finally, pack the input to the SA
				{
					for (int i = (N-1)>>1; i >= 0; i--) {
						int v = sa[i];
						if (v) *--next_S = v-1;
						sa[i] = 0;
					}
				}

				memset(sa, 0, sizeof(*sa) * (num_pieces+1));
				sais<const index_t*>(num_pieces, next_S, sa, next_sigma, tmp);

				{ // Compute the piece start points again and use those to map up the suffix array
					next_S = sa_end;
					index_t c0 = S[N-1], c1 = -1; bool isS = false;
					for (int i = N-2; i >= 0; i--) {
						c1 = c0;
						c0 = S[i];
						if (c0 < c1) {
							isS = true;
						} else if (c0 > c1 && isS) {
							isS = false;

							int v = i+1;
							*--next_S = v;
						}
					}
					sa[0] = N;
					for (int i = 1; i <= num_pieces; i++) {
						sa[i] = next_S[sa[i]];
					}
				}
			}

			// zero everything else
			memset(sa+num_pieces+1, 0, sizeof(*sa) * (N - num_pieces));

			{
				// Scatter the finished pieces
				build_bucket_end();
				for (int i = num_pieces; i > 0; i--) {
					int v = sa[i];
					sa[i] = 0;

					index_t c1 = S[v];
					sa[--tmp[c1]] = v;
				}
			}
		}

		// Home stretch! Just finish out with the L-type and then S-type
		{
			build_bucket_start();
			for (int z = 0; z <= N; z++) {
				int v = sa[z];
				if (v <= 0) continue;
				--v;
				index_t c1 = S[v];
				index_t c0 = v ? S[v-1] : c1; // if v = 0, we don't want to invert
				sa[tmp[c1]++] = (c0 < c1) ? ~v : v;
			}
		}

		// This just aggressively overwrites our original scattered pieces with the correct values
		{
			build_bucket_end();
			for (int z = N; z >= 0; z--) {
				int v = sa[z];
				if (v >= 0) continue;
				sa[z] = v = ~v;
				--v;
				index_t c1 = S[v];
				index_t c0 = v ? S[v-1] : c1+1;
				sa[--tmp[c1]] = (c0 > c1) ? v : ~v;
			}
		}
	}

	void build_rank() {
		rank = std::vector<index_t>(N+1);
		for (int i = 0; i <= N; i++) rank[sa[i]] = i;
	}

	template <typename String> void build_lcp(const String& S) {
		assert(sz(S) == N);
		lcp = std::vector<index_t>(N);
		for (int i = 0, k = 0; i < N - 1; i++) {
			int j = sa[rank[i]-1];
			while (k < N - std::max(i, j) && S[i+k] == S[j+k]) k++;
			lcp[rank[i]-1] = k;
			if (k) --k;
		}
	}

	void build_rmq() {
		std::vector<std::pair<index_t, index_t>> lcp_idx(N);
		for (int i = 0; i < N; i++) {
			lcp_idx[i] = {lcp[i], i+1};
		}
		rmq = RangeMinQuery<std::pair<index_t, index_t>>(std::move(lcp_idx));
	}
};

class PrefixArray : private SuffixArray {
	PrefixArray(const SuffixArray& sa_) : SuffixArray(sa_) {}
	PrefixArray(SuffixArray&& sa_) : SuffixArray(std::move(sa_)) {}
public:
	PrefixArray() {}
	template <typename String> static PrefixArray construct_raw(const String& S, int sigma) {
		return PrefixArray(SuffixArray::construct_raw(String(S.rbegin(), S.rend())), sigma);
	}

	// TODO: Fill in other constructors

	int get_lcs(int a, int b) const {
		return SuffixArray::get_lcp(SuffixArray::N - a, SuffixArray::N - b);
	}
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, Q; cin >> N >> Q;
	std::string S; S.reserve(N); cin >> S;
	assert(int(S.size()) == N);
	struct query_t {
		int op_type;
		std::array<std::vector<std::array<int, 2>>, 2> s;
	};
	std::vector<query_t> queries(Q);
	auto read_inters = [&](std::vector<std::array<int, 2>>& v) -> void {
		int k; cin >> k;
		v.resize(k);
		for (auto& a : v) { cin >> a[0] >> a[1]; a[0]--; }
	};

	for (int q = 0; q < Q; q++) {
		char c; cin >> c;
		if (c == '+') {
			queries[q].op_type = 0;
			read_inters(queries[q].s[0]);
			queries[q].s[1] = queries[q].s[0];
		} else if (c == '-') {
			int t; cin >> t;
			queries[q].op_type = ~t;
		} else if (c == '?') {
			queries[q].op_type = 1;
			read_inters(queries[q].s[0]);
			read_inters(queries[q].s[1]);
		} else assert(false);
	}

	for (int z = 0; z < 2; z++) {
		if (z) {
			std::reverse(S.begin(), S.end());
			for (int q = 0; q < Q; q++) {
				if (queries[q].op_type >= 0) {
					auto& s = queries[q].s[z];
					std::reverse(s.begin(), s.end());
					for (auto& a : queries[q].s[z]) {
						a = {N - a[1], N - a[0]};
					}
				}
			}
		}
		SuffixArray sa = SuffixArray::shift_and_construct(S);
		// inclusive interval
		auto get_interval = [&](std::array<int, 2> subst) -> std::array<int, 2> {
			int rnk = sa.rank[subst[0]];
			int len = subst[1] - subst[0];
			int lo, hi;
			{
				int mi = -1;
				int ma = rnk;
				while (ma - mi > 1) {
					int md = (mi + ma) / 2;
					if (sa.rmq.query(md, rnk - 1).first >= len) {
						ma = md;
					} else {
						mi = md;
					}
				}
				lo = ma;
			}
			{
				int mi = rnk;
				int ma = N+1;
				while (ma - mi > 1) {
					int md = (mi + ma) / 2;
					if (sa.rmq.query(rnk, md-1).first >= len) {
						mi = md;
					} else {
						ma = md;
					}
				}
				hi = mi;
			}
			return {lo, hi};
		};

		CartesianTree ct = CartesianTree::build_min_tree(sa.lcp);
		assert(int(sa.lcp.size()) == N);
		// for each cartesian tree node, we need
		struct ct_data {
			// min_len is exclusive
			int min_len, max_len;
			int64_t st, en;
		};
		std::vector<ct_data> ct_intervals(ct.size());
		{
			int64_t cur_idx = 0;
			std::y_combinator([&](auto self, int cur, int min_len) -> void {
				int max_len;
				if (cur & 1) {
					// we're an intermediate node
					max_len = sa.lcp[cur / 2];
				} else {
					max_len = N - sa.sa[cur / 2];
				}
				assert(min_len <= max_len);
				int64_t st = cur_idx;
				cur_idx += max_len - min_len;
				if (cur & 1) {
					self(ct[cur].c[0], max_len);
					self(ct[cur].c[1], max_len);
				}
				int64_t en = cur_idx;
				ct_intervals[cur] = {min_len, max_len, st, en};
			})(ct.root, -1);
		}

		auto range_to_inter = [&](std::array<int, 2> bounds, int len) -> std::array<int64_t, 2> {
			int ct_node;
			if (bounds[0] == bounds[1]) {
				ct_node = bounds[0] * 2;
			} else {
				ct_node = sa.rmq.query(bounds[0], bounds[1]-1).second * 2 - 1;
			}
			assert(ct[ct_node].l == bounds[0]);
			assert(ct[ct_node].r == bounds[1]);
			assert(ct_intervals[ct_node].min_len < len && len <= ct_intervals[ct_node].max_len);
			return {ct_intervals[ct_node].st + len - 1 - ct_intervals[ct_node].min_len, ct_intervals[ct_node].en};
		};

		std::vector<std::map<int64_t, int>> trie;
		trie.emplace_back();
		auto trie_move = [&](int trie_node, int64_t cur_str) -> int {
			int nxt_trie_node;
			auto it = trie[trie_node].find(cur_str);
			if (it == trie[trie_node].end()) {
				nxt_trie_node = int(trie.size());
				trie[trie_node].insert({cur_str, nxt_trie_node});
				trie.emplace_back();
			} else {
				nxt_trie_node = it->second;
			}
			return nxt_trie_node;
		};

		for (int q = 0; q < Q; q++) {
			if (queries[q].op_type >= 0) {
				auto& s = queries[q].s[z];

				assert(int(sa.sa.size()) == N+1);
				// canonicalize
				std::array<int, 2> cur_range{0, N};
				int cur_len = 0;
				int trie_node = 0;
				for (int i = 0; i < int(s.size()); i++) {
					auto nxt_subst = s[i];
					int nxt_len = nxt_subst[1] - nxt_subst[0];
					assert(nxt_len >= 1);
					auto nxt_range = get_interval(nxt_subst);
					int mi = cur_range[0] - 1;
					int ma = cur_range[1] + 1;
					while (ma - mi > 1) {
						int md = (mi + ma) / 2;
						assert(sa.sa[md] + cur_len <= N);
						int rnk = sa.rank[sa.sa[md] + cur_len];
						if (rnk < nxt_range[0]) {
							mi = rnk;
						} else {
							ma = rnk;
						}
					}
					// mi, ma are the two boundaries
					int lcp = 0;
					int lcp_idx = cur_range[0];
					if (mi >= cur_range[0]) {
						int cnd = sa.get_lcp(sa.sa[mi] + cur_len, nxt_subst[0]);
						if (cnd > lcp) {
							lcp = cnd;
							lcp_idx = mi;
						}
					}
					if (ma <= cur_range[1]) {
						int cnd = sa.get_lcp(sa.sa[ma] + cur_len, nxt_subst[0]);
						if (cnd > lcp) {
							lcp = cnd;
							lcp_idx = ma;
						}
					}
					lcp = std::min(lcp, nxt_len);
					cur_len += lcp;
					cur_range = get_interval({sa.sa[lcp_idx], sa.sa[lcp_idx] + cur_len});
					if (lcp < nxt_len) {
						// cut it and move on
						int64_t cur_str = range_to_inter(cur_range, cur_len)[0];
						trie_node = trie_move(trie_node, cur_str);

						cur_len = nxt_len - lcp;
						cur_range = get_interval({nxt_subst[0] + lcp, nxt_subst[1]});
					}
					assert(cur_len >= 1);
				}
				// just stuff our data into here
				auto [st, en] = range_to_inter(cur_range, cur_len);
				if (queries[q].op_type == 0) {
					int n = trie_move(trie_node, st);
					s = {{n, n}};
				} else if (queries[q].op_type == 1) {
					int a = trie_move(trie_node, st);
					int b = trie_move(trie_node, en);
					s = {{a, b}};
				} else assert(false);
			}
		}
		std::vector<int> trie_preorder(trie.size(), -1);
		{
			int cur_idx = 0;
			std::y_combinator([&](auto self, int cur) -> void {
				trie_preorder[cur] = cur_idx++;
				for (auto [v, nxt] : trie[cur]) {
					self(nxt);
				}
			});
			assert(cur_idx == int(trie.size()));
		}
		for (int q = 0; q < Q; q++) {
			if (queries[q].op_type >= 0) {
				auto& s = queries[q].s[z];
				assert(int(s.size()) == 1);
				s[0][0] = trie_preorder[s[0][0]];
				s[0][1] = trie_preorder[s[0][1]];
				cerr << "query " << q << ' ' << queries[q].op_type << ' ' << s[0][0] << '-' << s[0][1] << '\n';
			}
		}
	}

	return 0;
}
