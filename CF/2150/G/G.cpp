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

	int T; cin >> T;
	while (T--) {
		int X, Y, K; cin >> X >> Y >> K;
		int N = X + Y;
		std::string A; cin >> A; assert(int(A.size()) == N);
		using num = modnum<998244353>;
		std::vector<num> fact(N+1);
		fact[0] = 1;
		for (int i = 1; i <= N; i++) fact[i] = fact[i-1] * num(i);
		std::vector<num> ifact(N+1);
		ifact[N] = inv(fact[N]);
		for (int i = N; i >= 1; i--) ifact[i-1] = ifact[i] * num(i);

		auto choose = [&](int n, int r) -> num {
			return (0 <= r && r <= n) ? fact[n] * ifact[r] * ifact[n-r] : 0;
		};

		struct state_t {
			int b_st;
			int b_mi;
			int b_stmima;
			int b_stmimami;
			int b_cur;
			int len;
			int b_en;
		};

		auto ways_bounce = [&](int max_range, int len, int num_pos, int num_neg, int st_val) -> num {
			return 0;
		};
		
		auto solve = [&](state_t s) -> num {
			int base_pos = (s.b_en - s.b_cur + s.len) / 2;
			int base_neg = (-(s.b_en - s.b_cur) + s.len) / 2;
			if (base_pos < 0 || base_neg < 0) {
				return 0;
			}
			num ans = choose(s.len, base_pos);
			if ((s.b_stmimami + s.b_en + N) / 2 >= K) {
				return ans;
			}
			for (int b_mi = std::min(s.b_cur, s.b_en); true; b_mi--) {
				if ((s.b_stmima - 2 * b_mi + s.b_en + N) / 2 >= K) {
					break;
				}
				int cur_val = (s.b_st - 2 * std::min(s.b_mi, b_mi) + s.b_en + N) / 2;
				assert(cur_val <= K-1);
				int extra_gap = K - 1 - cur_val;
				int st_val;
				int num_pos = base_pos;
				int num_neg = base_neg;
				if (b_mi >= s.b_mi) {
					// then we're already going up
					st_val = s.b_cur - b_mi;
					num_pos -= s.b_en - b_mi;
				} else {
					st_val = s.b_cur - s.b_mi;
					num_neg -= s.b_mi - b_mi;
					num_pos -= s.b_en - b_mi;
				}
				if (num_pos < 0 || num_neg < 0) continue;
				ans -= ways_bounce(extra_gap, s.len, num_pos, num_neg, st_val);
			}
			return ans;
		};

		auto advance = [&](state_t s, char c) -> state_t {
			if (c == '0') s.b_cur --;
			else if (c == '1') s.b_cur ++;
			else assert(false);

			assert(s.len > 0);
			s.len--;

			s.b_mi = std::min(s.b_mi, s.b_cur);
			s.b_stmima = std::max(s.b_stmima, s.b_st - 2 * s.b_mi + 2 * s.b_cur);
			s.b_stmimami = std::max(s.b_stmimami, s.b_stmima - 2 * s.b_cur);
			return s;
		};

		state_t cur{0, 0, 0, 0, 0, N, Y - X};

		num ans = 0;
		for (char c : A) {
			if (c == '0') {
				ans += solve(advance(cur, '1'));
			}
			cur = advance(cur, c);
		}
		cout << ans << '\n';
	}

	return 0;
}
