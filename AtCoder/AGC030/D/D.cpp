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
using num = modnum<int(1e9)+7>;

const int MAXN = 4000;
const int MAXQ = 4000;
int N, Q;
int A[MAXN];
int X[MAXQ];
int Y[MAXQ];

num prob[MAXN][MAXN]; // probability that a[i] < a[j]

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N >> Q;
	for (int i = 0; i < N; i++) cin >> A[i];
	for (int q = 0; q < Q; q++) {
		cin >> X[q] >> Y[q];
		X[q]--, Y[q]--;
	}
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			prob[i][j] = (A[i] < A[j]);
		}
	}
	for (int q = 0; q < Q; q++) {
		int x = X[q], y = Y[q];
		assert(x != y);
		for (int i = 0; i < N; i++) {
			if (i == x || i == y) continue;
			prob[i][x] = prob[i][y] = (prob[i][x] + prob[i][y]) / 2;
			prob[x][i] = prob[y][i] = (prob[x][i] + prob[y][i]) / 2;
		}
		prob[x][x] = prob[y][y] = (prob[x][x] + prob[y][y]) / 2;
		prob[x][y] = prob[y][x] = (prob[x][y] + prob[y][x]) / 2;
	}
	num ev = 0;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < i; j++) {
			ev += prob[i][j];
		}
	}
	for (int q = 0; q < Q; q++) {
		ev *= 2;
	}
	cout << ev << '\n';

	return 0;
}
