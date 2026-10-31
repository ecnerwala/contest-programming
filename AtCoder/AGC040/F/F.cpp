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
using num = modnum<998244353>;

const int MAXV = 2.1e7;
const int V = int(2e7) + 100;
num fact[MAXV];
num ifact[MAXV];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	fact[0] = 1;
	for (int i = 1; i <= V; i++) fact[i] = fact[i-1] * num(i);
	ifact[V] = fact[V].inv();
	for (int i = V; i >= 1; i--) ifact[i-1] = ifact[i] * num(i);
	auto choose = [&](int n, int r) -> num {
		if (0 <= r && r <= n) return fact[n] * ifact[r] * ifact[n-r];
		else return 0;
	};

	int N, A, B; cin >> N >> A >> B;

	num ans = 0;
	for (int k = 0; k <= A && B + k <= N; k++) {
		int jumps = N - B - k;
		int slack = A - k;
		if (slack) {
			if (!jumps) {
				continue;
			}
			jumps--;
		}
		// jumps can go anywhere among slack + 1 things
		num jumpWays = choose(jumps + slack, jumps);
		// strictly below, except at the start
		num moveWays = k == 0 ? 1 : choose(B + k-1, B-1) - choose(B + k-1, B);
		ans += jumpWays * moveWays;
	}
	cout << ans << '\n';

	return 0;
}
