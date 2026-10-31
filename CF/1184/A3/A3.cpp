#include<bits/stdc++.h>
using namespace std;

/**
 * Author: Andrew He
 * Description: FFT/NTT, polynomial mod/log/exp
 * Source: http://neerc.ifmo.ru/trains/toulouse/2017/fft2.pdf
 * Papers about accuracy: http://www.daemonology.net/papers/fft.pdf, http://www.cs.berkeley.edu/~fateman/papers/fftvsothers.pdf
 * For integers rounding works if $(|a| + |b|)\max(a, b) < \mathtt{\sim} 10^9$, or in theory maybe $10^6$.
 */

#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define per(i, a, b) for(int i = (b)-1; i >= (a); --i)
#define trav(a, x) for(auto& a : x)
#define all(x) x.begin(), x.end()
#define sz(x) (int)(x).size()
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

const double PI = 2 * atan2(1, 0);

namespace fft {
#define FFT 1
#if FFT
// FFT
using dbl = double;
struct num { /// start-hash
	dbl x, y;
	num(dbl x_ = 0, dbl y_ = 0) : x(x_), y(y_) { }
};
inline num operator+(num a, num b) { return num(a.x + b.x, a.y + b.y); }
inline num operator-(num a, num b) { return num(a.x - b.x, a.y - b.y); }
inline num operator*(num a, num b) { return num(a.x * b.x - a.y * b.y, a.x * b.y + a.y * b.x); }
inline num conj(num a) { return num(a.x, -a.y); }
inline num inv(num a) { dbl n = (a.x*a.x+a.y*a.y); return num(a.x/n,-a.y/n); }
/// end-hash
#else
// NTT
const int mod = 998244353, g = 3;
// For p < 2^30 there is also (5 << 25, 3), (7 << 26, 3),
// (479 << 21, 3) and (483 << 21, 5). Last two are > 10^9.
struct num { /// start-hash
	int v;
	num(ll v_ = 0) : v(int(v_ % mod)) { if (v<0) v+=mod; }
	explicit operator int() const { return v; }
};
inline num operator+(num a,num b){return num(a.v+b.v);}
inline num operator-(num a,num b){return num(a.v+mod-b.v);}
inline num operator*(num a,num b){return num(1ll*a.v*b.v);}
inline num pow(num a, int b) {
	num r = 1;
	do{if(b&1)r=r*a;a=a*a;}while(b>>=1);
	return r;
}
inline num inv(num a) { return pow(a, mod-2); }
/// end-hash
#endif

using vn = vector<num>;
vi rev({0, 1});
vn rt(2, num(1)), fa, fb;

inline void init(int n) { /// start-hash
	if (n <= sz(rt)) return;
	rev.resize(n);
	rep(i,0,n) rev[i] = (rev[i>>1] | ((i&1)*n)) >> 1;
	rt.reserve(n);
	for (int k = sz(rt); k < n; k *= 2) {
		rt.resize(2*k);
#if FFT
		double a=PI/k; num z(cos(a),sin(a)); // FFT
#else
		num z = pow(num(g), (mod-1)/(2*k)); // NTT
#endif
		rep(i,k/2,k) rt[2*i] = rt[i], rt[2*i+1] = rt[i]*z;
	}
} /// end-hash

inline void fft(vector<num> &a, int n) { /// start-hash
	init(n);
	int s = __builtin_ctz(sz(rev)/n);
	rep(i,0,n) if (i < rev[i]>>s) swap(a[i], a[rev[i]>>s]);
	for (int k = 1; k < n; k *= 2)
		for (int i = 0; i < n; i += 2 * k) rep(j,0,k) {
			num t = rt[j+k] * a[i+j+k];
			a[i+j+k] = a[i+j] - t;
			a[i+j] = a[i+j] + t;
		}
} /// end-hash

// Complex/NTT
vn multiply(vn a, vn b) { /// start-hash
	int s = sz(a) + sz(b) - 1;
	if (s <= 0) return {};
	int L = s > 1 ? 32 - __builtin_clz(s-1) : 0, n = 1 << L;
	a.resize(n), b.resize(n);
	fft(a, n);
	fft(b, n);
	num d = inv(num(n));
	rep(i,0,n) a[i] = a[i] * b[i] * d;
	reverse(a.begin()+1, a.end());
	fft(a, n);
	a.resize(s);
	return a;
} /// end-hash

// Complex/NTT power-series inverse
// Doubles b as b[:n] = (2 - a[:n] * b[:n/2]) * b[:n/2]
vn inverse(const vn& a) { /// start-hash
	if (a.empty()) return {};
	vn b({inv(a[0])});
	b.reserve(2*a.size());
	while (sz(b) < sz(a)) {
		int n = 2*sz(b);
		b.resize(2*n, 0);
		if (sz(fa) < 2*n) fa.resize(2*n);
		fill(fa.begin(), fa.begin()+2*n, 0);
		copy(a.begin(), a.begin()+min(n,sz(a)), fa.begin());
		fft(b, 2*n);
		fft(fa, 2*n);
		num d = inv(num(2*n));
		rep(i, 0, 2*n) b[i] = b[i] * (2 - fa[i] * b[i]) * d;
		reverse(b.begin()+1, b.end());
		fft(b, 2*n);
		b.resize(n);
	}
	b.resize(a.size());
	return b;
} /// end-hash

#if FFT
// Double multiply (num = complex)
using vd = vector<double>;
vd multiply(const vd& a, const vd& b) { /// start-hash
	int s = sz(a) + sz(b) - 1;
	if (s <= 0) return {};
	int L = s > 1 ? 32 - __builtin_clz(s-1) : 0, n = 1 << L;
	if (sz(fa) < n) fa.resize(n);
	if (sz(fb) < n) fb.resize(n);

	fill(fa.begin(), fa.begin() + n, 0);
	rep(i,0,sz(a)) fa[i].x = a[i];
	rep(i,0,sz(b)) fa[i].y = b[i];
	fft(fa, n);
	trav(x, fa) x = x * x;
	rep(i,0,n) fb[i] = fa[(n-i)&(n-1)] - conj(fa[i]);
	fft(fb, n);
	vd r(s);
	rep(i,0,s) r[i] = fb[i].y / (4*n);
	return r;
} /// end-hash

// Integer multiply mod m (num = complex) /// start-hash
vi multiply_mod(const vi& a, const vi& b, int m) {
	int s = sz(a) + sz(b) - 1;
	if (s <= 0) return {};
	int L = s > 1 ? 32 - __builtin_clz(s-1) : 0, n = 1 << L;
	if (sz(fa) < n) fa.resize(n);
	if (sz(fb) < n) fb.resize(n);

	rep(i,0,sz(a)) fa[i] = num(a[i] & ((1<<15)-1), a[i] >> 15);
	fill(fa.begin()+sz(a), fa.begin() + n, 0);
	rep(i,0,sz(b)) fb[i] = num(b[i] & ((1<<15)-1), b[i] >> 15);
	fill(fb.begin()+sz(b), fb.begin() + n, 0);

	fft(fa, n);
	fft(fb, n);
	double r0 = 0.5 / n; // 1/2n
	rep(i,0,n/2+1) {
		int j = (n-i)&(n-1);
		num g0 = (fb[i] + conj(fb[j])) * r0;
		num g1 = (fb[i] - conj(fb[j])) * r0;
		swap(g1.x, g1.y); g1.y *= -1;
		if (j != i) {
			swap(fa[j], fa[i]);
			fb[j] = fa[j] * g1;
			fa[j] = fa[j] * g0;
		}
		fb[i] = fa[i] * conj(g1);
		fa[i] = fa[i] * conj(g0);
	}
	fft(fa, n);
	fft(fb, n);
	vi r(s);
	rep(i,0,s) r[i] = int((ll(fa[i].x+0.5)
				+ (ll(fa[i].y+0.5) % m << 15)
				+ (ll(fb[i].x+0.5) % m << 15)
				+ (ll(fb[i].y+0.5) % m << 30)) % m);
	return r;
} /// end-hash
#endif

} // namespace fft

int MOD;
template <int& MOD_> struct modnum {
	static constexpr int& MOD = MOD_;
	//static_assert(MOD_ > 0, "MOD must be positive");

private:
	using ll = long long;

	int v;

	static int minv(int a, int m) {
		a %= m;
		assert(a);
		return a == 1 ? 1 : int(m - ll(minv(m, a)) * ll(m) / a);
	}

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
		res.v = minv(v, MOD);
		return res;
	}
	modnum neg() const {
		modnum res;
		res.v = v ? MOD-v : 0;
		return res;
	}

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
		v += o.v;
		if (v >= MOD) v -= MOD;
		return *this;
	}
	modnum& operator -= (const modnum& o) {
		v -= o.v;
		if (v < 0) v += MOD;
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

// For multiply_mod, use num = modnum, poly = vector<num>
using num = modnum<MOD>;
using poly = vector<num>;

num pow(num a, long long b) {
	num r = 1;
	while (b) {
		if (b & 1) r *= a;
		b >>= 1; a *= a;
	}
	return r;
}

num inv(num a) { return a.inv(); }

/// start-hash
poly& operator+=(poly& a, const poly& b) {
	if (sz(a) < sz(b)) a.resize(b.size());
	rep(i,0,sz(b)) a[i]=a[i]+b[i];
	return a;
}
poly operator+(const poly& a, const poly& b) { poly r=a; r+=b; return r; }
poly& operator-=(poly& a, const poly& b) {
	if (sz(a) < sz(b)) a.resize(b.size());
	rep(i,0,sz(b)) a[i]=a[i]-b[i];
	return a;
}
poly operator-(const poly& a, const poly& b) { poly r=a; r-=b; return r; }
poly operator*(const poly& a, const poly& b) {
	// TODO: small-case?
	auto res = fft::multiply_mod({a.begin(), a.end()}, {b.begin(), b.end()}, num::MOD);
	return {res.begin(), res.end()};
}

poly& operator*=(poly& a, const poly& b) {return a = a*b;}
/// end-hash
poly& operator*=(poly& a, const num& b) { // Optional
	trav(x, a) x = x * b;
	return a;
}
poly operator*(const poly& a, const num& b) { poly r=a; r*=b; return r; }

vector<num> inverse(const vector<num>& a) {
	assert(!a.empty());
	vector<num> b({a[0].inv()});
	b.reserve(a.size());
	// Doubles b as b[:n] = (2 - a[:n] * b[:n/2]) * b[:n/2]
	while (b.size() < a.size()) {
		int n = int(b.size());
		vector<num> tmp(a.begin(), a.begin() + min(int(a.size()), n * 2));
		b = (poly({num(2)}) - tmp * b) * b;
		b.resize(2 * n);
	}
	b.resize(a.size());
	return b;
}

// Polynomial floor division; no leading 0's plz
poly operator/(poly a, poly b) { /// start-hash
	if (sz(a) < sz(b)) return {};
	int s = sz(a)-sz(b)+1;
	reverse(a.begin(), a.end());
	reverse(b.begin(), b.end());
	a.resize(s);
	b.resize(s);
	a = a * inverse(move(b));
	a.resize(s);
	reverse(a.begin(), a.end());
	return a;
} /// end-hash
poly& operator/=(poly& a, const poly& b) {return a = a/b;}
poly& operator%=(poly& a, const poly& b) { /// start-hash
	if (sz(a) >= sz(b)) {
		poly c = (a / b) * b;
		a.resize(sz(b)-1);
		rep(i,0,sz(a)) a[i] = a[i]-c[i];
	}
	return a;
} /// end-hash
poly operator%(const poly& a, const poly& b) { poly r=a; r%=b; return r; }

// Multipoint evaluation/interpolation
/// start-hash
vector<num> eval(const poly& a, const vector<num>& x) {
	int n=sz(x);
	if (!n) return {};
	vector<poly> up(2*n);
	rep(i,0,n) up[i+n] = poly({0-x[i], 1});
	per(i,1,n) up[i] = up[2*i]*up[2*i+1];
	vector<poly> down(2*n);
	down[1] = a % up[1];
	rep(i,2,2*n) down[i] = down[i/2] % up[i];
	vector<num> y(n);
	rep(i,0,n) y[i] = down[i+n][0];
	return y;
} /// end-hash

bool isPrime(int p) {
	if (p <= 1) return false;
	if (p % 2 == 0) return p == 2;
	for (int i = 3; i * i <= p; i += 2) {
		if (p % i == 0) return false;
	}
	return true;
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int N, M;
	cin >> N >> M;
	vector<int> P(N);

	/*
	string w1; cin >> w1;
	string w2; cin >> w2;
	for (int i = 0; i < N; i++) {
		P[i] = int(w1[i]) - int(w2[i]);
	}
	*/
	mt19937 mt_P(48);
	for (int i = 0; i < N; i++) {
		P[i] = int(mt_P() % 26) - int(mt_P() % 26);
	}

	auto go = [&] (int mod) {
		cerr << mod << '\n';
		MOD = mod;
		vector<num> p(P.begin(), P.end());
		vector<num> xs;
		for (int i = 2; i <= mod-2; i++) {
			xs.push_back(i);
		}
		vector<num> ys = eval(p, xs);
		for (int i = 0; i < int(ys.size()); i++) {
			if (ys[i] == num(0)) {
				cout << mod << ' ' << xs[i] << '\n';
				exit(0);
			}
		}
	};

	vector<int> primes;
	for (int p = int(1e5); p < int(1.1e5); p++) {
		if (isPrime(p)) primes.push_back(p);
	}
	mt19937 mt(48);
	shuffle(primes.begin(), primes.end(), mt);
	for (int p : primes) {
		go(p);
	}
	assert(false);
	return 0;
}
