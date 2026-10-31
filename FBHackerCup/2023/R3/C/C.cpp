#include<bits/stdc++.h>

template <typename T> void setmax(T& a, T b) { if (b > a) a = b; }
template <typename T> void setmin(T& a, T b) { if (b < a) a = b; }

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

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		int R, C; cin >> R >> C;
		cerr << "Case #" << case_num << ": " << R << " " << C << endl;

		tensor<int64_t, 2> A({R, C});
		for (int i = 0; i < R; i++) {
			for (int j = 0; j < C; j++) {
				cin >> A[{i, j}];
			}
		}

		tensor<int64_t, 2> st_val({R, C});
		for (int i = 0; i < R; i++) {
			for (int j = 0; j < C; j++) {
				st_val[{i, j}] = 0;
				if (i > 0) setmax(st_val[{i, j}], st_val[{i-1, j}]);
				if (j > 0) setmax(st_val[{i, j}], st_val[{i, j-1}]);
				st_val[{i, j}] += A[{i, j}];
			}
		}
		tensor<int64_t, 2> en_val({R, C});
		for (int i = R-1; i >= 0; i--) {
			for (int j = C-1; j >= 0; j--) {
				en_val[{i, j}] = 0;
				if (i < R-1) setmax(en_val[{i, j}], en_val[{i+1, j}]);
				if (j < C-1) setmax(en_val[{i, j}], en_val[{i, j+1}]);
				en_val[{i, j}] += A[{i, j}];
			}
		}

		auto get_st = [&](int a, int b) -> int64_t {
			assert(-1 <= a && a < R);
			assert(-1 <= b && b < C);
			if (a == -1 || b == -1) return 0;
			return st_val[{a, b}];
		};
		auto get_en = [&](int a, int b) -> int64_t {
			assert(0 <= a && a <= R);
			assert(0 <= b && b <= C);
			if (a == R || b == C) return 0;
			return en_val[{a, b}];
		};

		const int64_t INF = 1e18;
		tensor<int64_t, 3> dp({R, C, 2}, INF);
		dp[{0, C-1, 0}] = 0;
		dp[{0, C-1, 1}] = 0;
		for (int i = 0; i < R; i++) {
			for (int j = C-1; j >= 0; j--) {
				// 0 means walking down
				{
					int z = 0;
					int64_t cross_vals = 0;
					for (int steps = 1; i + steps < R; steps++) {
						if (steps >= 2) {
							setmax(
								cross_vals,
								get_st(i + steps - 1, j - 1) + get_en(i + steps - 1, j + 1)
							);
						}
						setmin(
							dp[{i + steps, j, !z}],
							std::max({
								dp[{i, j, z}],
								// top to bottom
								get_st(i - 1, j) + get_en(i + steps + 1, j),
								get_st(i - 1, j) + get_en(i + 1, j + 1),
								get_st(i + steps - 1, j - 1) + get_en(i + steps + 1, j),

								get_st(i + steps - 1, j - 1) + get_en(i + steps, j + 1),
								cross_vals
								//(steps == 1 ? get_st(i + steps - 1, j - 1) + get_en(i + steps, j + 1) : cross_vals)
							})
						);
					}
				}
				// 1 means walking left
				{
					int z = 1;
					int64_t cross_vals = 0;
					for (int steps = 1; j - steps >= 0; steps++) {
						if (steps >= 2) {
							setmax(
								cross_vals,
								get_st(i - 1, j - steps + 1) + get_en(i + 1, j - steps + 1)
							);
						}
						setmin(
							dp[{i, j - steps, !z}],
							std::max({
								dp[{i, j, z}],
								get_st(i, j - steps - 1) + get_en(i, j + 1),
								get_st(i, j - steps - 1) + get_en(i + 1, j - steps + 1),
								get_st(i - 1, j - 1) + get_en(i, j + 1),

								get_st(i - 1, j - 1) + get_en(i + 1, j),
								cross_vals
								//(steps == 1 ? get_st(i - 1, j - 1) + get_en(i + 1, j) : cross_vals)
							})
						);
					}
				}
			}
		}

		int64_t ans = std::min(dp[{R-1, 0, 0}], dp[{R-1, 0, 1}]);

		cout << "Case #" << case_num << ": " << ans << '\n';
	}

	return 0;
}
