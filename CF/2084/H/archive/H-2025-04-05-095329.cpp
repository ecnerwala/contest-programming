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
		int N; cin >> N;
		std::string S; S.reserve(N); cin >> S; assert(int(S.size()) == N);

		assert(N >= 1);

		std::vector<int> flips; flips.reserve(N+1);
		flips.push_back(0);
		flips.push_back(1);
		for (int i = 1; i < N; i++) {
			if (S[i] != S[i-1]) flips.push_back(i+1);
		}
		int tail_len = N+1 - flips.back();

		using num = modnum<int(1e9)+7>;
		int F = int(flips.size());
		std::vector<num> dp(F);
		std::array<int, 2> offset{0, 0};
		// len - offset, dp value
		std::array<std::vector<std::pair<int, num>>, 2> stk;
		std::array<num, 2> stk_val{0, 0};
		stk[0].reserve(F);
		stk[1].reserve(F);
		dp[0] = 1;
		dp[1] = 1;
		for (int i = 2; i < F; i++) {
			// first, add the option of i-1 .. i
			int z = i & 1;
			offset[z]++;
			int len = flips[i] - flips[i-1] - offset[z];
			while (!stk[z].empty() && len >= stk[z].back().first) {
				auto [l, v] = stk[z].back();
				stk[z].pop_back();
				if (!stk[z].empty()) stk_val[z] -= (stk[z].back().first - l) * stk[z].back().second;
			}
			if (!stk[z].empty()) stk_val[z] += (stk[z].back().first - len) * stk[z].back().second;
			stk[z].push_back({len, dp[i-1]});
			dp[i] = stk_val[z] + (stk[z].back().first + offset[z]) * stk[z].back().second;
			if (i & 1) {
				// jump directly to 0 is a special case since that's the only way to end the sequence
				dp[i] += num(offset[z]) * dp[0];
			}
			cerr << i << ' ' << flips[i] << ' ' << dp[i] << '\n';
		}

		num ans = dp.back();
		ans *= num(tail_len);
		if (int(flips.size()) == 2) ans--;
		cout << ans << '\n';
	}


	return 0;
}
