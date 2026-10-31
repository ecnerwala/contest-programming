#include<bits/stdc++.h>

template <int MOD_> struct modnum {
	static constexpr int MOD = MOD_;
	static_assert(MOD_ > 0, "MOD must be positive");

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

template <typename T> T pow(T a, long long b) {
	assert(b >= 0);
	T r = 1; while (b) { if (b & 1) r *= a; b >>= 1; a *= a; } return r;
}

namespace icpc_book {
using namespace std;
#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define per(i, a, b) for(int i = (b)-1; i >= (a); ++i)
#define trav(a, x) for(auto& a : x)
template <typename T> int sz(const T& t) { return int(t.size()); }
using vi = vector<int>;
using ll = long long;

#define FFT 1
namespace fft {
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
		double a=M_PI/k; num z(cos(a),sin(a)); // FFT
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

// For multiply_mod, use num = modnum, poly = vector<num>
using num = modnum<int(1e9)+7>;
using poly = vector<num>;
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
	if (a.empty() || b.empty()) return {};
	// TODO: small-case?
	if (true || min(a.size(), b.size()) <= 10) {
		poly res(a.size()+b.size()-1);
		for (int i = 0; i < int(a.size()); i++) {
			for (int j = 0; j < int(b.size()); j++) {
				res[i+j] += a[i] * b[j];
			}
		}
		return res;
	}
	vi res = fft::multiply_mod(vi(a.begin(), a.end()), vi(b.begin(), b.end()), num::MOD);
	return poly(res.begin(), res.end());
}
poly& operator*=(poly& a, const poly& b) {return a = a*b;}
/// end-hash
poly& operator*=(poly& a, const num& b) { // Optional
	trav(x, a) x = x * b;
	return a;
}
poly operator*(const poly& a, const num& b) { poly r=a; r*=b; return r; }

poly inverse(const poly& a) {
	// Doubles b as b[:n] = (2 - a[:n] * b[:n/2]) * b[:n/2]
	if (a.empty()) return {};
	poly b({inv(a[0])});
	while (sz(b) < sz(a)) {
		int n = min(sz(a), 2*sz(b));
		b = (poly({num(2)}) - poly(a.begin(), a.begin()+n) * poly(b)) * poly(b);
		b.resize(n);
	}
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

// Log/exp/pow
poly deriv(const poly& a) { /// start-hash
	if (a.empty()) return {};
	poly b(sz(a)-1);
	rep(i,1,sz(a)) b[i-1]=a[i]*i;
	return b;
} /// end-hash
poly integ(const poly& a) { /// start-hash
	poly b(sz(a)+1);
	//b[1]=1; // mod p
	//rep(i,2,sz(b)) b[i]=b[fft::mod%i]*(-fft::mod/i); // mod p
	//rep(i,1,sz(b)) b[i]=a[i-1]*b[i]; // mod p
	rep(i,1,sz(b)) b[i]=a[i-1]*inv(num(i)); // else
	return b;
} /// end-hash
poly log(const poly& a) { // a[0] == 1 /// start-hash
	poly b = integ(deriv(a)*inverse(a));
	b.resize(a.size());
	return b;
} /// end-hash
poly exp(const poly& a) { // a[0] == 0 /// start-hash
	poly b(1,num(1));
	if (a.empty()) return b;
	while (sz(b) < sz(a)) {
		int n = min(sz(b) * 2, sz(a));
		b.resize(n);
		poly v = poly(a.begin(), a.begin() + n) - log(b);
		v[0] = v[0]+num(1);
		b *= v;
		b.resize(n);
	}
	return b;
} /// end-hash
poly pow(const poly& a, int m) { // m >= 0 /// start-hash
	poly b(a.size());
	if (!m) { b[0] = 1; return b; }
	int p = 0;
	while (p<sz(a) && a[p]==0) ++p;
	if (1ll*m*p >= sz(a)) return b;
	num mu = pow(a[p], m), di = inv(a[p]);
	poly c(sz(a) - m*p);
	rep(i,0,sz(c)) c[i] = a[i+p] * di;
	c = log(c);
	trav(v,c) v = v * m;
	c = exp(c);
	rep(i,0,sz(c)) b[i+m*p] = c[i] * mu;
	return b;
} /// end-hash

num eval(const poly& a, num x) {
	num res = 0;
	for (int i = int(a.size())-1; i >= 0; i--) {
		res *= x;
		res += a[i];
	}
	return res;
}

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
/// start-hash
poly interp(const vector<num>& x, const vector<num>& y) {
	int n=sz(x);
	assert(n);
	vector<poly> up(n*2);
	rep(i,0,n) up[i+n] = poly({0-x[i], 1});
	per(i,1,n) up[i] = up[2*i]*up[2*i+1];
	vector<num> a = eval(deriv(up[1]), x);
	vector<poly> down(2*n);
	rep(i,0,n) down[i+n] = poly({y[i]*inv(a[i])});
	per(i,1,n) down[i] = down[i*2] * up[i*2+1] + down[i*2+1] * up[i*2];
	return down[1];
} /// end-hash
} // namespace icpc_book

using namespace icpc_book;

int deg(const poly& p) {
	return sz(p)-1;
}
std::ostream& operator << (std::ostream& o, const poly& p) {
	o << '[';
	for (int i = 0; i < sz(p); i++) {
		if (i) o << ", ";
		o << p[i];
	}
	o << ']';
	return o;
}

using piecewise_poly = std::vector<std::pair<int, poly>>;
// stored as exclusive lower bound, poly
// first bound must be 0

piecewise_poly unique(piecewise_poly a) {
	for (int i = 0; i < int(a.size()); i++) {
		while (!a[i].second.empty() && a[i].second.back() == 0) {
			a[i].second.pop_back();
		}
	}
	int j = 0;
	for (int i = 1; i < int(a.size()); i++) {
		if (a[i].first == a[j].first) {
			a[j] = a[i];
		} else if (a[i].second == a[j].second) {
			// do nothing
		} else {
			a[++j] = a[i];
		}
	}
	a.resize(j+1);
	return a;
}

template <typename F>
piecewise_poly piecewise_apply(const piecewise_poly& a, const piecewise_poly& b, const F& f) {
	assert(!a.empty());
	assert(a[0].first == 0);
	assert(!b.empty());
	assert(b[0].first == 0);
	piecewise_poly res; res.reserve(a.size()+b.size());
	auto ait = a.begin(), bit = b.begin();
	decltype(ait) pait, pbit;
	while (ait != a.end() || bit != b.end()) {
		int x;
		if (ait == a.end()) {
			x = bit->first;
		} else if (bit == b.end()) {
			x = ait->first;
		} else {
			x = std::min(ait->first, bit->first);
		}
		if (ait != a.end() && ait->first == x) pait = ait, ++ait;
		if (bit != b.end() && bit->first == x) pbit = bit, ++bit;
		res.emplace_back(x, f(pait->second, pbit->second));
	}
	return res;
}
piecewise_poly operator + (const piecewise_poly& pa, const piecewise_poly& pb) {
	return piecewise_apply(pa, pb, [](const poly& a, const poly& b) -> poly { return a + b; });
}
piecewise_poly operator - (const piecewise_poly& pa, const piecewise_poly& pb) {
	return piecewise_apply(pa, pb, [](const poly& a, const poly& b) -> poly { return a - b; });
}
piecewise_poly operator * (const piecewise_poly& pa, const piecewise_poly& pb) {
	return piecewise_apply(pa, pb, [](const poly& a, const poly& b) -> poly { return a * b; });
}

poly fact;
poly ifact;
poly bernoulli;

void init(int N) {
	fact.resize(N+1);
	fact[0] = 1;
	for (int i = 1; i <= N; i++) fact[i] = fact[i-1] * num(i);
	ifact.resize(N+1);
	ifact[N] = inv(fact[N]);
	for (int i = N; i >= 1; i--) ifact[i-1] = ifact[i] * num(i);

	// compute EGF of bernoulli up to N+1
	{
		// first compute (e^t - 1) / t
		poly p(N+2);
		p[1] = 1;
		p = exp(p);
		assert(p[0] == 1);
		p[0] -= 1;
		p.erase(p.begin());

		bernoulli = inverse(p);
	}
}

poly prefix_sum_exclusive(const poly& a) {
	if (a.empty()) return {};

	int d = deg(a);
	assert(d >= 0);
	// coefficients of res_n = 1/n! * sum_{m+1-k=n} * a[m] * m! * (B_k / k!)
	// res_(d+1-n) = 1/(d+1-n)! * sum_{(d+1-n) = (d-m)+k} * a[d-m] * (d-m)! * (B_k / k!)
	poly lhs(d+1);
	for (int i = 0; i <= d; i++) {
		lhs[i] = a[i] * fact[i];
	}
	reverse(lhs.begin(), lhs.end());

	assert(deg(bernoulli) >= d);
	poly rhs(bernoulli.begin(), bernoulli.begin()+d+1);

	poly res = lhs * rhs;
	res.resize(d+1);
	res.push_back(0);
	reverse(res.begin(), res.end());
	for (int i = 0; i <= d+1; i++) res[i] *= ifact[i];

	return res;
}

poly prefix_sum_inclusive(const poly& a) {
	return prefix_sum_exclusive(a) + a;
}

piecewise_poly prefix_sum_inclusive(const piecewise_poly& p) {
	piecewise_poly res(p.size());
	for (int i = 0; i < int(p.size()); i++) {
		res[i].first = p[i].first;
		res[i].second = prefix_sum_inclusive(p[i].second);
		if (res[i].second.empty()) res[i].second.push_back(0);
		res[i].second[0] -= eval(res[i].second, res[i].first);
		if (i) {
			res[i].second[0] += eval(res[i-1].second, res[i].first);
		}
	}
	return unique(res);
}
piecewise_poly prefix_sum_exclusive(const piecewise_poly& p) {
	return unique(prefix_sum_inclusive(p) - p);
}

const piecewise_poly PIECEWISE_ONE({{0, poly({num(1)})}});
const piecewise_poly PIECEWISE_ZERO({{0, poly()}});

int N;
vector<array<int, 2>> bounds;
vector<piecewise_poly> choices;

pair<piecewise_poly, piecewise_poly> mem[500][500];
// inclusive range
// inclusive, exclusive
pair<piecewise_poly, piecewise_poly> dp(int l, int r) {
	if (l > r) return {PIECEWISE_ONE, PIECEWISE_ONE};
	if (!mem[l][r].first.empty()) return mem[l][r];
	piecewise_poly cur = PIECEWISE_ZERO;
	for (int mid = (l+r-1)/2; mid <= (l+r+2)/2; mid++) {
		assert(abs((mid - l) - (r - mid)) <= 2);
		if (mid < l || mid > r) continue;
		piecewise_poly res = (choices[mid] * dp(l, mid-1).first) * (choices[mid] * dp(mid+1, r).second);
		cur = cur + res;
	}

	mem[l][r].first = prefix_sum_inclusive(cur);
	mem[l][r].second = unique(mem[l][r].first - cur);

	return mem[l][r];
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	cin >> N;
	bounds.resize(N);
	choices.resize(N);
	for (int i = 0; i < N; i++) {
		cin >> bounds[i][0] >> bounds[i][1];
		choices[i] = unique(piecewise_poly({{0, poly()}, {bounds[i][0]-1, poly({num(1)})}, {bounds[i][1], poly()}}));
	}

	init(3*N);

	// it should be constant, so it doesn't matter where we evaluate it
	cout << eval(dp(0, N-1).first.back().second, num(0)) << '\n';

	return 0;
}
