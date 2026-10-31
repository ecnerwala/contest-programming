#include <bits/stdc++.h>

template <typename T> T mod_inv_in_range(T a, T m) {
	// assert(0 <= a && a < m);
	T x = a, y = m;
	// coeff of a in x and y
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

template <typename T> struct extended_gcd_result {
	T gcd;
	T coeff_a, coeff_b;
};
template <typename T> extended_gcd_result<T> extended_gcd(T a, T b) {
	T x = a, y = b;
	// coeff of a and b in x and y
	T ax = 1, ay = 0;
	T bx = 0, by = 1;
	while (x) {
		T k = y / x;
		y %= x;
		ay -= k * ax;
		by -= k * bx;
		std::swap(x, y);
		std::swap(ax, ay);
		std::swap(bx, by);
	}
	return {y, ay, by};
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
	int v;

public:

	modnum() : v(0) {}
	modnum(int64_t v_) : v(int(v_ % MOD)) { if (v < 0) v += MOD; }
	explicit operator int() const { return v; }
	friend std::ostream& operator << (std::ostream& out, const modnum& n) { return out << int(n); }
	friend std::istream& operator >> (std::istream& in, modnum& n) { int64_t v_; in >> v_; n = modnum(v_); return in; }

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
		v = int(int64_t(v) * int64_t(o.v) % MOD);
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

template <typename T> T power(T a, long long b) {
	assert(b >= 0);
	T r = 1; while (b) { if (b & 1) r *= a; b >>= 1; a *= a; } return r;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	const int V = int(1.1e6);
	std::vector<int> primes; primes.reserve(V);
	std::vector<int> pfac(V+1);
	for (int i = 2; i <= V; i++) {
		if (!pfac[i]) {
			primes.push_back(i);
			pfac[i] = i;
		}
		for (int p : primes) {
			if (int64_t(i) * int64_t(p) > V) break;
			pfac[i * p] = p;
			if (p == pfac[i]) break;
		}
	}

	int T; cin >> T;
	while (T--) {
		std::array<int, 3> X;
		cin >> X[0] >> X[1] >> X[2];
		std::vector<std::pair<int, int>> pfacs;
		{
			std::array<int, 3> X_tmp = X;
			for (int z = 0; z < 3; z++) {
				while (X_tmp[z] > 1) {
					int p = pfac[X_tmp[z]];
					int e = 0;
					for (auto& x : X_tmp) { while (x % p == 0) { x /= p, e++; } }
					assert(e >= 1);
					pfacs.push_back({p, e});
				}
			}
		}
		using num = modnum<998244353>;
		num tot_cnt = 1;
		std::map<int, int> tot_qs;
		for (auto [p, e] : pfacs) {
			// we need a != 0 mod p
			tot_cnt *= p-1;
			for (int v = p-1; v > 1; ) {
				int q = pfac[v];
				int e_q = 0;
				while (v % q == 0) { v /= q; e_q++; }
				assert(e_q >= 1);
				tot_qs[q] += e_q;
			}
		}
		tot_cnt /= num(X[0]);
		tot_cnt /= num(X[1]);
		tot_cnt /= num(X[2]);
		for (auto [q, e] : tot_qs) {
			num inv_q = inv(num(q));
			num prob_no_q = power(inv_q, e);
			tot_cnt *= prob_no_q + (1 - prob_no_q) * (X[0] % q == 0 || X[1] % q == 0 || X[2] % 2 == 0 ? 0 : inv_q);
		}
		cout << tot_cnt << '\n';
	}

	return 0;
}

// b_n == 1 mod m <=> gcd(a, m) == 1 and b_{n-1} == 0 mod ord_m(a)
// b_{n-1} = a^huge, so b_{n-1} == 0 <=> p | a for all p | ord_m(a)
// ord_m(a)
