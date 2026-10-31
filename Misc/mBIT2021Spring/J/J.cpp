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

#define nicememe main

int64_t cross3(std::pair<int, int64_t> a, std::pair<int, int64_t> b, std::pair<int, int64_t> c) {
	return (b.first - a.first) * (c.second - a.second) - (c.first - a.first) * (b.second - a.second);
}

int nicememe() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; int64_t D; cin >> N >> D;

	vector<int> A(N); for (auto& a : A) cin >> a;
	vector<int64_t> suff(N+1);
	for (int i = N-1; i >= 0; i--) suff[i] = suff[i+1] + A[i];

	const int64_t INF = 2e18;
	tensor<int64_t, 2> dp({N+1, N+1}, INF);
	for (int c = N; c >= 0; c--) {
		// (d-c) * (suff[b] + suff[c]) + (d+b) * (suff[d] + suff[b] + suff[c]) + (d-b) * (suff[d] + suff[c])
		// = (b * suff[b]) + (2 * d - c) * suff[b] + (d-c) * suff[c] + (2*d) * (suff[d] + suff[c])
		if (c == N) {
			dp[{c, c}] = 0;
		} else {
			int64_t v = INF;
			for (int d = c+1; d <= N; d++) {
				int coeff = 2 * d - c;
				int64_t val = dp[{c, d}] + (d - c) * suff[c] + (2 * d) * (suff[d] + suff[c]);
				v = min(v, c * suff[c] + coeff * suff[c] + val);
			}
			dp[{c, c}] = v;
		}
		vector<pair<int, int64_t>> lines;
		for (int d = c; d <= N; d++) {
			int coeff = 2 * d - c;
			int64_t val = dp[{c, d}] + (d - c) * suff[c] + (2 * d) * (suff[d] + suff[c]);
			while (lines.size() >= 2) {
				if (cross3(lines.end()[-2], lines.end()[-1], {coeff, val}) <= 0) {
					lines.pop_back();
				} else {
					break;
				}
			}
			lines.push_back({coeff, val});
		}

		for (int b = c-1; b >= 0; b--) {
			auto eval = [&](pair<int, int64_t> a) {
				return b * suff[b] + a.first * suff[b] + a.second;
			};
			while (lines.size() >= 2 && eval(lines.end()[-2]) <= eval(lines.end()[-1])) {
				lines.pop_back();
			}
			dp[{b, c}] = eval(lines.back());
		}
	}

	int64_t ans = dp[{0,0}];
	// cost for own exploration edges
	for (int i = 0; i < N; i++) {
		ans += 3 * suff[i];
	}
	int64_t g = std::gcd(ans, D);
	ans /= g, D /= g;
	cout << ans << '/' << D << '\n';
	return 0;
}
