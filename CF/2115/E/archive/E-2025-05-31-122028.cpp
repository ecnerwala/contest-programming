#include <bits/stdc++.h>

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
			std::cerr << i << ' ' << idx[i] << ' ' << shape[i] << '\n';
			assert(0 <= idx[i] && idx[i] < shape[i]);
			res += idx[i] * strides[i];
		}
		return res;
	}

public:
	T& operator[] (std::array<int, NDIMS> idx) const {
		return data[flatten_index_checked(idx)];
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
		len = 1;
		for (int i = NDIMS-1; i >= 0; i--) {
			strides[i] = len;
			len *= shape[i];
		}
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

template <typename T> void setmax(T& a, T b) { if (b > a) a = b; }

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M; cin >> N >> M;
	struct card_t {
		int c, w;
	};
	std::vector<card_t> C(N);
	for (auto& [c, w] : C) cin >> c >> w;

	const int CMAX = 201;
	const int SMAX = CMAX * CMAX;

	std::vector<std::vector<int>> adj(N);
	for (int e = 0; e < M; e++) {
		int u, v; cin >> u >> v; u--, v--;
		adj[u].push_back(v);
	}

	int Q; cin >> Q;
	struct query_t {
		int p, r;
	};
	std::vector<query_t> queries(Q);
	for (auto& [p, r] : queries) { cin >> p >> r; p--; }

	std::vector<int64_t> ans(Q);

	{
		tensor<int64_t, 2> dp({N, SMAX+1}, 0);
		for (int i = 0; i < N; i++) {
			// infinite use knapsack
			for (int j = 0; j + C[i].c <= SMAX; j++) {
				setmax(dp[{i, j + C[i].c}], dp[{i, j}] + C[i].w);
			}
			for (auto k : adj[i]) {
				for (int j = 0; j <= SMAX; j++) {
					setmax(dp[{k, j}], dp[{i, j}]);
				}
			}
		}

		for (int q = 0; q < Q; q++) {
			setmax(ans[q], dp[{queries[q].p, std::min(queries[q].r, SMAX)}]);
		}
	}

	for (int mid = 0; mid < N; mid++) {
		int MOD = C[mid].c;
		int64_t MOD_VAL = C[mid].w;
		std::vector<bool> vis(N, false);
		tensor<int64_t, 2> dp({N, MOD}, 0);
		vis[0] = true;

		for (int i = 0; i < N; i++) {
			if (!vis[i]) continue;
			int g = std::gcd(C[i].c, MOD);
			for (int st = 0; st < g; st++) {
				int j = st;
				for (int a = 0; a <= 2 * (MOD / g); a++) {
					int nj = j + C[i].c;
					if (nj >= MOD) {
						setmax(dp[{i, nj - MOD}], dp[{i, j}] + C[i].w - MOD_VAL);
					} else {
						setmax(dp[{i, nj}], dp[{i, j}] + C[i].w);
					}
				}
			}
			// We want to cycle forwards twice
			for (int j = 0; j < MOD; j++) {
			}
			for (auto k : adj[i]) {
				if (i < mid && mid < k) continue;
				vis[k] = true;
				for (int j = 0; j < MOD; j++) {
					setmax(dp[{k, j}], dp[{i, j}]);
				}
			}
		}

		for (int q = 0; q < Q; q++) {
			if (queries[q].p >= mid && vis[queries[q].p] && queries[q].r >= SMAX) {
				setmax(ans[q], dp[{queries[q].p, queries[q].r % MOD}] + (queries[q].r / MOD) * MOD_VAL);
			}
		}
	}

	for (auto a : ans) cout << a << '\n';

	return 0;
}

// If r is smaller than c^2, we just do it
// If r is at least c_max^2, then we only care about the mod, since we use at most c_best - 1 non-best items
