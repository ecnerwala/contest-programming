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

/*
 * This is mostly inspired by https://golang.org/src/index/suffixarray/sais.go.
 */

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

	template <typename String> static SuffixArray construct(const String& S) {
		int N = sz(S);
		SuffixArray sa(N);

		sa.build_sa(S);
		sa.build_rank();
		sa.build_lcp(S);
		sa.build_rmq();

		return sa;
	}

	template <typename String, typename F> static SuffixArray map_and_compress(const String& S, const F& f) {
		std::vector<decltype(f(S[0]))> mapped(sz(S));
		for (int i = 0; i < sz(S); i++) {
			mapped[i] = f(S[i]);
		}
		return construct(mapped);
	}

	template <typename String> static SuffixArray compress_and_construct(const String& S) {
		using std::begin;
		using std::end;
		std::vector<decltype(S[0])> vals(begin(S), end(S));
		std::sort(vals.begin(), vals.end());
		vals.resize(unique(vals.begin(), vals.end()) - vals.begin());
		std::vector<decltype(S[0])> compressed_s(sz(S));
		for (int i = 0; i < sz(S); i++) {
			compressed_s[i] = lower_bound(vals.begin(), vals.end(), S[i]) - vals.begin();
		}
		return construct(compressed_s);
	}

	static SuffixArray construct_lower_alpha(const std::string& s) {
		return SuffixArray::map_and_compress(s, [](char c) -> char { return char(c - 'a'); });
	}

	static SuffixArray construct_upper_alpha(const std::string& s) {
		return SuffixArray::map_and_compress(s, [](char c) -> char { return char(c - 'A'); });
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

	template <typename String> void build_sa(const String& S) {
		sa = std::vector<index_t>(N+1);
		for (auto s : S) assert(index_t(s) >= 0);
		int sigma = N ? *max_element(S.begin(), S.end())+1 : 0;
		std::vector<index_t> tmp(std::max(N, sigma * 2));
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

template <typename T, int NDIMS> struct tensor_view {
	static_assert(NDIMS >= 0, "NDIMS must be nonnegative");

protected:
	std::array<int, NDIMS> shape;
	std::array<int, NDIMS> strides;
	T* data;

	tensor_view(std::array<int, NDIMS> shape_, std::array<int, NDIMS> strides_, T* data_) : shape(shape_), strides(strides_), data(data_) {}

public:
	tensor_view() : shape{0}, strides{0}, data(nullptr) {}

protected:
	int flatten_index(std::array<int, NDIMS> idx) const {
		int res = 0;
		for (int i = 0; i < NDIMS; i++) { res += idx[i] * strides[i]; }
		return res;
	}
	int flatten_index_checked(std::array<int, NDIMS> idx) const {
		int res = 0;
		for (int i = 0; i < NDIMS; i++) {
			assert(0 <= idx[i] && idx[i] < shape[i]);
			res += idx[i] * strides[i];
		}
		return res;
	}

public:
	T& operator[] (std::array<int, NDIMS> idx) const {
		return data[flatten_index(idx)];
	}
	T& at(std::array<int, NDIMS> idx) const {
		return data[flatten_index_checked(idx)];
	}

	template <int D = NDIMS>
	typename std::enable_if<(0 < D), tensor_view<T, NDIMS-1>>::type operator[] (int idx) const {
		std::array<int, NDIMS-1> nshape; std::copy(shape.begin()+1, shape.end(), nshape.begin());
		std::array<int, NDIMS-1> nstrides; std::copy(strides.begin()+1, strides.end(), nstrides.begin());
		T* ndata = data + (strides[0] * idx);
		return tensor_view<T, NDIMS-1>(nshape, nstrides, ndata);
	}
	template <int D = NDIMS>
	typename std::enable_if<(0 < D), tensor_view<T, NDIMS-1>>::type at(int idx) const {
		assert(0 <= idx && idx < shape[0]);
		return operator[](idx);
	}

	template <int D = NDIMS>
	typename std::enable_if<(0 == D), T&>::type operator * () const {
		return *data;
	}

	template <typename U, int D> friend struct tensor_view;
	template <typename U, int D> friend struct tensor;
};

template <typename T, int NDIMS> struct tensor {
	static_assert(NDIMS >= 0, "NDIMS must be nonnegative");

protected:
	std::array<int, NDIMS> shape;
	std::array<int, NDIMS> strides;
	int len;
	T* data;

public:
	tensor() : shape{0}, strides{0}, len(0), data(nullptr) {}

	explicit tensor(std::array<int, NDIMS> shape_, const T& t = T()) {
		shape = shape_;
		strides[NDIMS-1] = 1;
		for (int i = NDIMS-1; i > 0; i--) {
			strides[i-1] = strides[i] * shape[i];
		}
		len = strides[0] * shape[0];
		data = new T[len];
		std::fill(data, data + len, t);
	}

	tensor(const tensor& o) : shape(o.shape), strides(o.strides), len(o.len), data(new T[len]) {
		for (int i = 0; i < len; i++) {
			data[i] = o.data[i];
		}
	}

	tensor& operator=(tensor&& o) noexcept {
		using std::swap;
		swap(shape, o.shape);
		swap(strides, o.strides);
		swap(len, o.len);
		swap(data, o.data);
		return *this;
	}
	tensor(tensor&& o) : tensor() {
		*this = std::move(o);
	}
	tensor& operator=(const tensor& o) {
		return *this = tensor(o);
	}
	~tensor() { delete[] data; }

	using view_t = tensor_view<T, NDIMS>;
	view_t view() {
		return tensor_view<T, NDIMS>(shape, strides, data);
	}
	operator view_t() {
		return view();
	}

	using const_view_t = tensor_view<const T, NDIMS>;
	const_view_t view() const {
		return tensor_view<const T, NDIMS>(shape, strides, data);
	}
	operator const_view_t() const {
		return view();
	}

	T& operator[] (std::array<int, NDIMS> idx) { return view()[idx]; }
	T& at(std::array<int, NDIMS> idx) { return view().at(idx); }
	const T& operator[] (std::array<int, NDIMS> idx) const { return view()[idx]; }
	const T& at(std::array<int, NDIMS> idx) const { return view().at(idx); }

	template <int D = NDIMS>
	typename std::enable_if<(0 < D), tensor_view<T, NDIMS-1>>::type operator[] (int idx) {
		return view()[idx];
	}
	template <int D = NDIMS>
	typename std::enable_if<(0 < D), tensor_view<T, NDIMS-1>>::type at(int idx) {
		return view().at(idx);
	}

	template <int D = NDIMS>
	typename std::enable_if<(0 < D), tensor_view<const T, NDIMS-1>>::type operator[] (int idx) const {
		return view()[idx];
	}
	template <int D = NDIMS>
	typename std::enable_if<(0 < D), tensor_view<const T, NDIMS-1>>::type at(int idx) const {
		return view().at(idx);
	}

	template <int D = NDIMS>
	typename std::enable_if<(0 == D), T&>::type operator * () {
		return *view();
	}
	template <int D = NDIMS>
	typename std::enable_if<(0 == D), const T&>::type operator * () const {
		return *view();
	}
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int T; cin >> T;
	while (T--) {
		int N, M, K; cin >> N >> M >> K;
		string A, B; cin >> A >> B;
		SuffixArray sa = SuffixArray::construct_lower_alpha(A + char('z' + 1) + B);
		tensor<array<int, 2>, 2> best_loc({K+1,2*K+1}, {-1,-1});
		tensor<int, 2> prv({K+1,2*K+1}, -1);
		best_loc[{0,0}] = {0,0};
		for (int k = 0; k <= K; k++) {
			for (int i = 0; i <= 2*k; i++) {
				int a = best_loc[{k,i}][0], b = best_loc[{k,i}][1];
				if (a == -1 && b == -1) continue;

				int jmp = sa.get_lcp(a, N + 1 + b);
				a += jmp, b += jmp;
				if (a == N && b == M) {
					cout << "YES" << '\n';
					cout << k << '\n';
					for (int cur_k = k, cur_i = i; cur_k != 0; cur_i = prv[{cur_k, cur_i}], cur_k--) {
						int d = cur_i - prv[{cur_k, cur_i}];
						int a = best_loc[{cur_k, cur_i}][0], b = best_loc[{cur_k, cur_i}][1];
						if (d == 0) {
							// we incremented a by 1; delete from this position
							cout << "DELETE " << a << '\n';
						} else if (d == 1) {
							cout << "REPLACE " << a << ' ' << B[b-1] << '\n';
						} else if (d == 2) {
							cout << "INSERT " << a+1 << ' ' << B[b-1] << '\n';
						} else assert(false);
					}
					goto found;
				}
				if (k == K) continue;
				if (a < N) {
					// we can increment a by 1, and i by 0
					array<int, 2> cnd{a+1, b};
					if (cnd > best_loc[{k+1, i+0}]) {
						best_loc[{k+1,i+0}] = cnd;
						prv[{k+1, i+0}] = i;
					}
				}
				if (b < M) {
					// we can increment b by 1, and i by 2
					array<int, 2> cnd{a, b+1};
					if (cnd > best_loc[{k+1, i+2}]) {
						best_loc[{k+1,i+2}] = cnd;
						prv[{k+1, i+2}] = i;
					}
				}
				if (a < N && b < M) {
					// we can increment both by 1.
					array<int, 2> cnd{a+1, b+1};
					if (cnd > best_loc[{k+1, i+1}]) {
						best_loc[{k+1,i+1}] = cnd;
						prv[{k+1, i+1}] = i;
					}
				}
			}
		}
		cout << "NO" << '\n';
found:;
	}

	return 0;
}
