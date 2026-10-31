#include <bits/stdc++.h>
using namespace std;
// head

#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define per(i,a,n) for (int i=n-1;i>=a;i--)
#define trav(a, x) for(auto& a : x)
#define all(x) x.begin(), x.end()
#define sz(x) (int)(x).size()
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

#define FFT 0

namespace fft {
#if FFT
// FFT
using dbl = double;
struct num {
	dbl x, y;
	num(dbl x_ = 0, dbl y_ = 0) : x(x_), y(y_) { }
};
inline num operator+(num a, num b) { return num(a.x + b.x, a.y + b.y); }
inline num operator-(num a, num b) { return num(a.x - b.x, a.y - b.y); }
inline num operator*(num a, num b) { return num(a.x * b.x - a.y * b.y, a.x * b.y + a.y * b.x); }
inline num conj(num a) { return num(a.x, -a.y); }
inline num inv(num a) { dbl n = (a.x*a.x+a.y*a.y); return num(a.x/n,-a.y/n); }
#else
// NTT
const int mod = 998244353, g = 3;
struct num {
	int v;
	num(int v_ = 0) : v(v_) { }
	explicit operator int() const { return v; }
};
inline num operator+(num a, num b) { return num((a.v + b.v)%mod); }
inline num operator-(num a, num b) { return num((a.v + mod - b.v)%mod); }
inline num operator*(num a, num b) { return num(int(1ll*a.v*b.v%mod)); }
inline num pow(num a, int b) {
	num r = 1;
	do{if(b&1)r=r*a;a=a*a;}while(b>>=1);
	return r;
}
inline num inv(num a) { return pow(a, mod-2); }
#endif

using vn = vector<num>;
vi rev({0, 1});
vn rt(2, num(1)), fa, fb;

inline void init(int n) {
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
}

inline void fft(vector<num> &a, int n) {
	init(n);
	int s = __builtin_ctz(sz(rev)/n);
	rep(i,0,n) if (i < rev[i]>>s) swap(a[i], a[rev[i]>>s]);
	for (int k = 1; k < n; k *= 2)
		for (int i = 0; i < n; i += 2 * k) rep(j,0,k) {
			num t = rt[j+k] * a[i+j+k];
			a[i+j+k] = a[i+j] - t;
			a[i+j] = a[i+j] + t;
		}
}

// Complex/NTT
vn multiply(vn a, vn b) {
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
}

// Complex/NTT power-series inverse
// Doubles b as b[:n] = (2 - a[:n] * b[:n/2]) * b[:n/2]
vn inverse(const vn& a) {
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
}

#if FFT
// Double multiply (num = complex)
using vd = vector<double>;
vd multiply(const vd& a, const vd& b) {
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
}

// Integer multiply mod m (num = complex)
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
}
#endif

} // namespace fft

using fft::num;
using poly = fft::vn;
using fft::multiply;
using fft::inverse;
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
poly& operator*=(poly& a, const num& b) { // Optional
	trav(x, a) x = x * b;
	return a;
}
poly operator*(const poly& a, const num& b) { poly r=a; r*=b; return r; }
poly operator*(const poly& a, const poly& b) {
	// TODO: small-case?
	return multiply(a, b);
}
poly& operator*=(poly& a, const poly& b) {return a = a*b;}

// Polynomial floor division
poly operator/(poly a, poly b) { // no leading 0's plz
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
}
poly& operator/=(poly& a, const poly& b) {return a = a/b;}
poly& operator%=(poly& a, const poly& b) {
	if (sz(a) >= sz(b)) {
		poly c = (a / b) * b;
		a.resize(sz(b)-1);
		rep(i,0,sz(a)) a[i] = a[i]-c[i];
	}
	return a;
}
poly operator%(const poly& a, const poly& b) { poly r=a; r%=b; return r; }

// Log/exp/pow
poly deriv(const poly& a) {
	if (a.empty()) return {};
	poly b(sz(a)-1);
	rep(i,1,sz(a)) b[i-1]=a[i]*i;
	return b;
}
poly integ(const poly& a) {
	poly b(sz(a)+1);
	// TODO: Fast inverse as you go
	rep(i,1,sz(b)) b[i]=a[i-1]*inv(num(i));
	return b;
}
poly log(const poly& a) { // a[0] == 1
	poly b = integ(deriv(a)*inverse(a));
	b.resize(a.size());
	return b;
}
poly exp(const poly& a) { // a[0] == 0
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
}
poly pow(const poly& a, int m) { // m >= 0
	poly b(a.size());
	if (!m) { b[0] = 1; return b; }
	int p = 0;
	while (p<sz(a) && a[p].v==0) ++p;
	if (1ll*m*p >= sz(a)) return b;
	num mu = pow(a[p], m), di = inv(a[p]);
	poly c(sz(a) - m*p);
	rep(i,0,sz(c)) c[i] = a[i+p] * di;
	c = log(c);
	trav(v,c) v = v * m;
	c = exp(c);
	rep(i,0,sz(c)) b[i+m*p] = c[i] * mu;
	return b;
}

// Multipoint evaluation/interpolation
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
}
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
}

using fft::mod;
ll powmod(ll a,ll b) {ll res=1;a%=mod;for(;b;b>>=1){if(b&1)res=res*a%mod;a=a*a%mod;}return res;}
#define N 31000
int n, k;
int mi[N], ni[N];
vector <int> gg, ve[N];
int ans;
int sum[N];
bool used[N];

int L[N], R[N], dep[N], fa[N][21], SS[N];
int Time;

int C(int x, int y) {
	if (x < y)
		return 0;
	assert(x <= k);
	return 1LL * mi[x] * ni[y] % mod * ni[x - y] % mod;
}

void dfs1(int x) {
	used[x] = true;
	sum[x] = 1;
	for (auto y: ve[x])
		if (!used[y])
			dfs1(y), sum[x] += sum[y];
	int tot = powmod(n, k);
	for (auto y: ve[x])
		if (!used[y])
			tot = (tot - gg[sum[y]]) % mod;
	tot = (tot - gg[n - sum[x]]) % mod;

	if (k % 2 == 0) {
		int ss = 0;
		long long ff = 0;
		for (auto y: ve[x])
			if (!used[y])
				ff = (ff + 1LL * powmod(sum[y], k / 2) * ss) % mod, ss = (ss + powmod(sum[y], k / 2)) % mod;
		ff += 1LL * powmod(n - sum[x], k / 2) * ss;
		ff %= mod;
		tot = (tot + 1LL * C(k, k / 2) * ff) % mod;
	}
	ans = (ans + 1LL * x * tot) % mod;
	used[x] = false;
}

void dfs_sum(int k) {
	used[k] = true;
	sum[k] = 1;
	for (auto x: ve[k])
		if (!used[x])
			dfs_sum(x), sum[k] += sum[x];
	used[k] = false;
}

int zx;

void dfs_zx(int k, int S) {
	used[k] = true;
	bool ok = (S - sum[k]) <= S / 2;
	for (auto x: ve[k])
		if (!used[x]) {
			dfs_zx(x, S);
			ok &= (sum[x] <= S / 2);
		}
	if (ok)
		zx = k;

	used[k] = false;
}

vector <pair<int, int> > V;

int getsum(int x, int root) {
	if (!(L[x] <= L[root] && L[root] <= R[x]))
		return SS[x];
	for (int i = 20; i >= 0; i--)
		if (dep[fa[root][i]] > dep[x])
			root = fa[root][i];
	return n - SS[root];
}

void dfs_put(int x, int mi, int dep, int root, int f) {
	int val = powmod(getsum(x, root), k / 2);
	mi = min(mi, x);
	used[x] = true;

	for (auto y: ve[x])
		if (!used[y]) {
			dfs_put(y, mi, dep + 1, root, x);
		}
	for (auto y: ve[x])
		if (y != f) {
			val = (val - powmod(getsum(y, root), k / 2)) % mod;
		}

	if (dep) {
		V.push_back(make_pair(-mi, val));
	}

	used[x] = false;
}

int solve(vector <pair<int, int> >& V) {
	sort(V.begin(), V.end());
	int pre = 0;
	int sum = 0;
	for (auto p: V) {
		sum = (sum + 1LL * (-p.first) * p.second % mod * pre) % mod;
		pre = (pre + p.second) % mod;
	}
	return sum;
}

void dfs(int x) {
	dfs_sum(x);
	dfs_zx(x, sum[x]);
	x = zx;

	dfs_sum(x);

	V.clear();

	dfs_put(x, 1e9, 0, x, 0);
	ans = (ans + 1LL * C(k, k / 2) * solve(V)) % mod;
	
	int sumpow = 0;
	for (auto y: ve[x])
		sumpow = (sumpow + powmod(getsum(y, x), k / 2)) % mod;

	used[x] = true;
	for (auto y: ve[x])
		if (!used[y]) {
			V.clear();
			dfs_put(y, x, 1, x, x);
			ans = (ans - 1LL * C(k, k / 2) * solve(V)) % mod;
			int ss = (powmod(n - getsum(y, x), k / 2) - (sumpow - powmod(getsum(y, x), k / 2)) % mod) % mod;
			for (auto p: V) {
				ans = (ans + 1LL * C(k, k / 2) * ss % mod * (-p.first) % mod * p.second) % mod;
			}
		}

	for (auto y: ve[x])
		if (!used[y])
			dfs(y);
}

void mydfs(int k, int f) {
	SS[k] = 1;
	used[k] = true;
	dep[k] = dep[f] + 1;
	fa[k][0] = f;
	L[k] = ++Time;
	for (int i = 1; i <= 20; i++)
		fa[k][i] = fa[fa[k][i - 1]][i - 1];

	for (auto y: ve[k])
		if (!used[y]) {
			mydfs(y, k);
			SS[k] += SS[y];
		}
	R[k] = Time;
	used[k] = false;
}

int main() {
	scanf("%d%d", &n, &k);
	for (int i = 1; i < n; i++) {
		int x, y;
		scanf("%d%d", &x, &y);
		ve[x].push_back(y);
		ve[y].push_back(x);
	}
	mi[0] = 1;
	for (int i = 1; i <= k; i++)
		mi[i] = 1LL * mi[i - 1] * i % mod;
	for (int i = 0; i <= k; i++)
		ni[i] = powmod(mi[i], mod - 2);
	// for (int s = 0; s <= n; s++) {
	// 	for (int j = (k + 1) / 2; j <= k; j++)
	// 		ff[s] = (ff[s] + 1LL * C(k, j) * powmod(s, j) % mod * powmod(n - s, k - j)) % mod;
	// }

	poly A(k+1), B(k+1);
	
	for (int j = (k + 1) / 2; j <= k; j++)
		A[j] = ni[j];
	for (int j = 0; j <= k; j++)
		if (j % 2 == 0)
			B[j] = ni[j];
		else
			B[j] = (mod-ni[j])%mod;

	auto C = A * B;
	C.resize(k + 1);
	for (int i = 0; i <= k; i++) {
		C[i] = C[i] * num(mi[k] % mod * powmod(n, k - i) % mod * ni[k - i] % mod);
	}
	vector <num> dd;
	for (int i = 0; i <= n; i++)
		dd.push_back(i);
	auto vgg = eval(C, dd);
	gg = vi(all(vgg));

	dfs1(1);

	if (k % 2 == 0) {
		mydfs(1, 0);
		dfs(1);
	}
	ans = (ans + mod) % mod;
	printf("%d\n", ans);

}
