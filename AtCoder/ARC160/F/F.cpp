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

	int N, Q; cin >> N >> Q;

	tensor<int, 2> cnt_inv({N, N}, (1 << (N-2)));

	std::vector<int> cur_res(1<<N);
	std::iota(cur_res.begin(), cur_res.end(), 0);

	std::vector<int64_t> dp(1<<N);
	auto run_dp = [&]() -> void {
		for (int i = (1<<N)-1; i >= 0; i--) {
			if (!(cur_res[i] & (cur_res[i]+1))) {
				int64_t res = 0;
				for (int v = (1 << N) - 1 - i; v > 0; v &= v-1) {
					int off = v & -v;
					res += dp[i + off];
				}
				if (i == (1 << N) - 1) res = 1;
				dp[i] = res;
			}
		}
	};
	run_dp();

	auto do_query = [&](int a, int b) -> int64_t {
		assert(0 <= a && a < b && b < N);
		if (cnt_inv[{a, b}] != 0) {
			for (int m = 0; m < (1 << N); m++) {
				if (!(cur_res[m] & (1 << a)) && (cur_res[m] & (1 << b))) {
					// a goes 0 -> 1, b goes 1 -> 0
					cur_res[m] ^= (1 << a) ^ (1 << b);
					cnt_inv[{a, b}]--;
					for (int c = 0; c < a; c++) {
						if (!(cur_res[m] & (1 << c))) {
							cnt_inv[{c, a}]++;
							cnt_inv[{c, b}]--;
						}
					}
					for (int c = a+1; c < b; c++) {
						if (!(cur_res[m] & (1 << c))) {
							cnt_inv[{c, b}]--;
						} else {
							cnt_inv[{a, c}]--;
						}
					}
					for (int c = b+1; c < N; c++) {
						if (cur_res[m] & (1 << c)) {
							cnt_inv[{b, c}]++;
							cnt_inv[{a, c}]--;
						}
					}
				}
			}
			run_dp();
		}
		return dp[0];
	};

	int64_t prvS = 1;

	for (int q = 0; q < Q; q++) {
		int a, b;
		{
			cin >> a >> b;
			a = int((a + prvS) % N);
			b = int((b + prvS * 2) % N);
			assert(a != b);
			if (a > b) std::swap(a, b);
			assert(0 <= a && a < b && b < N);
		}

		//cerr << "query " << a << ' ' << b << '\n';
		prvS = do_query(a, b);
		//cerr << prvS << '\n';
		cout << prvS << '\n';
	}

	return 0;
}
