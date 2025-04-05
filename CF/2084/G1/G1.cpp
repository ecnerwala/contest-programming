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
#ifdef _GLIBCXX_DEBUG
		return data[flatten_index_checked(idx)];
#else
		return data[flatten_index(idx)];
#endif
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

bool winning_fast(int n, int m) {
	if ((m & 1) == (n & 1) || ((m >> (n-1)) & 1) == (n & 1)) return n & 1;
	else return !(n & 1);
}

void slow(int N) {
	std::vector<std::vector<bool>> winning(N+1);
	for (int n = 1; n <= N; n++) {
		winning[n].resize(1 << n);
		for (int m = 0; m < (1 << n); m++) {
			if (n == 1) {
				winning[n][m] = m;
				continue;
			}
			winning[n][m] = false;
			for (int i = 0; i < n; i++) {
				// you can delete i if it's 1, or it's 0 and it's followed by a 0
				if (((m >> i) & 1) || (i+1 < n && (((m >> i) & 3) == 0))) {
					int nm = (m & ((1 << i) - 1)) | ((m >> (i+1)) << i);
					if (!winning[n-1][(1 << (n-1)) - 1 - nm]) { winning[n][m] = true; break; }
				}
			}
		}
		using std::cerr;
		for (int m = 0; m < (1 << n); m++) {
			//for (int i = 0; i < n; i++) { cerr << ((m >> i) & 1); } cerr << ' '; cerr << winning[n][m] << '\n';
			assert(winning[n][m] == winning_fast(n, m));
		}
	}
}

// if it's odd length, i.e. same parity, it's the max of the first and last, otherwise it's the min
// max means we want same side distance to be large, and opposite side distance to be small

template <typename T> void setmax(T& a, T b) { if (b > a) a = b; }
int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	//slow(20);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> A(N);
		for (auto& a : A) { cin >> a; a--; }
		// we only care about the evens and the odds
		std::vector<int> invA(N, -1);
		for (int i = 0; i < N; i++) if (A[i] != -1) invA[A[i]] = i;

		const int64_t INF = 1e18;
		int F = N / 2;
		tensor<int64_t, 2> dp({N+1, F+1}, -INF);
		dp[{0, 0}] = 0;
		for (int i = 0; i < N; i++) {
			for (int c = 0; c <= i && c <= F; c++) {
				if (dp[{i, c}] == -INF) continue;
				dp[{i, c}] += (2 * c - i) * (2 * (F - c) - (N-i));
				for (int z = 0; z < 2 && c + z <= F; z++) {
					if (invA[i] == -1 || ((invA[i] & 1) == z)) {
						setmax(dp[{i+1, c + z}], dp[{i, c}]);
					}
				}
			}
		}
		int64_t ans = dp[{N, F}];
		ans += int64_t(N+1) * int64_t(N) * int64_t(N+1) / 2;
		assert(!(ans & 1));
		ans /= 2;
		cout << ans << '\n';
	}

	return 0;
}
