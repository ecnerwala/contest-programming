#include<bits/stdc++.h>
using namespace std;

// probability of exactly k monkeys defeating everyone else
// that group must be unique
// choose k monkeys, then it's a bunch of p's and a bunch of (1-p)'s
// # of 1-p's = sum of indices of the k - (k choose 2)
// take the product of (1 + (1-p)^i p^(n-1-i) x)
// what's P_{a,n}(x) = product_{i=0^n-1} (1 + a^i x)

int modinv(int a, int m) {
	a %= m;
	assert(a > 0);
	if (a == 1) return 1;
	return m - int(((long long)modinv(m, a)) * m / a);
}
template <int MOD_> struct modnum {
	using ll = long long;
	static const int MOD = MOD_;
private:
	int v;
public:
	modnum() : v(0) {}
	modnum(ll v_) : v(int(v_ % MOD)) { if (v < 0) v += MOD; }
	explicit operator int () { return v; }

	friend std::ostream& operator << (std::ostream& o, const modnum& m) { return o << m.v; }
	modnum operator - () const {
		modnum res;
		res.v = (v ? MOD - v : 0);
		return res;
	}
	modnum operator ~ () const {
		modnum res;
		res.v = modinv(v, MOD);
		return res;
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
		return *this *= (~o);
	}

	friend modnum operator + (const modnum& a, const modnum& b) { return modnum(a) += b; }
	friend modnum operator - (const modnum& a, const modnum& b) { return modnum(a) -= b; }
	friend modnum operator * (const modnum& a, const modnum& b) { return modnum(a) *= b; }
	friend modnum operator / (const modnum& a, const modnum& b) { return modnum(a) /= b; }

	friend bool operator == (const modnum& a, const modnum& b) { return a.v == b.v; }

	modnum pow(ll e) const {
		assert(e >= 0);
		modnum r = 1;
		modnum a = *this;
		while (e) {
			if (e & 1) r *= a;
			a *= a, e /= 2;
		}
		return r;
	}
};
using num = modnum<998244353>;
const num PRIMITIVE_ROOT = 3;
template <typename num> struct fft {
	const int L;
	const int N = (1 << L);
	using num_t = num;

	const num root;
	const num invRoot = num(1) / root;
	const num invN = num(1) / num(N);
	num roots[30+1];
	num invRoots[30+1];
	fft(int L_, num root_) : L(L_), root(root_) {
		roots[0] = root;
		invRoots[0] = invRoot;
		for (int i = 1; i <= L; i++) {
			roots[i] = roots[i-1] * roots[i-1];
			invRoots[i] = invRoots[i-1] * invRoots[i-1];
		}
		assert(roots[L] == 1);
		assert(invRoots[L] == 1);
	}

	void bitReversal(num* a) {
		for (int i = 1, j = 0; i < N-1; i++) {
			for (int k = N / 2; k > (j ^= k); k /= 2);
			if (j < i) {
				std::swap(a[j], a[i]);
			}
		}
	}

	void operator () (num* a, bool inv = false) {
		bitReversal(a);
		for (int l = 1, p = 0; l < N; l <<= 1, p++) {
			num w = inv ? invRoots[L - p - 1] : roots[L - p - 1];
			for (int k = 0; k < N; k += (2 * l)) {
				num v = 1;
				for (int i = k; i < k + l; i++, v *= w) {
					num x = a[i];
					num y = v * a[i+l];
					a[i] = x + y;
					a[i+l] = x - y;
				}
			}
		}
		if (inv) {
			for (int i = 0; i < N; i++) {
				a[i] *= invN;
			}
		}
	}
};
void conv(vector<num>& a, vector<num> b) {
	int s = int(a.size()) + int(b.size()) - 1;
	if (s <= 0) {
		a.clear();
		return;
	}
	if (s <= 200) {
		vector<num> c(s);
		for (int i = 0; i < int(a.size()); i++) {
			for (int j = 0; j < int(b.size()); j++) {
				c[i+j] += a[i] * b[j];
			}
		}
		a = c;
		return;
	}
	int L = s > 1 ? 32 - __builtin_clz(s-1) : 0, n = 1 << L;
	fft<num> f(L, PRIMITIVE_ROOT.pow((num::MOD-1) / n));
	a.resize(n); f(&a[0], false);
	b.resize(n); f(&b[0], false);
	for (int i = 0; i < n; i++) a[i] *= b[i];
	f(&a[0], true); a.resize(s);
}
void sq(vector<num>& a) {
	int s = int(a.size()) + int(a.size()) - 1;
	if (s <= 0) {
		a.clear();
		return;
	}
	if (s <= 200) {
		vector<num> c(s);
		for (int i = 0; i < int(a.size()); i++) {
			for (int j = 0; j < int(a.size()); j++) {
				c[i+j] += a[i] * a[j];
			}
		}
		a = c;
		return;
	}
	int L = s > 1 ? 32 - __builtin_clz(s-1) : 0, n = 1 << L;
	fft<num> f(L, PRIMITIVE_ROOT.pow((num::MOD-1) / n));
	a.resize(n); f(&a[0], false);
	for (int i = 0; i < n; i++) a[i] *= a[i];
	f(&a[0], true); a.resize(s);
}

const int MAXN = 7e5;
int N;
int A, B;
num P, Q;

// here, a vector of size n+1 is product_{i=0^n-1} (1+p^i q^(n-1-i) x)
void merge(vector<num>& a, vector<num> b) {
	int as = int(a.size()) - 1;
	int bs = int(b.size()) - 1;
	// for each x in a, we should add in bs Q factors
	// for each x in b, we should add in as P factors
	num bfac = Q.pow(bs);
	num cur = 1;
	for (int i = 0; i < int(a.size()); i++) {
		a[i] *= cur;
		cur *= bfac;
	}
	num afac = P.pow(as);
	cur = 1;
	for (int i = 0; i < int(b.size()); i++) {
		b[i] *= cur;
		cur *= afac;
	}
	conv(a, std::move(b));
}

vector<num> base;
void rec(int v) {
	if (v == 1) {
		base = {num(1), num(1)};
		return;
	} else {
		rec(v/2);
		merge(base, base);
		if (v & 1) {
			num cur = 1;
			for (int i = 0; i < int(base.size()); i++) {
				base[i] *= cur;
				cur *= Q;
			}
			num fac = P.pow(int(base.size()) - 1);
			base.push_back(0);
			for (int i = int(base.size()) - 1; i > 0; i--) {
				base[i] += base[i-1] * fac;
			}
		}
	}
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N >> A >> B;
	P = num(A) / num(B);
	Q = 1 - P;
	rec(N);
	assert(int(base.size()) == (N+1));
	num ans = 0;
	num g = 1;
	num extra = 1;
	for (int k = 1; k <= N-1; k++) {
		// there are (k choose 2) extraneous factors of PQ
		ans += base[k] * extra * g;
		g = g * g + 2;
		extra /= (P*Q).pow(k);
	}
	cout << ans << '\n';

	return 0;
}
