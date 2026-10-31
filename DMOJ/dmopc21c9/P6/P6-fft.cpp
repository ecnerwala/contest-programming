#include <bits/stdc++.h>

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

template <int& MOD_> struct modnum {
	static constexpr int& MOD = MOD_;
	//static_assert(MOD_ > 0, "MOD must be positive");

private:
	using ll = long long;

	int v;

public:

	modnum() : v(0) {}
	modnum(ll v_) : v(int(v_ % MOD)) { if (v < 0) v += MOD; }
	explicit operator int() const { return v; }
	friend std::ostream& operator << (std::ostream& out, const modnum& n) { return out << int(n); }
	friend std::istream& operator >> (std::istream& in, modnum& n) { ll v_; in >> v_; n = modnum(v_); return in; }

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
		v = int(ll(v) * ll(o.v) % MOD);
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
template <int& MOD> struct getRoot<modnum<MOD>> {
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

template <typename num> vector<int> fft<num>::rev({0,1});
template <typename num> vector<num> fft<num>::rt(2, num(1));
template <typename num> vector<num> fft<num>::scratch_a;
template <typename num> vector<num> fft<num>::scratch_b;

template <typename num> void fft<num>::init(int n) {
	if (n <= sz(rt)) return;
	rev.resize(n);
	for (int i = 0; i < n; i++) {
		rev[i] = (rev[i>>1] | ((i&1)*n)) >> 1;
	}
	rt.reserve(n);
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
	static void multiply(IterA ia, int sza, IterB ib, int szb, IterOut io) {
		vector<num>& fa = fft<num>::scratch_a;
		vector<num>& fb = fft<num>::scratch_b;

		if (sza == 0 || szb == 0) return;
		int s = sza + szb - 1;
		int n = nextPow2(s);
		fft<num>::init(n);
		if (sz(fa) < n) fa.resize(n);
		if (sz(fb) < n) fb.resize(n);
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
		copy(fa.begin(), fa.begin()+s, io);
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
			// TODO: could be square instead of multiply
			multiplier::multiply(b.begin(),n,b.begin(),n,tmp.begin());
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
	vector<T> r(max(0, sz(a) + sz(b) - 1));
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

}} // namespace ecnerwala::fft

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, R; cin >> N >> R;
	static int MOD; cin >> MOD;
	using num = modnum<MOD>;

	std::vector<num> fact(N+1);
	fact[0] = 1;
	for (int i = 1; i <= N; i++) fact[i] = fact[i-1] * num(i);

	std::vector<num> ifact(N+1);
	ifact[N] = inv(fact[N]);
	for (int i = N; i >= 1; i--) ifact[i-1] = ifact[i] * num(i);

	tensor<std::vector<num>, 3> dp({N+1, N+1, 2});
	dp[{1, 1, 0}] = std::vector<num>({num(1)});
	dp[{1, 1, 1}] = std::vector<num>({num(0)});

	std::vector<std::vector<num>> pow_1x(N+1);
	pow_1x[0] = std::vector<num>({num(1)});
	for (int i = 1; i <= N; i++) {
		pow_1x[i] = pow_1x[i-1];
		pow_1x[i].push_back(num(0));
		for (int z = i; z > 0; z--) {
			pow_1x[i][z] += pow_1x[i][z-1];
		}
		//for (int z = 0; z <= i; z++) { cerr << pow_1x[i][z] << ' '; } cerr << '\n';
	}

	std::vector<std::vector<num>> choose_internal_edges(N+1);
	choose_internal_edges[1] = std::vector<num>({num(1)});
	for (int i = 2; i <= N; i++) {
		choose_internal_edges[i] = ecnerwala::fft::fft_mod_multiply(choose_internal_edges[i-1], pow_1x[i-1]);
	}

	for (int c = 1; c <= N; c++) {
		for (int a = 1; a <= c; a++) {
			for (int z = 0; z < 2; z++) {
				dp[{c, a, z}].resize(c * (c-1) / 2 - a * (a-1) / 2 + 1);
			}
		}
	}

	for (int c = 1; c <= N; c++) {
		cerr << "c = " << c << '\n';
		for (int a = 1; a <= c; a++) {

			for (int e = c-1; e < int(dp[{c, a, 0}].size()); e++) {
				dp[{c, a, 1}][e] += dp[{c, a, 0}][e] * num(N - c);
			}

			dp[{c, a, 0}] = ecnerwala::fft::fft_mod_multiply(dp[{c, a, 0}], choose_internal_edges[a]);
			dp[{c, a, 1}] = ecnerwala::fft::fft_mod_multiply(dp[{c, a, 1}], choose_internal_edges[a]);

			cerr << "c = " << c << " a = " << a << ' ' << dp[{c, a, 0}].size() << '\n';
			/*
			for (int z = 0; z < 2; z++) {
				for (int e = 0; e < int(dp[{c, a, z}].size()); e++) {
					cerr << dp[{c, a, z}][e] << ' ';
				}
				cerr << '\n';
			}
			*/

			for (int b = 1; b <= N-c; b++) {
				// just modify dp[{c, a, *, *}] in place with the extra edge counts
				for (int z = 0; z < 2; z++) {
					const auto& cdp = dp[{c, a, z}];
					auto ndp = ecnerwala::fft::fft_mod_multiply(cdp, pow_1x[a]);
					//std::vector<num> ndp(cdp.size() + a);
					for (int e = 0; e < int(cdp.size()); e++) {
						ndp[e] -= cdp[e];
					}

					dp[{c, a, z}] = std::move(ndp);

					for (int e = 0; e < int(dp[{c, a, z}].size()); e++) {
						dp[{c+b, b, z}][e] += dp[{c, a, z}][e] * ifact[b];
					}
				}
			}
		}
	}

	std::vector<num> ans(N * (N-1) / 2 + 1);
	{
		int c = N;
		for (int a = 1; a <= c; a++) {
			//cerr << int(dp[{c, a, 1}].size()) << '\n';
			for (int e = c-1; e < int(dp[{c, a, 1}].size()); e++) {
				ans[e] += dp[{c, a, 1}][e];
			}
		}
	}
	for (int e = N-1; e <= R; e++) {
		ans[e] *= fact[N-1];
		ans[e] /= num(N-1);
		cout << ans[e] << " \n"[e==R];
	}

	return 0;
}
