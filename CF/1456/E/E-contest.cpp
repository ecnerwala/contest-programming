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

template <typename T> void setmin(T& a, T b) { if (b < a) a = b; }

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int N, K; cin >> N >> K;
	vector<array<int64_t, 2>> bounds(N);
	// make them both exclusive
	for (auto& a : bounds) {
		cin >> a[0] >> a[1];
		a[0]--, a[1]++;
	}
	vector<int64_t> costs(K); for (auto& c : costs) cin >> c;

	tensor<int64_t, 5> dp({N+1,N+1,4,4,K+1}, -1);
	const int64_t INF = 1e18;
	using state_t = array<int, 5>;

	cout << (std::y_combinator([&](auto self, state_t s) -> int64_t {
		auto [i, j, mi, mj, k] = s;
		if (k == K && j - i == 1) {
			return 0;
		}
		array<int, 5> key{i+1, j, mi, mj, k};
		if (dp[key] != -1) return dp[key];

		int64_t ans = INF;

		if (k < K) {
			bool needs = i >= 0 && j <= N-1 && bool((((bounds[i][mi>>1] ^ bounds[j][mj>>1]) >> k) ^ mi ^ mj) & 1);
			setmin(ans, self(state_t{i,j,mi&2,mj&2,k+1}) + (needs * costs[k]));
		}

		for (int md = i+1; md <= j-1; md++) {
			if (!((bounds[md][0] >> k) & 1)) {
				// we can try using this midpoint
				int64_t mm = (bounds[md][0] >> k) ^ 1;
				assert(bounds[md][0] < (mm << k));
				if (((mm + 1) << k) <= bounds[md][1]) {
					setmin(ans, self(state_t{i,md,mi,1,k}) + self(state_t{md,j,1,mj,k}));
				}
			}
			if (((bounds[md][1] >> k) & 1)) {
				int64_t mm = (bounds[md][1] >> k) ^ 1;
				assert(((mm + 1) << k) <= bounds[md][1]);
				if (bounds[md][0] < (mm << k)) {
					setmin(ans, self(state_t{i,md,mi,3,k}) + self(state_t{md,j,3,mj,k}));
				}
			}
		}

		return dp[key] = ans;
	})(state_t{-1, N, 0, 0, 0})) << '\n';

	/*
	// half-open interval of internals
	tensor<int64_t, 5> dp({N+1, N+1, 2, 2, K+1}, INF);
	for (int i = 0; i <= N; i++) {
		for (int z1 = 0; z1 < 2; z1++) {
			for (int z2 = 0; z2 < 2; z2++) {
				dp[{i,z1,i,z2,K}] = 0;
			}
		}
	}
	for (int k = K; k >= 0; k--) {
		if (k < K) {
			for (int i = 0; i <= N; i++) {
				for (int j = i; j <= N; j++) {
					for (int zi = 0; zi < 2; zi++) {
						for (int zj = 0; zj < 2; zj++) {
							bool needs = (i > 0 && j < N && (((bounds[i-1][zi]^bounds[j][zj])>>k)&1));
							setmin(dp[{i,j,zi,zj,k}], dp[{i,j,zi,zj,k+1}] + (needs * costs[k]));
						}
					}
				}
			}
		}
		for (int m = 0; m < N; m++) {
			// If this guy can be a range of size (1 << k), we can merge two intervals together
			for (int zm = 0; zm < 2; zm++) {
				for (int l = 0; l <= m; l++) {
					for (int r = m+1; r <= N; r++) {
						for (int zl = 0; zl < 2; zl++) {
							for (int zr = 0; zr < 2; zr++) {
							}
						}
					}
				}
			}
		}
	}

	int64_t ans = INF;
	for (int z1 = 0; z1 < 2; z1++) {
		for (int z2 = 0; z2 < 2; z2++) {
			setmin(ans, dp[{0,z1,N,z2,0}]);
		}
	}
	cout << ans << '\n';
	*/

	return 0;
}
