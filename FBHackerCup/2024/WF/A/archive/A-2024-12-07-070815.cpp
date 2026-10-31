#include<bits/stdc++.h>

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

template <typename T> T pow(T a, long long b) {
	assert(b >= 0);
	T r = 1; while (b) { if (b & 1) r *= a; b >>= 1; a *= a; } return r;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		using num = modnum<int(1e9)+7>;
		int N, M1, M2, H, S; cin >> N >> M1 >> M2 >> H >> S;
		cerr << "Case #" << case_num << " " << N << ' ' << M1 << ' ' << M2 << ' ' << H << ' ' << S << '\n';

		num ans = 0;

		std::vector<num> fact(N+1);
		fact[0] = 1;
		for (int i = 1; i <= N; i++) fact[i] = fact[i-1] * num(i);
		std::vector<num> ifact(N+1);
		ifact[N] = inv(fact[N]);
		for (int i = N; i >= 1; i--) ifact[i-1] = ifact[i] * num(i);

		num ways = fact[N] * ifact[H] * ifact[N-H] * pow(num(S), N) * pow(num(S-1), H);

		std::vector<num> pow_S2(H+1);
		pow_S2[0] = 1;
		for (int i = 1; i <= H; i++) pow_S2[i] = pow_S2[i-1] * num(S-2);

		auto eval = [&](int a1, int a2) -> num {
			assert(a1 >= 0 && a2 >= 0);
			if (a1 + a2 > H) return 0;
			return fact[H] * ifact[H - a1 - a2] * ifact[a1] * ifact[a2] * pow_S2[H - a1 - a2];
		};
		std::array<int, 2> prv_s{0, 0};
		auto eval_side = [&](int z) -> num {
			num r = 0;
			for (int a = prv_s[!z]; a + prv_s[z] <= H; a++) {
				r += eval(prv_s[z], a);
			}
			return r;
		};
		num cur_tri = pow(num(S), H);
		std::array<num, 2> side_sum{eval_side(0), eval_side(1)};

		for (int a = 0; a <= N-H && a <= std::min(M1, M2); a++) {
			// we choose a of the N-H locations to mismatch
			num w = fact[N-H] * ifact[a] * ifact[N-H-a];
			w *= pow(num(S-1), a);
			int r1 = M1 - a;
			int r2 = M2 - a;
			// The remaining H locations have to mismatch one or both sides
			if (r1 + r2 < H) continue;
			std::array<int, 2> tgt_s{std::max(H - r1, 0), std::max(H - r2, 0)};
			cerr << tgt_s[0] << ' ' << tgt_s[1] << '\n';
			for (int z = 0; z < 2; z++) {
				assert(tgt_s[z] >= prv_s[z]);
				while (prv_s[z] < tgt_s[z]) {
					cur_tri -= side_sum[z];
					if (num(S-2) == 0) {
						side_sum[z] = eval(prv_s[z] + 1, H - prv_s[z] + 1);
					} else {
						side_sum[z] = (side_sum[z] - eval(prv_s[z], H - prv_s[z])) / num(S-2) - (side_sum[z] - eval(prv_s[z], prv_s[!z]));
					}
					side_sum[!z] -= eval(prv_s[z], prv_s[!z]);
					prv_s[z]++;
					assert(side_sum[!z] == eval_side(!z));
					assert(side_sum[z] == eval_side(z));
				}
			}
			ans += w * cur_tri;
		}

		ans *= ways;

		cout << "Case #" << case_num << ": " << ans << '\n';
	}

	return 0;
}
