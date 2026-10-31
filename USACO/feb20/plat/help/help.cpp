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

const int MAXN = 1.1e5;
const int MAXK = 12;
int N, K;
int endpoint[MAXN*2];

num choose[MAXK+1][MAXK+1];

const int S = 1 << 18;
num lazy[2*S];
num seg[2*S][MAXK+1];
void applyMul(int i, num v) {
	lazy[i] *= v;
	for (int k = 0; k <= K; k++) seg[i][k] *= v;
}
void updateNode(int i) {
	if (i < S) {
		for (int k = 0; k <= K; k++) {
			seg[i][k] = (seg[2*i][k] + seg[2*i+1][k]) * lazy[i];
		}
	}
}
void updateAll(int i) {
	for (int a = i; a; a /= 2) updateNode(a);
}
void propogateNode(int i) {
	if (i < S) {
		applyMul(2*i+0, lazy[i]);
		applyMul(2*i+1, lazy[i]);
		lazy[i] = 1;
	}
}
void propogateAll(int i) {
	if (!i) return;
	propogateAll(i/2);
	propogateNode(i);
}

int main() {
	if (fopen("help.in", "r")) {
		freopen("help.in", "r", stdin);
		freopen("help.out", "w", stdout);
	}
	cin >> N >> K;
	for (int i = 0; i < N; i++) {
		int l, r; cin >> l >> r;
		assert(l < r);
		endpoint[l] = r;
		endpoint[r] = l;
	}

	for (int i = 0; i <= K; i++) {
		choose[i][0] = choose[i][i] = 1;
		for (int j = 1; j < i; j++) {
			choose[i][j] = choose[i-1][j-1] + choose[i-1][j];
		}
	}

	for (int i = 1; i < 2*S; i++) lazy[i] = 1;

	seg[S+0][0] = 1;
	updateAll(S+0);

	for (int i = 1; i <= 2*N; i++) {
		if (endpoint[i] < i) {
			// allow ourselves to take this
			// we don't need to propogate because it's linear
			for (int k2 = K; k2 >= 0; k2--) {
				for (int k1 = 0; k1 < k2; k1++) {
					seg[S+i][k2] += seg[S+i][k1] * choose[k2][k1];
				}
			}
			updateAll(S+i);
		} else {
			int j = endpoint[i];
			propogateAll(S+j);
			for (int a = S+0, b = S+j; a < b; a /= 2, b /= 2) {
				if (b & 1) {
					--b;
					for (int k = 0; k <= K; k++) { seg[S+j][k] += seg[b][k]; }
				}
			}
			for (int a = S+j+1, b = S+S; a < b; a /= 2, b /= 2) {
				if (a & 1) {
					applyMul(a, 2);
					a++;
				}
			}
			updateAll(S+j);
		}
	}

	cout << seg[1][K] << '\n';

	return 0;
}
