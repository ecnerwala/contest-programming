#include <bits/stdc++.h>

template <typename T> T mod_inv_in_range(T a, T m) {
	// assert(0 <= a && a < m);
	T x = a, y = m;
	// abs coeff of a in x and y (they're always opposite sign)
	T vx = 1, vy = 0;
	bool swap = false;
	while (x) {
		T k = y / x;
		y %= x;
		vy += k * vx;
		std::swap(x, y);
		std::swap(vx, vy);
		swap ^= 1;
	}
	assert(y == 1);
	return swap ? vy : m - vy;
}

template <int MOD_> struct modnum {
	// Uses subtraction to support MOD up to 2^31 - 1
	static constexpr int MOD = MOD_;
	static_assert(MOD_ > 0, "MOD must be positive");
	struct is_reduced_tag {};

	int v;
	modnum() : v(0) {}
	modnum(int v_, is_reduced_tag) : v(v_) { assert(0 <= v && v < MOD); }
	static modnum from_reduced(int v) { return modnum(v, is_reduced_tag{}); }

	modnum(int v_) : v(int(v_ % MOD)) { if (v < 0) v += MOD; }
	modnum(unsigned v_) : v(int(v_ % MOD)) { }
	modnum(int64_t v_) : v(int(v_ % MOD)) { if (v < 0) v += MOD; }
	modnum(uint64_t v_) : v(int(v_ % MOD)) { }

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

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int T; std::cin >> T;
	while (T--) {
		int N, M; std::cin >> N >> M;

		using num = modnum<676767677>;
		const int V = N + M + 1;
		std::vector<num> fact(V+1);
		fact[0] = 1;
		for (int i = 1; i <= V; i++) fact[i] = fact[i-1] * num(i);
		std::vector<num> ifact(V+1);
		ifact[V] = inv(fact[V]);
		for (int i = V; i >= 1; i--) ifact[i-1] = ifact[i] * num(i);

		auto ways_with_eq = [&](int a) -> num {
			int cnt = N - a + 1;
			// cnt things with M values
			return fact[cnt + M - 1] * ifact[cnt] * ifact[M - 1];
		};

		num ans = 0;
		int l_big = N;
		int l_small = N-1;
		int cnt_big = 1;
		int cnt_small = 0;
		for (int cost = 1; l_big > 0; cost++) {
			std::cerr << N << ' ' << l_big << ' ' << cnt_big << ' ' << l_small << ' ' << cnt_small << '\n';
			assert(cnt_big > 0);
			if (cost == 1) {
				// it's always 1
				ans += num(cost) * num(cnt_big + cnt_small) * ways_with_eq(1);
			} else {
				// we subtract 1 big and 1 small, if they exist
				assert(cnt_big + cnt_small >= 2);
				if (cnt_small == 0) {
					l_small = l_big;
					cnt_small = 1;
					cnt_big--;
				}
				std::cerr << N << ' ' << l_big << ' ' << cnt_big << ' ' << l_small << ' ' << cnt_small << '\n';
				ans += num(cost) * num(cnt_big) * ways_with_eq(1);
				ans -= num(cost) * num(cnt_big-1) * ways_with_eq((l_big - 0) / 2 + 2);
				ans -= num(cost) * num(cnt_big) * ways_with_eq((l_big - 1) / 2 + 2);
				ans += num(cost) * num(cnt_big-1) * ways_with_eq(l_big + 2);

				if (l_small > 0) {
					ans += num(cost) * num(cnt_small) * ways_with_eq(1);
					ans -= num(cost) * num(cnt_small) * ways_with_eq((l_small - 0) / 2 + 2);
					ans -= num(cost) * num(cnt_small-1) * ways_with_eq((l_small - 1) / 2 + 2);
					ans += num(cost) * num(cnt_small-1) * ways_with_eq(l_small + 2);
				}
			}

			assert(l_big - l_small <= 1);
			if (l_small == 0) {
				break;
			}
			int nl_big = l_big / 2;
			int nl_small = nl_big - 1;
			int ncnt_big = 0;
			int ncnt_small = 0;
			(((l_big - 0) / 2) == nl_big ? ncnt_big : ncnt_small) += cnt_big;
			(((l_big - 1) / 2) == nl_big ? ncnt_big : ncnt_small) += cnt_big;
			(((l_small - 0) / 2) == nl_big ? ncnt_big : ncnt_small) += cnt_small;
			(((l_small - 1) / 2) == nl_big ? ncnt_big : ncnt_small) += cnt_small;
			l_big = nl_big;
			l_small = nl_small;
			cnt_big = ncnt_big;
			cnt_small = ncnt_small;
		}
		std::cout << ans << '\n';
	}

	return 0;
}
