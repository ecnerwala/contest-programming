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

	using num = modnum<998244353>;

	const int V = int(1e7);
	std::vector<num> fact(V+1);
	fact[0] = 1;
	for (int i = 1; i <= V; i++) fact[i] = fact[i-1] * num(i);
	std::vector<num> ifact(V+1);
	ifact[V] = inv(fact[V]);
	for (int i = V; i >= 1; i--) ifact[i-1] = ifact[i] * num(i);

	auto mix_ways = [&](int a, int b, int c) -> num {
		return fact[a+b+c] * ifact[a] * ifact[b] * ifact[c];
	};

	auto extra_ways = [&](int a, int segs) -> num {
		if (segs == 0) {
			return a == 0 ? 1 : 0;
		} else if (segs == 1) {
			return 1;
		} else if (segs == 2) {
			return a+1;
		} else assert(false);
	};

	auto solve = [&](int A, int B, int C, int D) -> num {
		assert(A + B + C + D > 0);

		int tot_len = 2 * A + B - C - 2 * D;
		if (tot_len == 0) return 0;
		if (tot_len < 0) {
			tot_len *= -1;
			std::swap(A, D);
			std::swap(B, C);
		}

		assert(tot_len > 0);

		num ans = 0;
		for (int bot_shift = -1; bot_shift <= 1; bot_shift ++) {
			for (int top_shift = -1; top_shift <= 1; top_shift ++) {
				int cur_A = A, cur_B = B, cur_C = C, cur_D = D;
				int extra_segs = 0;
				if (bot_shift == -1) {
					cur_C--;
					cur_A--;
					extra_segs++;
				} else if (bot_shift == 1) {
					cur_B--;
					cur_A--;
					cur_D--;
					extra_segs++;
				}

				if (top_shift == -1) {
					cur_C--;
					cur_A--;
					extra_segs++;
				} else if (top_shift == 1) {
					cur_B--;
					cur_A--;
					cur_D--;
					extra_segs++;
				}

				if (cur_A < 0 || cur_B < 0 || cur_C < 0 || cur_D < 0) continue;

				int dist = tot_len - extra_segs;
				if (dist < 0) continue;

				cur_A -= 2 * cur_C;
				if (cur_A < 0) continue;

				int mid_A = dist - 3 * cur_C - cur_B;
				assert(mid_A % 2 == 0);
				mid_A /= 2;
				assert(cur_A - mid_A == cur_D);
				if (mid_A < 0) continue;

				ans += mix_ways(cur_B, cur_C, mid_A) * extra_ways(cur_D, extra_segs);
			}
		}
		return ans;
	};

	int T; cin >> T;
	while (T--) {
		int A, B, C, D; cin >> A >> B >> C >> D;
		cout << solve(A, B, C, D) << '\n';
	}

	return 0;
}
