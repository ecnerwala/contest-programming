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
	struct is_reduced_tag {};

private:
	int v;

public:

	modnum() : v(0) {}
	modnum(int v_, is_reduced_tag) : v(v_) { assert(0 <= v && v < MOD); }
	static modnum from_reduced(int v) { return modnum(v, is_reduced_tag{}); }

	modnum(int64_t v_) : v(int(v_ % MOD)) { if (v < 0) v += MOD; }

	explicit operator int() const { return v; }
	int as_signed() const { return MOD-v > v ? v : v - MOD; }
	friend std::ostream& operator << (std::ostream& out, modnum n) { return out << int(n); }
	friend std::istream& operator >> (std::istream& in, modnum& n) { int64_t v_; in >> v_; n = modnum(v_); return in; }

	friend bool operator == (modnum a, modnum b) { return a.v == b.v; }
	friend bool operator != (modnum a, modnum b) { return a.v != b.v; }

	modnum inv() const { return from_reduced(mod_inv_in_range(v, MOD)); }
	friend modnum inv(modnum m) { return m.inv(); }
	modnum neg() const { return from_reduced(v ? MOD-v : 0); }
	friend modnum neg(modnum m) { return m.neg(); }

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
	modnum& operator += (modnum o) {
		v -= MOD-o.v;
		v = (v < 0) ? v + MOD : v;
		return *this;
	}
	modnum& operator -= (modnum o) {
		v -= o.v;
		v = (v < 0) ? v + MOD : v;
		return *this;
	}
	modnum& operator *= (modnum o) {
		v = int(int64_t(v) * int64_t(o.v) % MOD);
		return *this;
	}
	modnum& operator /= (modnum o) {
		return *this *= o.inv();
	}

	friend modnum operator ++ (modnum& a, int) { modnum r = a; ++a; return r; }
	friend modnum operator -- (modnum& a, int) { modnum r = a; --a; return r; }
	friend modnum operator + (modnum a, modnum b) { return modnum(a) += b; }
	friend modnum operator - (modnum a, modnum b) { return modnum(a) -= b; }
	friend modnum operator * (modnum a, modnum b) { return modnum(a) *= b; }
	friend modnum operator / (modnum a, modnum b) { return modnum(a) /= b; }
};

template <typename T> T power(T a, long long b) {
	assert(b >= 0);
	T r = 1; while (b) { if (b & 1) r *= a; b >>= 1; a *= a; } return r;
}

template <typename num>
num linearRec(const std::vector<num>& S, const std::vector<num>& tr, int64_t k) {
	int n = int(tr.size());
	assert(S.size() >= tr.size());

	auto combine = [&](std::vector<num> a, std::vector<num> b, bool e = false) {
		// multiply a * b * x^e
		std::vector<num> res(int(a.size()) + int(b.size()));
		for (int i = 0; i < int(a.size()); i++) {
			for (int j = 0; j < int(b.size()); j++) {
				res[i + j + e] += a[i] * b[j];
			}
		}
		for (int i = int(res.size())-1; i >= n; --i) {
			for (int j = 0; j < n; j++) {
				res[i - 1 - j] += res[i] * tr[j];
			}
		}
		res.resize(n);
		return res;
	};

	std::vector<num> pol(n);
	if (n > 0) pol[0] = num(1);

	assert(k >= 0);
	for (int i = 64 - 1 - (k == 0 ? 64 : __builtin_clzll(k)); i >= 0; i--) {
		pol = combine(pol, pol, (k >> i) & 1);
	}

	num res = 0;
	for (int i = 0; i < n; i++) res += pol[i] * S[i];
	return res;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	using num = modnum<998244353>;

	auto solve = [&](int N, int M, int K) -> num {
		assert(N >= 3);

		auto ways_2 = [&](int p_cap, int s_cap) -> num { 
			return linearRec(
				std::vector<num>({num(0), num(1)}),
				std::vector<num>({num(p_cap) + num(s_cap), -num(p_cap) * num(s_cap)}),
				N-1
			);
		};
		auto ways_3 = [&](int p_cap, int m_cap, int s_cap) -> num { 
			return linearRec(
				std::vector<num>({num(0), num(0), num(1)}),
				std::vector<num>({num(p_cap) + num(m_cap) + num(s_cap), -num(p_cap) * num(s_cap) - num(p_cap) * num(m_cap) - num(m_cap) * num(s_cap), num(p_cap) * num(m_cap) * num(s_cap)}),
				N-1
			);
		};

		num ans = 0;
		for (int mx = 1; mx <= M; mx++) {
			if (mx * mx + mx <= K) {
				num ways_eq_max = power(num(mx), N-1) - power(num(mx-1), N-1);
				for (int j = 1; j < mx && j * mx < K; j++) {
					num ways_eq_max_2 = num(N-1) * (power(num(j), N-2) - power(num(j-1), N-2));
					ways_eq_max -= ways_eq_max_2;
					ans += ways_eq_max_2 * num(std::min(M, K - mx * j));
				}
				ans += ways_eq_max * num(std::min(M, K - mx * mx));
			} else if (mx * mx < K) {
				// mx * (mx-1) + (mx-1) = mx * mx - 1 < K, if everything else is smaller, we're still okay
				for (int j = 1; j < mx; j++) {
					num ways_eq_max_2 = num(N-1) * (power(num(j), N-2) - power(num(j-1), N-2));
					ans += ways_eq_max_2 * num(std::min(M, K - mx * j));
				}

				// if we have exactly 2 of them, then pref and suff must both be small, mid can be anything
				int cap = K - mx * mx;
				assert(cap < mx);
				assert(cap >= 1);
				// [x^N-3] 1 / (1 - x * cap)^2 * 1 / (1-x)
				// (1 - x (cap + cap + 1) + x^2 (cap^2 + 2*cap) - cap^2 x^3)
				ans += ways_3(cap, 1, cap) * num(cap);
			} else {
				// we're only allowed to have 1 of them
				assert(mx * mx >= K);
				for (int j = 1; j * mx < K; j++) {
					assert(j < mx);
					if (j * mx + j <= K) {
						assert(j * j + mx <= K);
						num ways_eq_max_2 = num(N-1) * (power(num(j), N-2) - power(num(j-1), N-2));
						ans += ways_eq_max_2 * num(std::min(M, K - mx * j));
					} else {
						assert((j-1) * mx + j <= K);

						// 3 2 2 1
						// 1 3 2 1

						int cap = K - j * mx;
						assert(cap < j);
						assert(cap * cap + mx <= K);
						// if there's a j after mx, then all other js are banned
						// it's j-1 mx j-1 j cap
						// prefix max < j, suffix <= j and has 
						ans += ways_3(j-1, j-1, cap) * num(cap);

						// j * j + mx == j * mx + 1 + ((mx-1) - (mx-j) * j) <= j * mx + 1 <= K
						assert(j * j + mx <= K);

						// prefix max == j, suffix <= K - j * mx
						ans += (ways_2(j, cap) - ways_2(j-1, cap)) * num(cap);
					}
					//cerr << mx << ' ' << j << ' ' << ans << '\n';
				}
			}
			//cerr << mx << ' ' << ans << '\n';
		}
		cerr << "ans " << ans << '\n';
		return ans;
	};

	auto slow = [&](int N, int M, int K) -> num {
		assert(N >= 3);
		num ans = 0;
		[&](this auto&& self, int idx, int m1, int m2) -> void {
			if (idx == N) {
				ans++;
			}
			for (int v = 1; v <= M && v+m2 <= K; v++) {
				self(idx+1, std::max(m1, v), std::max(m1 * v, m2));
			}
		}(0, 0, 0);
		return ans;
	};
	for (int N = 3; N <= 10; N++) {
		for (int M = 1; M <= 5; M++) {
			for (int K = 1; K <= 5; K++) {
				auto a = solve(N, M, K);
				auto b = slow(N, M, K);
				if (a != b) {
					cerr << N << ' ' << M << ' ' << K << '\n';
					cerr << a << '\n';
					cerr << b << '\n';
				}
			}
		}
	}

	int N, M, K; cin >> N >> M >> K;
	cout << solve(N, M, K) << '\n';


	return 0;
}
