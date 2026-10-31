#include <bits/stdc++.h>

template <typename T> T mod_inv_in_range(T a, T m) {
	// assert(0 <= a && a < m);
	T x = a, y = m;
	T vx = 1, vy = 0;
	while (x) {
		T k = y / x;
		y %= x;
		vy -= k * vx;
		std::swap(x, y);
		std::swap(vx, vy);
	}
	assert(y == 1);
	return vy < 0 ? m + vy : vy;
}

template <typename T> T mod_inv(T a, T m) {
	a %= m;
	a = a < 0 ? a + m : a;
	return mod_inv_in_range(a, m);
}

template <int MOD_> struct modnum {
	static constexpr int MOD = MOD_;
	static_assert(MOD_ > 0, "MOD must be positive");

private:
	using ll = long long;

	int v;

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
		res.v = mod_inv_in_range(v, MOD);
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

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	const int V = 1e6;
	std::vector<int> pfac(V+1);
	std::vector<int> primes; primes.reserve(V);
	for (int i = 2; i <= V; i++) {
		if (pfac[i] == 0) {
			primes.push_back(i);
			pfac[i] = i;
		}
		for (int p : primes) {
			if (p > V / i) break;
			if (p > pfac[i]) break;
			pfac[p*i] = p;
		}
	}

	int N; cin >> N;

	using mnum = modnum<998244353>;
	mnum ans = 1;
	mnum exc = 1;

	std::vector<int> num_mult(V+1);
	std::vector<std::vector<std::map<int, int>>> mps(V+1);

	for (int i = 0; i < N; i++) {
		int x, y; cin >> x >> y;
		int g = std::gcd(x, y);
		x /= g, y /= g;

		ans *= pow(mnum(y), N-1);

		for (int cur_y = y; cur_y > 1; ) {
			int p = pfac[cur_y];
			int e = 0;
			int q = 1;
			while (cur_y % p == 0) q *= p, e++, cur_y /= p;

			if (mps[q].empty()) {
				mps[q].resize(e+1);
			}
			int v = int(int64_t(x) * int64_t(mod_inv(y / q, q)) % q);

			int64_t epow = 0;

			for (int cur_e = 1, cur_q = 1; cur_e <= e; cur_e++) {
				cur_q *= p;
				int cur_v = v % cur_q;
				epow += num_mult[cur_q]++;
				epow += mps[q][cur_e][cur_v]++;
			}
			exc *= pow(mnum(p), epow);
		}
	}

	ans /= exc;
	cout << ans << '\n';

	return 0;
}
