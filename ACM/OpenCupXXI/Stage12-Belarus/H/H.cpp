#include <bits/stdc++.h>

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
		v -= MOD-o.v;
		v = (v < 0) ? v + MOD : v;
		return *this;
	}
	modnum& operator -= (const modnum& o) {
		v -= o.v;
		v = (v < 0) ? v + MOD : v;
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

using num = modnum<998244353>;
num sq(num a) { return a * a; }

num sum_sq(int64_t a) {
	return num(a) * num(a+1) * num(2*a+1) / num(6);
}

num go(int64_t x1, int64_t y1, int64_t x2, int64_t y2) {
	using namespace std;

	if (x1 == x2) {
		if (y1 <= y2) {
			num ans = sum_sq(y2-1) - sum_sq(y1-1) + num(y2-y1) * sq(num(x1));
			//cerr << x1 << ' ' << y1 << ' ' << x2 <<  ' ' << y2 << ' ' << ans << '\n';
			return ans;
		} else {
			num ans = sum_sq(y1) - sum_sq(y2) + num(y1-y2) * sq(num(x1));
			//cerr << x1 << ' ' << y1 << ' ' << x2 <<  ' ' << y2 << ' ' << ans << '\n';
			return ans;
		}
	}
	if (y1 == y2) {
		return go(y1, x1, y2, x2);
	}

	assert(x1 != x2);
	assert(y1 != y2);

	if (x1 > x2) {
		return go(x2, y2, x1, y1) - sq(num(x2)) - sq(num(y2)) + sq(num(x1)) + sq(num(y1));
	}
	assert(x1 < x2);
	if (y1 > y2) {
		return go(x1, y1, x1, y2) + go(x1, y2, x2, y2);
	}
	assert(x1 < x2 && y1 < y2);
	if (x1 > y1) {
		swap(x1, y1);
		swap(x2, y2);
	}
	assert(x1 <= y1);
	if (x2 <= y1) {
		return go(x1, y1, x2, y1) + go(x2, y1, x2, y2);
	}
	int64_t p1 = y1;
	int64_t p2 = min(x2, y2);
	num ans = go(x1, y1, p1, p1) + go(p2, p2, x2, y2);
	ans += 3 * (sum_sq(p2-1) - sum_sq(p1-1)) + (sum_sq(p2) - sum_sq(p1));
	return ans;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	/*
	for (int64_t x0 = 0; x0 <= 1000; x0++) {
		for (int64_t x1 = x0; x1 <= 1000; x1++) {
			for (int64_t y = 1; y <= 1000; y++) {
				int64_t diff = x0 * x0 + y * y + x1 * x1 + (y-1)*(y-1) - (2*y-1) * (x1 - x0);
				assert(diff >= 0);
			}
		}
	}
	*/
	int T; cin >> T;
	while (T--) {
		int64_t x1, y1, x2, y2; cin >> x1 >> y1 >> x2 >> y2;
		num ans = go(x1, y1, x2, y2);
		//cerr << ans << '\n';
		ans += sq(num(x2) * num(y2)) - sq(num(x1) * num(y1));
		//cerr << ans << '\n';
		cout << ans << '\n';
	}

	return 0;
}
