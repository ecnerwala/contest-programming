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

	using surreal_t = int64_t;
	// fixed point with the point at 50
	const int FIX = 50;

	auto get_game = [&](surreal_t lo, surreal_t hi) -> surreal_t {
		assert(lo < hi);
		assert(hi - 1 != lo); // Otherwise, increase FIX
		if (lo < 0 && hi > 0) {
			return 0;
		} else {
			int len = 64 - __builtin_clzll((hi-1) ^ lo) - 1;
			if (len > FIX) {
				return ((lo >= 0) ? ((lo >> FIX) + 1) : ((hi-1) >> FIX)) << FIX;
			} else {
				return (hi-1) >> len << len;
			}
		}
	};

	const int N = 8;
	tensor<surreal_t, 2> val({(1<<N), (1<<N)});
	for (int mw = 0; mw < (1 << N); mw++) {
		for (int mb = 0; mb < (1 << N); mb++) {
			if (mw & mb) continue;

			const surreal_t INF = int64_t(1) << 62;
			surreal_t lo = -INF;
			surreal_t hi = INF;
			for (int i = 0; i < N; i++) {
				if (i > 0 && (mw & (1 << i)) && !((mw|mb) & (1 << (i-1)))) {
					lo = std::max(lo, val[{mw - (1 << (i-1)), mb}]);
				}
				if (mb & (1 << i)) {
					lo = std::max(lo, val[{mw, mb - (1 << i)}]);
				}

				if (i > 0 && (mb & (1 << i)) && !((mw|mb) & (1 << (i-1)))) {
					hi = std::min(hi, val[{mw, mb - (1 << (i-1))}]);
				}
				if (mw & (1 << i)) {
					hi = std::min(hi, val[{mw - (1 << i), mb}]);
				}
			}

			assert(lo < hi);


			// find the midpoint of lo and hi
			// you have to be strictly between lo and hi
			val[{mw, mb}] = get_game(lo, hi);
			if (false && mw < 16 && mb < 16) {
				for (int z = 0; z < N; z++) {
					if (mw & (1 << z)) cerr << 'W';
					else if (mb & (1 << z)) cerr << 'B';
					else cerr << '.';
				}
				cerr << ' ' << double(val[{mw, mb}]) * pow(2, -FIX) << '\n';
			}
		}
	}

	int A; cin >> A;
	std::vector<std::array<int, 2>> state(A);
	for (int i = 0; i < A; i++) {
		string s; cin >> s;
		for (int j = 0; j < A; j++) {
			if (s[j] == 'W') {
				state[j][0] |= (1 << i);
			} else if (s[j] == 'B') {
				state[j][1] |= (1 << i);
			} else if (s[j] == '.') {
			} else assert(false);
		}
	}
	surreal_t tot = 0;
	for (int i = 0; i < A; i++) {
		//cerr << double(val[state[i]]) / (int64_t(1) << FIX) << '\n';
		tot += val[state[i]];
	}
	if (tot > 0) {
		cout << "Takahashi" << '\n';
	} else {
		cout << "Snuke" << '\n';
	}

	return 0;
}
