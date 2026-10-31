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

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, H, S; cin >> N >> H >> S;
	std::vector<int> P(N);
	for (auto& a : P) {
		 cin >> a;
	}

	int num_totem; cin >> num_totem;
	std::vector<bool> is_totem(N);
	for (int z = 0; z < num_totem; z++) {
		int a; cin >> a; a--;
		is_totem[a] = true;
	}

	int T1, T2; cin >> T1 >> T2;
	T1 = std::min(T1, T2);
	tensor<double, 3> dp({N+1, H+1, S+1}, 0);

	for (int i = N-1; i >= 0; i--) {
		double p = P[i] / 100.;
		if (is_totem[i] && S > 0) {
			std::vector<double> cnd(H+1, 1e18);
			for (int lim = 2; lim <= H; lim++) {
				for (int h = lim; h <= H; h++) {
					if (h == lim) {
						dp[{i, h, S}] = (1 + (1-p) * T1 + p * dp[{i+1, h, S}]) / p;
					} else {
						dp[{i, h, S}] = 1 + (1-p) * (dp[{i, h-1, S}]) + p * dp[{i+1, h, S}];
					}
				}
				for (int h = 2; h <= H; h++) {
					cnd[h] = std::min(cnd[h], h < lim ? dp[{i, lim, S}] + (lim - h) * T1 : dp[{i, h, S}]);
				}
			}

			for (int h = 2; h <= H; h++) {
				for (int s = 0; s <= S; s++) {
					dp[{i, h, s}] = cnd[h];
				}
			}
		} else {
			// if we have 1 health and 0 mana, then we loop with prob 1-p and advance with prob p
			// v = 1 + (1-p) * (t2 + v) + p * dp[i+1, 2, 0]
			// v = (1 + (1-p) * t2 + p * dp[i+1, 2, 0]) / (1-p)
			dp[{i, 2, 0}] = (1 + (1-p) * T2 + p * dp[{i+1, 2, 0}]) / p;
			for (int s = 1; s <= S; s++) {
				dp[{i, 2, s}] = 1 + (1-p) * (T1 + dp[{i, 2, s-1}]) + p * dp[{i+1, 2, s}];
			}
			for (int h = 3; h <= H; h++) {
				for (int s = 0; s <= S; s++) {
					dp[{i, h, s}] = 1 + (1-p) * dp[{i, h-1, s}] + p * dp[{i+1, h, s}];
				}
			}
		}
	}

	cout << fixed << setprecision(12) << dp[{0, H, S}] << '\n';

	return 0;
}
