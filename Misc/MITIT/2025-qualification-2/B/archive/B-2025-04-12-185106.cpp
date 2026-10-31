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

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	using num = modnum<int(1e9)+7>;

	const int V = 1e6;
	std::vector<num> fact(V+1);
	fact[0] = 1;
	for (int i = 1; i <= V; i++) fact[i] = fact[i-1] * num(i);
	std::vector<num> ifact(V+1);
	ifact[V] = inv(fact[V]);
	for (int i = V; i >= 1; i--) ifact[i-1] = ifact[i] * num(i);

	auto choose = [&](int n, int r) -> num {
		assert(0 <= r); assert(r <= n);
		return fact[n] * ifact[r] * ifact[n-r];
	};

	int T; cin >> T;
	while (T--) {

		int N, M; cin >> N >> M;

		std::array<std::array<int, 4>, 3> cnts;
		for (int z = 0; z < 3; z++) {
			cnts[z] = {0, 0, 0, N};
		}

		int tgt = (3*N-1) / 2;
		int extra_lo = 3*N/2;
		int extra_hi = 3*N/2;

		int inv_tgt = -1;

		for (int e = 0; e < M; e++) {
			int i, v; cin >> i >> v; i--, v--;
			int t;
			if (v == tgt) t = 2;
			else if (v > tgt) t = 1;
			else if (v < tgt) t = 0;
			else assert(false);
			cnts[i/N][3]--;
			cnts[i/N][t]++;

			if (v == tgt) inv_tgt = i / N;
			if (v < tgt) extra_lo--;
			if (v > tgt) extra_hi--;
		}

		cout << [&]() -> num {
			num ans = 0;
			for (int z_mid = 0; z_mid < 3; z_mid++) {
				if (inv_tgt != -1 && inv_tgt != z_mid) continue;
				auto c_mid = cnts[z_mid];
				num w = 1;
				if (c_mid[2] == 0) {
					if (c_mid[3] == 0) continue;
					w *= c_mid[3];
					c_mid[3]--;
					c_mid[2]++;
				}
				assert(c_mid[2] == 1);

				// if this guy is balanced, then one guy will be low and the other will be high
				int need_1 = N / 2 - c_mid[0];
				int need_2 = N / 2 - c_mid[1];
				if (need_1 < 0 || need_2 < 0) continue;
				assert(c_mid[0] + c_mid[1] + c_mid[2] + c_mid[3] == N);
				assert(need_1 + need_2 == c_mid[3]);
				if (need_1 > extra_lo) continue;
				if (need_2 > extra_hi) continue;
				ans += choose(extra_lo, need_1) * choose(extra_hi, need_2) * fact[(extra_lo - need_1) + (extra_hi - need_2)] * fact[need_1 + need_2];
			}
			return ans;
		}() << '\n';
	}

	return 0;
}
