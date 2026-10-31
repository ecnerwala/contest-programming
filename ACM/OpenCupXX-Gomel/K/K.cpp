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
using num = modnum<998244353>;

num ans[2010][2010];
num fact[4011];
num ifact[4011];

num dp[2020][2020];

void precomp() {
	fact[0] = 1; for (int i = 1; i <= 4010; i++) fact[i] = fact[i-1] * num(i);
	ifact[4010] = inv(fact[4010]); for (int i = 4010; i >= 1; i--) ifact[i-1] = ifact[i] * num(i);

	auto choose = [&](int n, int r) -> num {
		if (0 <= r && r <= n) {
			assert(n <= 4010);
			return fact[n] * ifact[r] * ifact[n-r];
		} else return 0;
	};

	for (int l = 17; l <= 1000; l++) {
		for (int x = 1; max(x, (x+1) * l - x) <= 2000; x++) {
			for (int y = max(x, (x+1) * l - x); y <= min(2000, (x+1) * l); y++) {
				num v = 0;
				for (int b = 0; b <= x+1 && (l+1) * b <= y; b++) {
					v += ((b & 1) ? num(-1) : num(1)) * choose(x+1, b) * choose(y - (l+1) * b + x, x);
				}
				//cerr << x << ' ' << y << ' ' << l << ' ' << v << '\n';
				ans[x][y] = v;
			}
		}
	}

	for (int l = 1; l <= 16; l++) {
		memset(dp, 0, sizeof(dp));
		dp[0][0] = 1;

		for (int s = 1; s <= 4000; s++) {
			for (int x = max(s-2000, 0); x <= min(s, 2000); x++) {
				int y = s-x;
				if (y >= 1) {
					dp[y][x] = (s > 1 ? dp[y-1][x] : 0) + dp[x][y-1];
					if (y >= l+1) dp[y][x] -= dp[x][y-l-1];
				}
			}
		}

		for (int x = 1; x <= 2000; x++) {
			for (int y = 1; y <= 2000; y++) {
				if (x <= y && (y+x) / (x+1) == l) {
					ans[x][y] += dp[x][y];
				}
				if (x >= y && (x+y) / (y+1) == l) {
					ans[y][x] += dp[x][y];
				}
			}
		}
	}
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	precomp();

	int T; cin >> T;
	while (T--) {
		int x, y; cin >> x >> y;
		if (x > y) swap(x, y);
		assert(x <= y);
		cout << ans[x][y] << '\n';
	}

	return 0;
}
