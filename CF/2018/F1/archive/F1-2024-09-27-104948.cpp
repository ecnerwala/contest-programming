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

template <typename T> T pow(T a, long long b) {
	assert(b >= 0);
	T r = 1; while (b) { if (b & 1) r *= a; b >>= 1; a *= a; } return r;
}

template <typename tag> struct dynamic_modnum {
private:
#if __cpp_inline_variables >= 201606
	// C++17 and up
	inline static int MOD_ = 0;
	inline static uint64_t BARRETT_M = 0;
#else
	// NB: these must be initialized out of the class by hand:
	//   static int dynamic_modnum<tag>::MOD = 0;
	//   static int dynamic_modnum<tag>::BARRETT_M = 0;
	static int MOD_;
	static uint64_t BARRETT_M;
#endif

public:
	// Make only the const-reference public, to force the use of set_mod
	static constexpr int const& MOD = MOD_;

	// Barret reduction taken from KACTL:
	/**
	 * Author: Simon Lindholm
	 * Date: 2020-05-30
	 * License: CC0
	 * Source: https://en.wikipedia.org/wiki/Barrett_reduction
	 * Description: Compute $a \% b$ about 5 times faster than usual, where $b$ is constant but not known at compile time.
	 * Returns a value congruent to $a \pmod b$ in the range $[0, 2b)$.
	 * Status: proven correct, stress-tested
	 * Measured as having 4 times lower latency, and 8 times higher throughput, see stress-test.
	 * Details:
	 * More precisely, it can be proven that the result equals 0 only if $a = 0$,
	 * and otherwise lies in $[1, (1 + a/2^64) * b)$.
	 */
	static void set_mod(int mod) {
		assert(mod > 0);
		MOD_ = mod;
		BARRETT_M = (uint64_t(-1) / MOD);
	}
	static uint32_t barrett_reduce_partial(uint64_t a) {
		return uint32_t(a - uint64_t((__uint128_t(BARRETT_M) * a) >> 64) * MOD);
	}
	static int barrett_reduce(uint64_t a) {
		int32_t res = int32_t(barrett_reduce_partial(a) - MOD);
		return (res < 0) ? res + MOD : res;
	}

	struct mod_reader {
		friend std::istream& operator >> (std::istream& i, mod_reader) {
			int mod; i >> mod;
			dynamic_modnum::set_mod(mod);
			return i;
		}
	};
	static mod_reader MOD_READER() {
		return mod_reader();
	}

private:
	int v;

public:

	dynamic_modnum() : v(0) {}
	dynamic_modnum(int64_t v_) : v(int(v_ % MOD)) { if (v < 0) v += MOD; }
	explicit operator int() const { return v; }
	friend std::ostream& operator << (std::ostream& out, const dynamic_modnum& n) { return out << int(n); }
	friend std::istream& operator >> (std::istream& in, dynamic_modnum& n) { int64_t v_; in >> v_; n = dynamic_modnum(v_); return in; }

	friend bool operator == (const dynamic_modnum& a, const dynamic_modnum& b) { return a.v == b.v; }
	friend bool operator != (const dynamic_modnum& a, const dynamic_modnum& b) { return a.v != b.v; }

	dynamic_modnum inv() const {
		dynamic_modnum res;
		res.v = mod_inv_in_range(v, MOD);
		return res;
	}
	friend dynamic_modnum inv(const dynamic_modnum& m) { return m.inv(); }
	dynamic_modnum neg() const {
		dynamic_modnum res;
		res.v = v ? MOD-v : 0;
		return res;
	}
	friend dynamic_modnum neg(const dynamic_modnum& m) { return m.neg(); }

	dynamic_modnum operator- () const {
		return neg();
	}
	dynamic_modnum operator+ () const {
		return dynamic_modnum(*this);
	}

	dynamic_modnum& operator ++ () {
		v ++;
		if (v == MOD) v = 0;
		return *this;
	}
	dynamic_modnum& operator -- () {
		if (v == 0) v = MOD;
		v --;
		return *this;
	}
	dynamic_modnum& operator += (const dynamic_modnum& o) {
		v -= MOD-o.v;
		v = (v < 0) ? v + MOD : v;
		return *this;
	}
	dynamic_modnum& operator -= (const dynamic_modnum& o) {
		v -= o.v;
		v = (v < 0) ? v + MOD : v;
		return *this;
	}
	dynamic_modnum& operator *= (const dynamic_modnum& o) {
		v = barrett_reduce(int64_t(v) * int64_t(o.v));
		return *this;
	}
	dynamic_modnum& operator /= (const dynamic_modnum& o) {
		return *this *= o.inv();
	}

	friend dynamic_modnum operator ++ (dynamic_modnum& a, int) { dynamic_modnum r = a; ++a; return r; }
	friend dynamic_modnum operator -- (dynamic_modnum& a, int) { dynamic_modnum r = a; --a; return r; }
	friend dynamic_modnum operator + (const dynamic_modnum& a, const dynamic_modnum& b) { return dynamic_modnum(a) += b; }
	friend dynamic_modnum operator - (const dynamic_modnum& a, const dynamic_modnum& b) { return dynamic_modnum(a) -= b; }
	friend dynamic_modnum operator * (const dynamic_modnum& a, const dynamic_modnum& b) { return dynamic_modnum(a) *= b; }
	friend dynamic_modnum operator / (const dynamic_modnum& a, const dynamic_modnum& b) { return dynamic_modnum(a) /= b; }
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;

	while (T--) {
		struct my_tag{};
		using num = dynamic_modnum<my_tag>;
		int N; cin >> N >> num::MOD_READER();

		std::vector<num> ans(N+1);
		std::vector<std::array<num, 3>> dp(N);
		dp[N-1][2] = 1;
		for (int h = N-1; h >= 0; h--) {
			std::vector<std::array<num, 3>> ndp(h);
			for (int w = 0; w <= h; w++) {
				for (int in_slope = 0; in_slope <= 2; in_slope++) {
					for (int out_slope = 0; out_slope <= 2; out_slope++) {
						num ways = dp[w][in_slope];
						if (ways == 0) continue;
						if (w == 0) {
							// invalid to have out_slope == 1
							if (out_slope == 1) continue;

							if (in_slope == 2 && out_slope == 2) {
								ways *= num(N-h);
							}
						} else {
							// multiply by (N-h) for each in_slope / out_slope matching pair
							if (in_slope == 1 && out_slope == 1) {
								ways *= num((N-h) + 1);
							} else {
								if (in_slope >= 2 && out_slope >= 2) {
									ways *= num(N-h) * num(N-h);
								} else if (in_slope >= 1 && out_slope >= 1) {
									ways *= num(N-h);
								}
								if (out_slope == 1) {
									ways *= num(2);
								}
							}
						}
						if (w - out_slope < 0) {
							assert(out_slope == 2);
							ans[2 * h - w + 1] += ways;
						} else if (w - out_slope <= h - 1) {
							ndp[w - out_slope][out_slope] += ways;
						} else {
							assert(out_slope == 0);
						}
					}
				}
			}
			dp = std::move(ndp);
		}

		ans[0] = pow(num(N), N) - std::accumulate(ans.begin() + 1, ans.begin() + N + 1, num(0));
		for (int i = 0; i <= N; i++) {
			cout << ans[i] << " \n"[i==N];
		}
	}

	return 0;
}
