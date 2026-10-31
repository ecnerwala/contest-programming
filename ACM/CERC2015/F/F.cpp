#include<bits/stdc++.h>
using namespace std;

#ifndef M_PI
const double M_PI = acos(-1);
#endif

#define rep(i, a, b) for(int i = a; i < (b); ++i)
#define trav(a, x) for (auto& a : x)
#define all(x) begin(x), end(x)
#define sz(x) (int)(x).size()
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;

namespace fft {
#define FFT 1

using dbl = double;
struct num { 
	dbl x, y;
	num(dbl x_ = 0, dbl y_ = 0) : x(x_), y(y_) {}
};
inline num operator+(num a, num b) { return num(a.x + b.x, a.y + b.y); }
inline num operator-(num a, num b) { return num(a.x - b.x, a.y - b.y); }
inline num operator*(num a, num b) { return num(a.x * b.x - a.y * b.y, a.x * b.y + a.y * b.x); }
inline num conj(num a) { return num(a.x, -a.y); }
inline num inv(num a) { dbl n = (a.x * a.x + a.y * a.y); return num(a.x/n, -a.y/n); }

using vn = vector<num>;
vi rev({0,1});
vn rt(2, num(1)), fa, fb;
inline void init(int n) {
	if (n <= sz(rt)) return;
	rev.resize(n);
	rep(i,0,n) rev[i] = (rev[i>>1] | ((i&1)*n)) >> 1;
	rt.reserve(n);
	for (int k = sz(rt); k < n; k *= 2) {
		rt.resize(2*k);
#if FFT 
		double a = M_PI/k; num z(cos(a), sin(a));
#else
		num z = pow(num(g), (mod-1)/(2*k));
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

vi multiply_mod(const vi& a, const vi& b, int m) {
	int s = sz(a) + sz(b) - 1;
	if (s <= 0) return {};
	int L = s > 1 ? 32 - __builtin_clz(s-1) : 0, n = 1 << L;
	if (sz(fa) < n) fa.resize(n);
	if (sz(fb) < n) fb.resize(n);
	rep(i,0,sz(a)) fa[i] = num(a[i] & ((1<<15) - 1), a[i] >> 15);
	fill(fa.begin()+sz(a), fa.begin()+n,0);
	rep(i,0,sz(b)) fb[i] = num(b[i] & ((1<<15) - 1), b[i] >> 15);
	fill(fb.begin()+sz(b), fb.begin()+n,0);

	fft(fa, n);
	fft(fb, n);

	double r0 = 0.5 / n;
	rep(i,0,n/2+1) {
		int j = (n-i) & (n-1);
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
			+ (ll(fa[i].y + 0.5) % m << 15)
			+ (ll(fb[i].x + 0.5) % m << 15)
			+ (ll(fb[i].y + 0.5) % m << 30)) % m);
	return r;
}

} // namespace fft
const ll MOD = int(1e6)+3;

ll minv(ll a, ll m) {
	return a == 1 ? 1 : m - minv(m % a, a) * m / a;
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);

	int N; ll A, B, C; cin >> N >> A >> B >> C;

	vector<ll> X(N);
	vector<ll> Y(N);
	for (int i = 0; i < N; i++) {
		cin >> X[i];
	}
	for (int i = 0; i < N; i++) {
		cin >> Y[i];
	}

	vector<ll> fact(2*N+1);
	fact[0] = 1;
	for (int i = 1; i <= 2*N; i++) fact[i] = fact[i-1] * i % MOD;
	vector<ll> ifact(2*N+1);
	ifact[2*N] = minv(fact[2*N], MOD);
	for (int i = 2*N; i >= 1; i--) ifact[i-1] = ifact[i] * i % MOD;

	vector<ll> pA(N+1);
	pA[0] = 1; for (int i = 1; i <= N; i++) pA[i] = pA[i-1] * A % MOD;
	vector<ll> pB(N+1); pB[0] = 1;
	pB[0] = 1; for (int i = 1; i <= N; i++) pB[i] = pB[i-1] * B % MOD;


	auto paths = [&](int dx, int dy) {
		return fact[dx+dy] * ifact[dx] % MOD * ifact[dy] % MOD;
	};

	ll ans = 0;
	for (int i = 1; i < N; i++) {
		ans += X[i] * paths(N-1-i, N-2) % MOD * pA[N-1] % MOD * pB[N-1-i] % MOD;
		ans %= MOD;
		ans += Y[i] * paths(N-1-i, N-2) % MOD * pA[N-i-1] % MOD * pB[N-1] % MOD;
		ans %= MOD;
	}

	//cerr << ans << '\n';

	vector<int> va(N-1);
	vector<int> vb(N-1);
	for (int i = 0; i < N-1; i++) {
		va[i] = int(pA[i] * ifact[i] % MOD);
		vb[i] = int(pB[i] * ifact[i] % MOD);
	}

	vector<int> x = fft::multiply_mod(va, vb, MOD);
	for (int i = 0; i < int(x.size()); i++) {
		ans += C * fact[i] % MOD * x[i] % MOD;
		ans %= MOD;
	}
	cout << ans << '\n';

	return 0;
}
