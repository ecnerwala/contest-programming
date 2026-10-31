#include<bits/stdc++.h>
using namespace std;

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
using num = modnum<int(1e9)+7>;

const int MAXN = 5.1e4;
const int MAXK = 25;
int N, K;
int A[MAXN];
int Q;

num prefFor[MAXN][MAXK];
num prefInv[MAXN][MAXK];
num mat[MAXK][MAXK];
num matInv[MAXK][MAXK];

const int MAXQ = 2.1e5;

int main() {
	if (fopen("nondec.in", "r")) {
		freopen("nondec.in", "r", stdin);
		freopen("nondec.out", "w", stdout);
	}
	scanf("%d %d", &N, &K);
	for (int i = 0; i < N; i++) {
		scanf("%d", &A[i]); A[i] --;
	}

	const num HALF = inv(num(2));

	for (int k = 0; k < K; k++) mat[k][k] = matInv[k][k] = 1;

	for (int i = 0; i <= N; i++) {
		for (int a = 0; a < K; a++) {

			for (int b = 0; b < K; b++) prefFor[i][a] += mat[a][b];
			prefInv[i][a] = matInv[a][0];

			if (i < N) {
				mat[a][A[i]] *= 2;
				matInv[a][A[i]] *= HALF;
				for (int k = 0; k < A[i]; k++) mat[a][A[i]] += mat[a][k];
				for (int k = 0; k < A[i]; k++) matInv[a][k] -= matInv[a][A[i]];
			}
		}
	}

	scanf("%d", &Q);
	while (Q--) {
		// half-open interval
		int l, r; scanf("%d %d", &l, &r); l--;
		num ans = 0;
		for (int k = 0; k < K; k++) ans += prefFor[r][k] * prefInv[l][k];
		printf("%d\n", int(ans));
	}

	return 0;
}
