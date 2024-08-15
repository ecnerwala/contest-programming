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

public:
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

template <typename T> T mod_inv_in_range(T a, T m) {
	// assert(0 <= a && a < m);
	T x = a, y = m;
	// coeff of a in x and y
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

template <typename T> struct extended_gcd_result {
	T gcd;
	T coeff_a, coeff_b;
};
template <typename T> extended_gcd_result<T> extended_gcd(T a, T b) {
	T x = a, y = b;
	// coeff of a and b in x and y
	T ax = 1, ay = 0;
	T bx = 0, by = 1;
	while (x) {
		T k = y / x;
		y %= x;
		ay -= k * ax;
		by -= k * bx;
		std::swap(x, y);
		std::swap(ax, ay);
		std::swap(bx, by);
	}
	return {y, ay, by};
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

/**
 * Author: Andrew He
 * Source: http://neerc.ifmo.ru/trains/toulouse/2017/fft2.pdf
 * Papers about accuracy: http://www.daemonology.net/papers/fft.pdf, http://www.cs.berkeley.edu/~fateman/papers/fftvsothers.pdf
 * For integers rounding works if $(|a| + |b|)\max(a, b) < \mathtt{\sim} 10^9$, or in theory maybe $10^6$.
 */

namespace ecnerwala {
namespace fft {

using std::swap;
using std::vector;
using std::min;
using std::max;

template<class T> int sz(T&& arg) { using std::size; return int(size(std::forward<T>(arg))); }
inline int nextPow2(int s) { return 1 << (s > 1 ? 32 - __builtin_clz(s-1) : 0); }

// Complex
template <typename dbl> struct cplx { /// start-hash
	dbl x, y;
	cplx(dbl x_ = 0, dbl y_ = 0) : x(x_), y(y_) { }
	friend cplx operator+(cplx a, cplx b) { return cplx(a.x + b.x, a.y + b.y); }
	friend cplx operator-(cplx a, cplx b) { return cplx(a.x - b.x, a.y - b.y); }
	friend cplx operator*(cplx a, cplx b) { return cplx(a.x * b.x - a.y * b.y, a.x * b.y + a.y * b.x); }
	friend cplx conj(cplx a) { return cplx(a.x, -a.y); }
	friend cplx inv(cplx a) { dbl n = (a.x*a.x+a.y*a.y); return cplx(a.x/n,-a.y/n); }
};

// getRoot implementations
template <typename num> struct getRoot {
	static num f(int k) = delete;
};
template <typename dbl> struct getRoot<cplx<dbl>> {
	static cplx<dbl> f(int k) {
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
		dbl a=2*M_PI/k;
		return cplx<dbl>(cos(a),sin(a));
	}
};
template <int MOD> struct primitive_root {
	static const int value;
};
template <> struct primitive_root<998244353> {
	static const int value = 3;
};
template <int MOD> struct getRoot<modnum<MOD>> {
	static modnum<MOD> f(int k) {
		assert((MOD-1)%k == 0);
		return pow(modnum<MOD>(primitive_root<MOD>::value), (MOD-1)/k);
	}
};

template <typename num> class fft {
	static vector<int> rev;
	static vector<num> rt;

public:
	static void init(int n);
	template <typename Iterator> static void go(Iterator begin, int n);

	static vector<num> scratch_a;
	static vector<num> scratch_b;
};

template <typename num> vector<int> fft<num>::rev;
template <typename num> vector<num> fft<num>::rt;
template <typename num> vector<num> fft<num>::scratch_a;
template <typename num> vector<num> fft<num>::scratch_b;

template <typename num> void fft<num>::init(int n) {
	if (n <= sz(rt)) return;
	rev.resize(n);
	for (int i = 0; i < n; i++) {
		rev[i] = (rev[i>>1] | ((i&1)*n)) >> 1;
	}
	rt.reserve(n);
	while (sz(rt) < 2 && sz(rt) < n) rt.push_back(num(1));
	for (int k = sz(rt); k < n; k *= 2) {
		rt.resize(2*k);
		num z = getRoot<num>::f(2*k);
		for (int i = k/2; i < k; i++) {
			rt[2*i] = rt[i], rt[2*i+1] = rt[i]*z;
		}
	}
}

template <typename num> template <typename Iterator> void fft<num>::go(Iterator begin, int n) {
	init(n);
	int s = __builtin_ctz(sz(rev)/n);
	for (int i = 0; i < n; i++) {
		if (i < (rev[i]>>s)) {
			swap(*(begin+i), *(begin+(rev[i]>>s)));
		}
	}
	for (int k = 1; k < n; k *= 2) {
		for (int i = 0; i < n; i += 2 * k) {
			Iterator it1 = begin + i, it2 = it1 + k;
			for (int j = 0; j < k; j++, ++it1, ++it2) {
				num t = rt[j+k] * *it2;
				*it2 = *it1 - t;
				*it1 = *it1 + t;
			}
		}
	}
}

template <typename num> struct fft_multiplier {
	template <typename IterA, typename IterB, typename IterOut>
	static void convolve_circular(int n, IterA ia, int sza, IterB ib, int szb, IterOut io, int szo) {
		vector<num>& fa = fft<num>::scratch_a;
		vector<num>& fb = fft<num>::scratch_b;

		assert((n & (n - 1)) == 0);
		if (n == 0) return;
		assert(sza <= n);
		assert(szb <= n);
		assert(szo <= n);

		if (sz(fa) < n) fa.resize(n);
		if (sz(fb) < n) fb.resize(n);
		fft<num>::init(n);

		copy(ia, ia+sza, fa.begin());
		fill(fa.begin()+sza, fa.begin()+n, num(0));
		copy(ib, ib+szb, fb.begin());
		fill(fb.begin()+szb, fb.begin()+n, num(0));
		fft<num>::go(fa.begin(), n);
		fft<num>::go(fb.begin(), n);
		num d = inv(num(n));
		for (int i = 0; i < n; i++) fa[i] = fa[i] * fb[i] * d;
		reverse(fa.begin()+1, fa.begin()+n);
		fft<num>::go(fa.begin(), n);

		copy(fa.begin(), fa.begin()+szo, io);
	}

	template <typename IterA, typename IterOut>
	static void square_circular(int n, IterA ia, int sza, IterOut io, int szo) {
		vector<num>& fa = fft<num>::scratch_a;

		assert((n & (n - 1)) == 0);
		if (n == 0) return;
		assert(sza <= n);
		assert(szo <= n);

		if (sz(fa) < n) fa.resize(n);
		fft<num>::init(n);

		copy(ia, ia+sza, fa.begin());
		fill(fa.begin()+sza, fa.begin()+n, num(0));
		fft<num>::go(fa.begin(), n);
		num d = inv(num(n));
		for (int i = 0; i < n; i++) fa[i] = fa[i] * fa[i] * d;
		reverse(fa.begin()+1, fa.begin()+n);
		fft<num>::go(fa.begin(), n);

		copy(fa.begin(), fa.begin()+szo, io);
	}

	template <typename IterA, typename IterB, typename IterOut>
	static void multiply(IterA ia, int sza, IterB ib, int szb, IterOut io) {
		vector<num>& fa = fft<num>::scratch_a;
		vector<num>& fb = fft<num>::scratch_b;

		if (sza == 0 || szb == 0) return;
		int s = sza + szb - 1;
		int n = nextPow2(s);
		if (sz(fa) < n) fa.resize(n);
		if (sz(fb) < n) fb.resize(n);
		fft<num>::init(n);

		bool did_cut = false;
		if (sza > 1 && szb > 1 && n == 2 * (s - 1)) {
			// we have exactly 1 wraparound, so let's just handle it explicitly to save a factor of 2
			// only do it if sza < s and szb < s so we don't have to wrap the inputs
			did_cut = true;
			n /= 2;
		}
		copy(ia, ia+sza, fa.begin());
		fill(fa.begin()+sza, fa.begin()+n, num(0));
		copy(ib, ib+szb, fb.begin());
		fill(fb.begin()+szb, fb.begin()+n, num(0));
		// used if did_cut
		num v_init; if (did_cut) { v_init = fa[0] * fb[0]; }
		fft<num>::go(fa.begin(), n);
		fft<num>::go(fb.begin(), n);
		num d = inv(num(n));
		for (int i = 0; i < n; i++) fa[i] = fa[i] * fb[i] * d;
		reverse(fa.begin()+1, fa.begin()+n);
		fft<num>::go(fa.begin(), n);
		if (did_cut) {
			fa[s-1] = std::exchange(fa[0], v_init) - v_init;
		}
		copy(fa.begin(), fa.begin()+s, io);
	}

	template <typename IterA, typename IterOut>
	static void square(IterA ia, int sza, IterOut io) {
		multiply<IterA, IterA, IterOut>(ia, sza, ia, sza, io);
	}
};

template <typename num>
struct fft_inverser {
	template <typename IterA, typename IterOut>
	static void inverse(IterA ia, int sza, IterOut io) {
		vector<num>& fa = fft<num>::scratch_a;
		vector<num>& fb = fft<num>::scratch_b;

		if (sza == 0) return;
		int s = nextPow2(sza) * 2;
		fft<num>::init(s);
		if (sz(fa) < s) fa.resize(s);
		if (sz(fb) < s) fb.resize(s);
		fb[0] = inv(*ia);
		for (int n = 1; n < sza; ) {
			fill(fb.begin() + n, fb.begin() + 4 * n, num(0));
			n *= 2;
			copy(ia, ia+min(n,sza), fa.begin());
			fill(fa.begin()+min(n,sza), fa.begin()+2*n, 0);
			fft<num>::go(fb.begin(), 2*n);
			fft<num>::go(fa.begin(), 2*n);
			num d = inv(num(2*n));
			for (int i = 0; i < 2*n; i++) fb[i] = fb[i] * (2 - fa[i] * fb[i]) * d;
			reverse(fb.begin()+1, fb.begin()+2*n);
			fft<num>::go(fb.begin(), 2*n);
		}
		copy(fb.begin(), fb.begin()+sza, io);
	}
};

template <typename dbl>
struct fft_double_multiplier {
	template <typename IterA, typename IterB, typename IterOut>
	static void multiply(IterA ia, int sza, IterB ib, int szb, IterOut io) {
		vector<cplx<dbl>>& fa = fft<cplx<dbl>>::scratch_a;
		vector<cplx<dbl>>& fb = fft<cplx<dbl>>::scratch_b;

		if (sza == 0 || szb == 0) return;
		int s = sza + szb - 1;
		int n = nextPow2(s);
		fft<cplx<dbl>>::init(n);
		if (sz(fa) < n) fa.resize(n);
		if (sz(fb) < n) fb.resize(n);

		fill(fa.begin(), fa.begin() + n, 0);
		{ auto it = ia; for (int i = 0; i < sza; ++i, ++it) fa[i].x = *it; }
		{ auto it = ib; for (int i = 0; i < szb; ++i, ++it) fa[i].y = *it; }
		fft<cplx<dbl>>::go(fa.begin(), n);
		for (auto& x : fa) x = x * x;
		for (int i = 0; i < n; ++i) fb[i] = fa[(n-i)&(n-1)] - conj(fa[i]);
		fft<cplx<dbl>>::go(fb.begin(), n);
		{ auto it = io; for (int i = 0; i < s; ++i, ++it) *it = fb[i].y / (4*n); }
	}

	template <typename IterA, typename IterOut>
	static void square(IterA ia, int sza, IterOut io) {
		multiply<IterA, IterA, IterOut>(ia, sza, ia, sza, io);
	}
};

template <typename mnum>
struct fft_mod_multiplier {
	template <typename IterA, typename IterB, typename IterOut>
	static void multiply(IterA ia, int sza, IterB ib, int szb, IterOut io) {
		using cnum = cplx<double>;
		vector<cnum>& fa = fft<cnum>::scratch_a;
		vector<cnum>& fb = fft<cnum>::scratch_b;

		if (sza == 0 || szb == 0) return;
		int s = sza + szb - 1;
		int n = nextPow2(s);
		fft<cnum>::init(n);
		if (sz(fa) < n) fa.resize(n);
		if (sz(fb) < n) fb.resize(n);

		{ auto it = ia; for (int i = 0; i < sza; ++i, ++it) fa[i] = cnum(int(*it) & ((1<<15)-1), int(*it) >> 15); }
		fill(fa.begin()+sza, fa.begin() + n, 0);
		{ auto it = ib; for (int i = 0; i < szb; ++i, ++it) fb[i] = cnum(int(*it) & ((1<<15)-1), int(*it) >> 15); }
		fill(fb.begin()+szb, fb.begin() + n, 0);

		fft<cnum>::go(fa.begin(), n);
		fft<cnum>::go(fb.begin(), n);
		double r0 = 0.5 / n; // 1/2n
		for (int i = 0; i <= n/2; i++) {
			int j = (n-i)&(n-1);
			cnum g0 = (fb[i] + conj(fb[j])) * r0;
			cnum g1 = (fb[i] - conj(fb[j])) * r0;
			swap(g1.x, g1.y); g1.y *= -1;
			if (j != i) {
				swap(fa[j], fa[i]);
				fb[j] = fa[j] * g1;
				fa[j] = fa[j] * g0;
			}
			fb[i] = fa[i] * conj(g1);
			fa[i] = fa[i] * conj(g0);
		}
		fft<cnum>::go(fa.begin(), n);
		fft<cnum>::go(fb.begin(), n);
		using ll = long long;
		const ll m = mnum::MOD;
		auto it = io;
		for (int i = 0; i < s; ++i, ++it) {
			*it = mnum((ll(fa[i].x+0.5)
						+ (ll(fa[i].y+0.5) % m << 15)
						+ (ll(fb[i].x+0.5) % m << 15)
						+ (ll(fb[i].y+0.5) % m << 30)) % m);
		}
	}

	template <typename IterA, typename IterOut>
	static void square(IterA ia, int sza, IterOut io) {
		multiply<IterA, IterA, IterOut>(ia, sza, ia, sza, io);
	}
};

template <class multiplier, typename num>
struct multiply_inverser {
	template <typename IterA, typename IterOut>
	static void inverse(IterA ia, int sza, IterOut io) {
		if (sza == 0) return;
		int s = nextPow2(sza);
		vector<num> b(s,num(0));
		vector<num> tmp(2*s);
		b[0] = inv(*ia);
		for (int n = 1; n < sza; ) {
			multiplier::square(b.begin(),n,tmp.begin());
			int nn = min(sza,2*n);
			multiplier::multiply(tmp.begin(),nn,ia,nn,tmp.begin());
			for (int i = n; i < nn; i++) b[i] = -tmp[i];
			n = nn;
		}
		copy(b.begin(), b.begin()+sza, io);
	}
};

template <class multiplier, typename T> vector<T> multiply(const vector<T>& a, const vector<T>& b) {
	if (sz(a) == 0 || sz(b) == 0) return {};
	vector<T> r(sz(a) + sz(b) - 1);
	multiplier::multiply(begin(a), sz(a), begin(b), sz(b), begin(r));
	return r;
}

template <typename T> vector<T> fft_multiply(const vector<T>& a, const vector<T>& b) {
	return multiply<fft_multiplier<T>, T>(a, b);
}
template <typename T> vector<T> fft_double_multiply(const vector<T>& a, const vector<T>& b) {
	return multiply<fft_double_multiplier<T>, T>(a, b);
}
template <typename T> vector<T> fft_mod_multiply(const vector<T>& a, const vector<T>& b) {
	return multiply<fft_mod_multiplier<T>, T>(a, b);
}

template <class multiplier, typename T> vector<T> square(const vector<T>& a) {
	if (sz(a) == 0) return {};
	vector<T> r(2 * sz(a) - 1);
	multiplier::square(begin(a), sz(a), begin(r));
	return r;
}
template <typename T> vector<T> fft_square(const vector<T>& a) {
	return square<fft_multiplier<T>, T>(a);
}
template <typename T> vector<T> fft_double_square(const vector<T>& a) {
	return square<fft_double_multiplier<T>, T>(a);
}
template <typename T> vector<T> fft_mod_square(const vector<T>& a) {
	return square<fft_mod_multiplier<T>, T>(a);
}

template <class inverser, typename T> vector<T> inverse(const vector<T>& a) {
	vector<T> r(sz(a));
	inverser::inverse(begin(a), sz(a), begin(r));
	return r;
}
template <typename T> vector<T> fft_inverse(const vector<T>& a) {
	return inverse<fft_inverser<T>, T>(a);
}
template <typename T> vector<T> fft_double_inverse(const vector<T>& a) {
	return inverse<multiply_inverser<fft_double_multiplier<T>, T>, T>(a);
}
template <typename T> vector<T> fft_mod_inverse(const vector<T>& a) {
	return inverse<multiply_inverser<fft_mod_multiplier<T>, T>, T>(a);
}
/* namespace fft */ }

// Power series; these are assumed to be the min of the length
template <typename T, typename multiplier, typename inverser>
struct power_series : public std::vector<T> {
	using std::vector<T>::vector;

	int ssize() const {
		return int(this->size());
	}
	int len() const {
		return ssize();
	}
	int degree() const {
		return len() - 1;
	}
	void extend(int sz) {
		assert(sz >= ssize());
		this->resize(sz);
	}
	void shrink(int sz) {
		assert(sz <= ssize());
		this->resize(sz);
	}
	// multiply by x^n
	void shift(int n = 1) {
		assert(n >= 0 && n <= ssize());
		std::rotate(this->begin(), this->end()-n, this->end());
		std::fill(this->begin(), this->begin()+n, T(0));
	}
	// divide by x^n and 0-pad
	void unshift(int n = 1) {
		assert(n >= 0 && n <= ssize());
		std::fill(this->begin(), this->begin()+n, T(0));
		std::rotate(this->begin(), this->begin()+n, this->end());
	}
	power_series& operator += (const power_series& o) {
		assert(len() == o.len());
		for (int i = 0; i < int(o.size()); i++) {
			(*this)[i] += o[i];
		}
		return *this;
	}
	friend power_series operator + (const power_series& a, const power_series& b) {
		power_series r(std::min(a.size(), b.size()));
		for (int i = 0; i < r.len(); i++) {
			r[i] = a[i] + b[i];
		}
		return r;
	}
	power_series& operator -= (const power_series& o) {
		assert(len() == o.len());
		for (int i = 0; i < int(o.size()); i++) {
			(*this)[i] -= o[i];
		}
		return *this;
	}
	friend power_series operator - (const power_series& a, const power_series& b) {
		power_series r(std::min(a.size(), b.size()));
		for (int i = 0; i < r.len(); i++) {
			r[i] = a[i] - b[i];
		}
		return r;
	}

	power_series& operator *= (const T& n) {
		for (auto& v : *this) v *= n;
		return *this;
	}
	friend power_series operator * (const power_series& a, const T& n) {
		power_series r(a.size());
		for (int i = 0; i < a.len(); i++) {
			r[i] = a[i] * n;
		}
		return r;
	}
	friend power_series operator * (const T& n, const power_series& a) {
		power_series r(a.size());
		for (int i = 0; i < a.len(); i++) {
			r[i] = n * a[i];
		}
		return r;
	}

	friend power_series operator * (const power_series& a, const power_series& b) {
		if (sz(a) == 0 || sz(b) == 0) return {};
		power_series r(std::max(0, sz(a) + sz(b) - 1));
		multiplier::multiply(begin(a), sz(a), begin(b), sz(b), begin(r));
		r.resize(std::min(a.size(), b.size()));
		return r;
	}
	power_series& operator *= (const power_series& o) {
		return *this = (*this) * o;
	}
	friend power_series square(const power_series& a) {
		if (sz(a) == 0) return {};
		power_series r(sz(a) * 2 - 1);
		multiplier::square(begin(a), sz(a), begin(r));
		r.resize(a.size());
		return r;
	}

	friend power_series stretch(const power_series& a, int n) {
		power_series r(a.size());
		for (int i = 0; i*n < int(a.size()); i++) {
			r[i*n] = a[i];
		}
		return r;
	}
	friend power_series inverse(power_series a) {
		power_series r(sz(a));
		inverser::inverse(begin(a), sz(a), begin(r));
		return r;
	}
	friend power_series deriv_shift(power_series a) {
		for (int i = 0; i < a.len(); i++) {
			a[i] *= i;
		}
		return a;
	}
	friend power_series integ_shift(power_series a) {
		assert(a[0] == 0);
		T f = 1;
		for (int i = 1; i < int(a.size()); i++) {
			a[i] *= f;
			f *= i;
		}
		f = inv(f);
		for (int i = int(a.size()) - 1; i > 0; i--) {
			a[i] *= f;
			f *= i;
		}
		return a;
	}
	friend power_series deriv_shift_log(power_series a) {
		auto r = deriv_shift(a);
		return r * inverse(a);
	}
	friend power_series poly_log(power_series a) {
		assert(a[0] == 1);
		return integ_shift(deriv_shift_log(std::move(a)));
	}
	friend power_series poly_exp(power_series a) {
		assert(a.size() >= 1);
		assert(a[0] == 0);
		power_series r(1, T(1));
		while (r.size() < a.size()) {
			int n_sz = std::min(int(r.size()) * 2, int(a.size()));
			r.resize(n_sz);
			power_series v(a.begin(), a.begin() + n_sz);
			v -= poly_log(r);
			v[0] += 1;
			r *= v;
		}
		return r;
	}
	friend power_series poly_pow_monic(power_series a, int64_t k) {
		if (a.empty()) return a;
		assert(a.size() >= 1);
		assert(a[0] == 1);
		a = poly_log(a);
		a *= k;
		return poly_exp(a);
	}
	friend power_series poly_pow(power_series a, int64_t k) {
		int st = 0;
		while (st < a.len() && a[st] == 0) st++;
		if (st == a.len()) return a;

		power_series r(a.begin() + st, a.end());
		T leading_coeff = r[0];
		r *= inv(leading_coeff);
		r = poly_pow_monic(r, k);
		r *= pow(leading_coeff, k);
		r.insert(r.begin(), size_t(st), T(0));
		return r;
	}

	friend power_series to_newton_sums(const power_series& a, int deg) {
		auto r = log_deriv_shift(a);
		r[0] = deg;
		for (int i = 1; i < int(r.size()); i++) r[i] = -r[i];
		return r;
	}
	friend power_series from_newton_sums(power_series S, int deg) {
		assert(S[0] == int(deg));
		S[0] = 0;
		for (int i = 1; i < int(S.size()); i++) S[i] = -S[i];
		return poly_exp(integ_shift(std::move(S)));
	}

	// Calculates prod 1/(1-x^i)^{a[i]}
	friend power_series euler_transform(const power_series& a) {
		power_series r = deriv_shift(a);
		std::vector<bool> is_prime(a.size(), true);
		for (int p = 2; p < int(a.size()); p++) {
			if (!is_prime[p]) continue;
			for (int i = 1; i*p < int(a.size()); i++) {
				r[i*p] += r[i];
				is_prime[i*p] = false;
			}
		}
		return poly_exp(integ_shift(r));
	}
	friend power_series inverse_euler_transform(const power_series& a) {
		power_series r = deriv_shift(poly_log(a));
		std::vector<bool> is_prime(a.size(), true);
		for (int p = 2; p < int(a.size()); p++) {
			if (!is_prime[p]) continue;
			for (int i = (int(a.size())-1)/p; i >= 1; i--) {
				r[i*p] -= r[i];
				is_prime[i*p] = false;
			}
		}
		return integ_shift(r);
	}
};

template <typename num> using power_series_fft = power_series<num, fft::fft_multiplier<num>, fft::fft_inverser<num>>;
template <typename num, typename multiplier> using power_series_with_multiplier = power_series<num, multiplier, fft::multiply_inverser<multiplier, num>>;
template <typename num> using power_series_fft_mod = power_series_with_multiplier<num, fft::fft_mod_multiplier<num>>;
template <typename num> using power_series_fft_double = power_series_with_multiplier<num, fft::fft_double_multiplier<num>>;

// TODO: Use iterator traits to deduce value type?
template <typename base_iterator, typename value_type> struct add_into_iterator {
	base_iterator base;
	add_into_iterator() : base() {}
	add_into_iterator(base_iterator b) : base(b) {}
	add_into_iterator& operator * () { return *this; }
	add_into_iterator& operator ++ () { base.operator ++ (); return *this; }
	add_into_iterator& operator ++ (int) { auto temp = *this; operator ++ (); return temp; }
	auto operator = (value_type v) { base.operator * () += v; }
};

// TODO: Use iterator traits to deduce value type?
template <typename base_iterator, typename value_type> struct add_double_into_iterator {
	base_iterator base;
	add_double_into_iterator() : base() {}
	add_double_into_iterator(base_iterator b) : base(b) {}
	add_double_into_iterator& operator * () { return *this; }
	add_double_into_iterator& operator ++ () { base.operator ++ (); return *this; }
	add_double_into_iterator& operator ++ (int) { auto temp = *this; operator ++ (); return temp; }
	auto operator = (value_type v) { base.operator * () += 2 * v; }
};

template <typename num, typename multiplier> struct online_multiplier {
	int N; int i;
	std::vector<num> f, g;
	std::vector<num> res;

	// Computes the first 2N terms of the product
	online_multiplier(int N_) : N(N_), i(0), f(N), g(N), res(2*N+1, num(0)) {}

	num peek() {
		return res[i];
	}

	void push(num v_f, num v_g) {
		assert(i < N);
		f[i] = v_f;
		g[i] = v_g;
		if (i == 0) {
			res[i] += v_f * v_g;
		} else {
			res[i] += v_f * g[0];
			res[i] += f[0] * v_g;
			// TODO: We could do this second half more lazily, since it only affects res[i+1]...
			for (int p = 1; (i & (p-1)) == (p-1); p <<= 1) {
				int lo1 = p;
				int lo2 = i + 1 - p;
				multiplier::multiply(
					// TODO: We can cache FFT([f,g].begin() + p, p)
					f.begin() + lo1, p,
					g.begin() + lo2, p,
					add_into_iterator<decltype(res.begin()), num>(res.begin() + lo1 + lo2)
				);
				if (i == 2*p-1) break;
				// TODO: Don't recompute if squaring
				multiplier::multiply(
					f.begin() + lo2, p,
					g.begin() + lo1, p,
					add_into_iterator<decltype(res.begin()), num>(res.begin() + lo1 + lo2)
				);
			}
		}
		i++;
	}

	num back() {
		return res[i-1];
	}
};

template <typename num, typename multiplier> struct online_squarer {
	int N; int i;
	std::vector<num> f;
	std::vector<num> res;

	// Computes the first 2N terms of the product
	online_squarer(int N_) : N(N_), i(0), f(N), res(2*N+1, num(0)) {}

	num peek() {
		return res[i];
	}

	void push(num v_f) {
		assert(i < N);
		f[i] = v_f;
		if (i == 0) {
			res[i] += v_f * v_f;
		} else {
			res[i] += 2 * v_f * f[0];
			// TODO: We could do this second half more lazily, since it only affects res[i+1]...
			for (int p = 1; (i & (p-1)) == (p-1); p <<= 1) {
				int lo1 = p;
				int lo2 = i + 1 - p;
				if (i == 2*p-1) {
					multiplier::square(
						// TODO: We can cache FFT([f,g].begin() + p, p)
						f.begin() + lo1, p,
						add_into_iterator<decltype(res.begin()), num>(res.begin() + lo1 + lo2)
					);
					break;
				} else {
					multiplier::multiply(
						// TODO: Use cached FFT
						f.begin() + lo1, p,
						f.begin() + lo2, p,
						add_double_into_iterator<decltype(res.begin()), num>(res.begin() + lo1 + lo2)
					);
				}
			}
		}
		i++;
	}

	num back() {
		return res[i-1];
	}
};

/* namespace ecnerwala */ }

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	using num = modnum<int(1e9)+7>;
	const int N = 130;
	const int M = 30;

	// the main dp is:
	// M, length, cost, max surplus, min surplus without paying
	tensor<num, 3> ans({M+1, N+1, N/2 + 1});
	ans[{0, 0, 0}] = num(1);
	for (int m = 1; m <= M; m++) {
		cerr << "M = " << m << '\n';
		tensor<num, 2> cur_dp_lo({N+1, N/2 + 1});
		tensor<num, 2> cur_dp_hi({N+1, N/2 + 1});
		// ring buffer mod 3
		tensor<num, 3> fail_dp({3, N+1, N/2 + 1});
		for (int l1 = 0; l1 <= N; l1++) {
			for (int k = 0; k <= l1/2; k++) {
				cur_dp_lo[{l1, k}] += ans[{m-1, l1, k}];
				cur_dp_hi[{l1, 0}] += ans[{m-1, l1, k}];
			}
		}
		for (int c0 = 0; c0 <= N; c0++) {
			for (int l0 = c0; l0 <= N; l0++) {
				for (int k1 = 0; k1 <= (l0 - c0) / 2; k1++) {
					num w = std::exchange(fail_dp[{c0 % 3, l0, k1}], num(0));
					cur_dp_lo[{l0, (l0 - c0) / 2}] += w;
					cur_dp_hi[{l0, k1}] += w;
				}
			}

			// update ans
			for (int l0 = c0; l0 <= N; l0++) {
				for (int k0 = 0; k0 <= (l0 - c0) / 2; k0++) {
					num w = cur_dp_lo[{l0, k0}];
					if ((l0 - c0) & 1) {
						ans[{m, l0, (l0 - c0 - 1) / 2}] += w;
					} else if (k0 == (l0 - c0) / 2) {
						ans[{m, l0, (l0 - c0) / 2}] += w;
					} else {
						ans[{m, l0, (l0 - c0 - 2) / 2}] += w;
					}
				}
			}

			tensor<num, 2> nxt_dp_lo({N+1, N/2+1});
			tensor<num, 2> nxt_dp_hi({N+1, N/2+1});

			// hi -> lo
			// TODO: This is 2D convolution, could optimize down
			int mx = N - c0 - 1;
			std::vector<num> lhs((mx + 1) * (mx + 1));
			std::vector<num> rhs((mx + 1) * (mx + 1));
			for (int l0 = 0; l0 <= mx; l0++) {
				for (int k1 = 0; k1 <= l0 / 2; k1++) {
					lhs[l0 * (mx + 1) + (l0 - k1)] += cur_dp_hi[{l0 + c0, k1}];
				}
			}
			for (int l1 = 0; l1 <= mx; l1++) {
				for (int k = 0; k <= l1 / 2; k++) {
					rhs[l1 * (mx + 1) + k] += ans[{m-1, l1, k}];
				}
			}
			auto prod = ecnerwala::fft::fft_mod_multiply(lhs, rhs);
			for (int nl = 0; nl <= mx; nl++) {
				for (int nk = 0; nk <= nl; nk++) {
					nxt_dp_lo[{nl + c0 + 1, std::min(nk, nl / 2)}] += prod[nl * (mx + 1) + nk];
				}
			}

			// common case for lo -> hi, just convolve with powers of m-1
			for (int l0 = c0; l0 <= N; l0++) {
				for (int k0 = 0; k0 <= (l0 - c0) / 2; k0++) {
					num w = cur_dp_lo[{l0, k0}] * pow(num(m-1), l0 - c0 - 2 * k0);
					int e = l0 + 1 + l0 - c0 - 2 * k0;
					if (e <= N) nxt_dp_hi[{e, (e-c0-1-0)/2}] += w;
				}
			}
			for (int nl = c0 + 1; nl + 1 <= N; nl++) {
				for (int nk = 0; nk <= (nl - c0 - 1) / 2; nk++) {
					nxt_dp_hi[{nl + 1, nk}] = nxt_dp_hi[{nl, nk}] * num(m-1);
				}
			}

			// case where l1 < a0, and we need to increase the cost and also subtract it from nxt_dp_lo
			for (int l0 = c0 + 1; l0 <= N; l0++) {
				// could optimize further w/ prefix/suffix sums, but don't really need to, this is already M * N^4
				int sgn = (l0 - c0) & 1;
				num w = 0;
				for (int k0 = 0; k0 <= (l0 - c0) / 2; k0++) {
					w += cur_dp_lo[{l0, k0}];
				}
				num o = 1;
				for (int l1 = 0; l1 < l0 - c0 && l0 + 1 + l1 <= N; l1++, o *= num(m-1)) {
					if ((l1 & 1) == sgn) w -= cur_dp_lo[{l0, (l0 - c0 - l1) / 2}];
					int nl = l0 + l1 + 1;
					int nc = c0 + 1;
					int na1 = l1 - !sgn;
					nc++;
					if (na1 < 0) {
						assert(l1 == 0);
						assert(!sgn);
						na1 = l1;
						nc++;
					}
					//int na0 = na1 & 1;
					fail_dp[{nc % 3, nl, (nl - nc - na1) / 2}] += w * o;
					// subtract from nxt_dp_lo
					nxt_dp_lo[{nl, (nl - c0 - 1) / 2}] -= w * o;
				}
			}


			cur_dp_lo = std::move(nxt_dp_lo);
			cur_dp_hi = std::move(nxt_dp_hi);
		}
		//for (int i = 0; i <= N; i++) { for (int c = 0; c <= i; c++) { cerr << ans[{m, i, c}] << ' '; } cerr << '\n'; }
	}

	int T; cin >> T;
	while (T--) {
		int n, m; cin >> n >> m;
		for (int k = 0; k <= (n-1)/2; k++) {
			cout << ans[{m, n, k}] << " \n"[k == (n-1)/2];
		}
	}

	return 0;
}

// The main DP is:
