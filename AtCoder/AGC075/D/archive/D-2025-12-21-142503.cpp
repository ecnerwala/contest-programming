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
			assert(p_cap >= s_cap);
			// [x^(N-2)] 1/(1-x*p_cap)(1-x*s_cap) = 1 / (1-
			if (p_cap == s_cap) {
				return num(N-1) * power(num(p_cap), N-2);
			} else {
				// [x^(N-2)] 1/(1-x*p_cap)(1-x*s_cap) = a / (1-x*p_cap) + b / (1-x*s_cap)
				// a+b = 1, s_cap a + p_cap b = 0 => a = p_cap / (p_cap - s_cap), b = -s_cap / (p_cap - s_cap);
				return (power(num(p_cap), N-1) - power(num(s_cap), N-1)) / num(p_cap - s_cap);
			}
		};
		auto ways_3 = [&](int p_cap, int s_cap) -> num { 
			// [x^(N-3)] 1/(1-x*p_cap)^2(1-x*s_cap)
			assert(p_cap >= s_cap);
			if (p_cap == s_cap) {
				return num(N-1) * num(N-2) / num(2) * power(num(p_cap), N-3);
			} else {
				// [x^(N-3)] 1/(1-x*p_cap)^2(1-x*s_cap) = a / (1-x*p_cap) + b / (1-x*p_cap)^2 + c / (1-x*s_cap)
				// c = 1 / (1-x*p_cap)^2 mod 1-x*s_cap, so c = s_cap^2 / (s_cap - p_cap)^2 = s_cap^2 / (p_cap - s_cap)^2
				// a(1-x*p_cap) + b = 1 / (1-x*s_cap) mod (1-x*p_cap)^2
				// b = p_cap / (p_cap - s_cap)
				// a+b+c = 1 so a = ((p_cap-s_cap)^2 - p_cap(p_cap - s_cap) - s_cap^2) / (p_cap - s_cap)^2 = -p_cap s_cap / (p_cap - s_cap)^2
				return (power(num(s_cap), N-1) - power(num(p_cap), N-2) * num(s_cap) * num(N-1) + power(num(p_cap), N-1) * num(N-2)) / power(num(p_cap - s_cap), 2);
			}
		};

		num ans = 0;
		num prv_pN1 = 0;
		num prv_pN2 = 0;
		for (int m2 = 1; m2 * m2 < K && m2 <= M; m2++) {
			int m1_max = K / m2;
			assert(m2 <= m1_max);
			if (m2 < m1_max) {
				num nxt_pN2 = power(num(m2), N-2);
				num nxt_pN1 = nxt_pN2 * num(m2);
				nxt_pN2 *= num(N-1);

				// m1 = m2
				ans += (nxt_pN1 - prv_pN1 - prv_pN2) * std::min(M, K - m2 * m2);

				// m1 = m2+1.. std::min(M, m1_max - 1)
				int m1_hi = std::min(M, m1_max - 1);
				// K - m1 * m2 >= M <=> m1 <= (K - M) / m2
				int m1_mid = K - m2 * m2 >= M ? std::min((K-M) / m2, m1_hi) : m2;
				num tail_weight = num(M) * (m1_mid - m2) + num(m1_hi - m1_mid) * (K - num(m1_hi + m1_mid + 1) / num(2) * m2);
				ans += (nxt_pN2 - prv_pN2) * tail_weight;

				prv_pN1 = nxt_pN1;
				prv_pN2 = nxt_pN2;
			}
			if (m1_max <= M) {
				int cap = K % m2;
				ans += ways_3(m2-1, cap) * cap;
				if (m2 < m1_max) {
					ans += (ways_2(m2, cap) - ways_2(m2-1, cap)) * cap;
				}
			}
		}
		return ans;
	};

	int N, M, K; cin >> N >> M >> K;
	cout << solve(N, M, K) << '\n';


	return 0;
}
