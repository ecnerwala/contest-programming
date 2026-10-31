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

const int MAXV = 1.1e5;
num fact[MAXV+1];
num ifact[MAXV+1];
num p4[MAXV+1];

void precomp() {
	fact[0] = 1;
	for (int i = 1; i <= MAXV; i++) fact[i] = fact[i-1] * num(i);
	ifact[MAXV] = inv(fact[MAXV]);
	for (int i = MAXV; i >= 1; i--) ifact[i-1] = ifact[i] * num(i);

	p4[0] = 1;
	for (int i = 1; i <= MAXV; i++) p4[i] = p4[i-1] * num(4);
}

num binom(int n, int r) {
	assert(0 <= r && r <= n);
	return fact[n] * ifact[r] * ifact[n-r];
}

num getDist1D(int t, int v) {
	assert(t >= 0);
	if ((t^v) & 1) return 0;
	if (t < abs(v)) return 0;
	return binom(t, (t+v)/2);
}

num getDist2D(int t, int dx, int dy) {
	return getDist1D(t, dx + dy) * getDist1D(t, dx - dy);
}

num getDist2D(int t, pair<int, int> a, pair<int, int> b) {
	return getDist2D(t, a.first-b.first, a.second-b.second);
}

int X, Y, N, D;
int B;
vector<pair<int, int>> bads;
num dp[2000][16];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	precomp();

	cin >> X >> Y >> N >> D;
	for (int i = -D; i <= D; i++) {
		for (int j = -D; j <= D; j++) {
			if (abs(i) + abs(j) == D) bads.emplace_back(i, j);
		}
	}
	B = int(bads.size());

	pair<int, int> S(X, Y);

	num ans = p4[N];

	for (int t = 1; t <= N; t++) {
		for (int b = 0; b < B; b++) {
			dp[t][b] += getDist2D(t, S, bads[b]);
		}
	}

	for (int t = 1; t <= N; t++) {
		for (int b = 0; b < B; b++) {
			if (dp[t][b] == num(0)) continue;

			ans -= dp[t][b] * p4[N-t];

			for (int t2 = t+2; t2 <= N; t2 += 2) {
				for (int b2 = 0; b2 < B; b2++) {
					dp[t2][b2] -= dp[t][b] * getDist2D(t2-t, bads[b], bads[b2]);
				}
			}
		}
	}
	cout << ans << '\n';

	return 0;
}
