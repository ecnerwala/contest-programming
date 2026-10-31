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

template <typename T> T pow(T a, long long b) {
	assert(b >= 0);
	T r = 1; while (b) { if (b & 1) r *= a; b >>= 1; a *= a; } return r;
}

using num = modnum<int(1e9)+7>;

const int S = 1 << 19;

num val[2*S];
num p2[2*S];
int lazy[2*S];
void updateNode(int a) {
	if (a < S) {
		p2[a] = p2[2*a] * p2[2*a+1];
		val[a] = val[2*a] + val[2*a+1] * p2[2*a];
	} else {
		p2[a] = 2;
		val[a] = 0;
	}
	if (lazy[a]) {
		val[a] = lazy[a] > 0 ? p2[a]-1 : 0;
	}
}
void setLazy(int a, int v) {
	assert(v);
	lazy[a] = v;
	updateNode(a);
}
void updateAll(int a) {
	if (!a) return;
	updateNode(a);
	updateAll(a/2);
}

void propogateNode(int a) {
	if (a < S) {
		if (lazy[a]) {
			lazy[2*a] = lazy[2*a+1] = lazy[a];
			updateNode(2*a);
			updateNode(2*a+1);
			lazy[a] = 0;
		}
	}
}
void propogateAll(int a) {
	if (!a) return;
	propogateAll(a/2);
	propogateNode(a);
}

void updateRange(int l, int r) {
	assert(l <= r);
	propogateAll(S+l);
	propogateAll(S+r);
	for (int a = S+l, b = S+r+1; a < b; a /= 2, b /= 2) {
		if (a & 1) setLazy(a++, 1);
		if (b & 1) setLazy(--b, 1);
	}
	updateAll(S+l);
	updateAll(S+r);
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int N, Q; cin >> N >> Q;
	string V; cin >> V;
	reverse(V.begin(), V.end());
	for (int i = 0; i < N; i++) {
		lazy[S+i] = (V[i] == '1' ? 1 : -1);
	}
	for (int i = 2*S-1; i; i--) updateNode(i);

	while (Q--) {
		int L, R; cin >> L >> R;
		updateRange(N-R, N-L);
		cout << val[1] << '\n';
	}

	return 0;
}
