#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

template <typename T> T mod_inv_in_range(T a, T m) {
	// assert(0 <= a && a < m);
	T x = a, y = m;
	T vx = 1, vy = 0;
	while (x) {
		T k = y / x;
		y %= x;
		vy -= k * vx;
		std::swap(x, y);
		std::swap(vx, vy);
	}
	assert(y == 1);
	return vy < 0 ? m + vy : vy;
}

template <typename T> T mod_inv(T a, T m) {
	a %= m;
	a = a < 0 ? a + m : a;
	return mod_inv_in_range(a, m);
}

template <int MOD_> struct modnum {
	static constexpr int MOD = MOD_;
	static_assert(MOD_ > 0, "MOD must be positive");

private:
	int v;

public:

	modnum() : v(0) {}
	modnum(int64_t v_) : v(int(v_ % MOD)) { if (v < 0) v += MOD; }
	explicit operator int() const { return v; }
	friend std::ostream& operator << (std::ostream& out, const modnum& n) { return out << int(n); }
	friend std::istream& operator >> (std::istream& in, modnum& n) { int64_t v_; in >> v_; n = modnum(v_); return in; }

	friend bool operator == (const modnum& a, const modnum& b) { return a.v == b.v; }
	friend bool operator != (const modnum& a, const modnum& b) { return a.v != b.v; }

	modnum inv() const {
		modnum res;
		res.v = mod_inv_in_range(v, MOD);
		return res;
	}
	friend modnum inv(const modnum& m) { return m.inv(); }
	modnum neg() const {
		modnum res;
		res.v = v ? MOD-v : 0;
		return res;
	}
	friend modnum neg(const modnum& m) { return m.neg(); }

	modnum operator- () const {
		return neg();
	}
	modnum operator+ () const {
		return modnum(*this);
	}

	modnum& operator ++ () {
		v ++;
		if (v == MOD) v = 0;
		return *this;
	}
	modnum& operator -- () {
		if (v == 0) v = MOD;
		v --;
		return *this;
	}
	modnum& operator += (const modnum& o) {
		v -= MOD-o.v;
		v = (v < 0) ? v + MOD : v;
		return *this;
	}
	modnum& operator -= (const modnum& o) {
		v -= o.v;
		v = (v < 0) ? v + MOD : v;
		return *this;
	}
	modnum& operator *= (const modnum& o) {
		v = int(int64_t(v) * int64_t(o.v) % MOD);
		return *this;
	}
	modnum& operator /= (const modnum& o) {
		return *this *= o.inv();
	}

	friend modnum operator ++ (modnum& a, int) { modnum r = a; ++a; return r; }
	friend modnum operator -- (modnum& a, int) { modnum r = a; --a; return r; }
	friend modnum operator + (const modnum& a, const modnum& b) { return modnum(a) += b; }
	friend modnum operator - (const modnum& a, const modnum& b) { return modnum(a) -= b; }
	friend modnum operator * (const modnum& a, const modnum& b) { return modnum(a) *= b; }
	friend modnum operator / (const modnum& a, const modnum& b) { return modnum(a) /= b; }
};

template <typename T> T pow(T a, long long b) {
	assert(b >= 0);
	T r = 1; while (b) { if (b & 1) r *= a; b >>= 1; a *= a; } return r;
}

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

class PreInPostOrder {

public:

	int reconstruct(vector <int> A, vector <int> B) {
		using num = modnum<int(1e9)+7>;
		int N = int(A.size());
		assert(N == int(B.size()));

		std::vector<int> invA(N);
		for (int i = 0; i < N; i++) invA[A[i]] = i;
		std::vector<int> invB(N);
		for (int i = 0; i < N; i++) invB[B[i]] = i;

		tensor<num, 3> dp_pip_ipp({N+1, N+1, N+1}, 0);
		tensor<num, 3> dp_ipp_ppi({N+1, N+1, N+1}, 0);
		tensor<num, 3> dp_ppi_pip({N+1, N+1, N+1}, 0);
		for (int i = 0; i <= N; i++) {
			for (int j = 0; j <= N; j++) {
				dp_pip_ipp[{0, i, j}] = 1;
				dp_ipp_ppi[{0, i, j}] = 1;
				dp_ppi_pip[{0, i, j}] = 1;
			}
		}

		for (int l = 1; l <= N; l++) {
			for (int i = 0; i+l <= N; i++) {
				for (int j = 0; j+l <= N; j++) {
					{
						// pip_ipp
						int v = A[i];
						int k = invB[v];
						if (j <= k && k < j+l) {
							int a = k-j;
							int b = j+l-k-1;
							dp_pip_ipp[{l, i, j}] = dp_ipp_ppi[{a, i+1, j}] * dp_ipp_ppi[{b, i+a+1, j+a+1}];
						}
					}
					{
						// ipp_ppi
						int v = B[j+l-1];
						int k = invA[v];
						if (i <= k && k < i+l) {
							int a = k-i;
							int b = i+l-k-1;
							dp_ipp_ppi[{l, i, j}] = dp_ppi_pip[{a, i, j}] * dp_ppi_pip[{b, i+a+1, j+a}];
						}
					}
					{
						// ppi_pip
						if (A[i+l-1] == B[j]) {
							for (int a = 0, b = l-1; b >= 0; a++, b--) {
								dp_ppi_pip[{l, i, j}] += dp_pip_ipp[{a, i, j+1}] * dp_pip_ipp[{b, i+a, j+a+1}];
							}
						}
					}
				}
			}
		}

		return int(dp_pip_ipp[{N, 0, 0}]);
	}
};

void my_tests() {
	// FILL IN TESTS
	int N = 70;
	std::vector<int> A(N);
	std::vector<int> B(N);
	std::iota(A.begin(), A.end(), 0);
	std::iota(B.begin(), B.end(), 0);
	cerr << PreInPostOrder().reconstruct(A, B) << '\n';
}

}  // namespace ecnerwala

namespace JeffreyHo {
class PreInPostOrder {
public:
	int reconstruct(vector<int> a, vector<int> b) {
		int n = (int)a.size(), d[77][77][3] = {}, c[77] = {}, e[77] = {};
		for (int i = 0; i < n; i++) c[b[i]] = i, e[a[i]] = i;
		for (int i = 0; i < n; i++) for (int j = 0; j <3; j++) d[i][i][j] = 1;
		for (int i = 1; i <= n; i++) for (int j = 0; j <3; j++) d[i][i-1][j] = 1;
		for (int i = 1; i < n; i++) for (int j = 0; j + i < n; j++) {
			int l = j, r = j + i;
			int u = 77, v = -1;
			for (int k = l; k <= r; k++) u = min(u, c[a[k]]), v = max(v, c[a[k]]);
			if (v - u + 1 != r - l + 1) continue;
			int w = -1;
			for (int k = u; k <= v; k++) if (b[k] == a[l]) w = k;
			int u2 = l, v2 = r+1;
			for (int k = u; k < w; k++) u2 = max(u2, e[b[k]]);
			for (int k = u; k < w; k++) 
		}
	}
}
}

using ecnerwala::PreInPostOrder;

// vim:ft=cpp
