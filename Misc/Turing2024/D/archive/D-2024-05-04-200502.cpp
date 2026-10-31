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

template <typename T> T pow(T a, long long b) {
	assert(b >= 0);
	T r = 1; while (b) { if (b & 1) r *= a; b >>= 1; a *= a; } return r;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	using num = modnum<998244353>;
	int op, N; num p1_numer, p1_denom; cin >> op >> N >> p1_numer >> p1_denom;
	N--;
	// Reindex so that N is the number of slots we're allowed to use
	num p1 = p1_numer / p1_denom;
	num p2 = 1 - p1;

	num weight_2 = p2 + p1 * p1;
	assert(weight_2 != 0);
	// 1 - x + x^2 = 0 has no solutions

	// EV of the total is just sum of probabilties of valid states <= N, times (p1 + 2 * p2)

	std::vector<num> ans(N+1);
	ans[0] = 1;
	// ans_2 is the answer excluding things that start exactly with 1
	std::vector<num> ans_2(N+1);
	ans_2[0] = 1;

	// We have several cases:

	// These are the number of ways to have a nonempty stack starting >= 2 and ending >= 2
	// capped means starting <= 2^n, uncapped means starting at exactly 2^{n+1}
	std::vector<num> stack_capped(N+1); // of each length
	std::vector<num> stack_uncapped(N+1); // of each length

	// These are the number of ways to have a nonempty stack starting >= 2 and ending == 1
	std::vector<num> stack_to_1_capped(std::max(3, N+1)); // of each length
	std::vector<num> stack_to_1_uncapped(std::max(3, N+1)); // of each length

	std::vector<num> pow_weight_2(N+1);
	pow_weight_2[0] = weight_2;
	for (int n = 1; n <= N; n++) {
		pow_weight_2[n] = pow_weight_2[n-1] * pow_weight_2[n-1];

		// if you have n slots, then 2^{n} is always achievable with weight weight_2^{2^{n-1}},
		// 2^{n+1} has slightly smaller weight weight_2^{2^{n}} * p2 / weight_2
		// Update stack_*

		// Uncapped is now must start with 2^{n+1}
		num weight_uncapped = pow_weight_2[n] * p2 / weight_2;
		for (int l = n; l-1 >= 2; l--) {
			// value
			stack_to_1_uncapped[l] = weight_uncapped * (stack_to_1_uncapped[l-1] + stack_to_1_capped[l-1]);
		}
		stack_to_1_uncapped[2] = weight_uncapped * p1;
		for (int l = n; l-1 >= 1; l--) {
			stack_uncapped[l] = weight_uncapped * (stack_uncapped[l-1] + stack_capped[l-1]);
		}
		stack_uncapped[1] = weight_uncapped;

		num weight_capped = pow_weight_2[n-1];
		for (int l = n; l-1 >= 2; l--) {
			stack_to_1_capped[l] += weight_capped * stack_to_1_capped[l-1];
		}
		stack_to_1_capped[2] += weight_capped * p1;
		for (int l = n; l-1 >= 1; l--) {
			stack_capped[l] += weight_capped * stack_capped[l-1];
		}
		stack_capped[1] += weight_capped;

		// empty
		ans_2[n] = 1;

		// starts at >= 2 and never gets to 1
		for (int l = 1; l <= n; l++) {
			ans_2[n] += stack_capped[l];
			ans_2[n] += stack_uncapped[l];
		}

		// starts at >= 2 and gets to 1
		for (int l = 2; l <= n; l++) {
			ans_2[n] += stack_to_1_capped[l] * ans_2[n-l];
			ans_2[n] += stack_to_1_uncapped[l] * ans_2[n-l];
		}

		// starts at 1 is just ans_2[n-1]
		ans[n] = ans_2[n] + p1 * ans_2[n-1];
	}

	for (int i = 0; i <= N; i++) {
		ans[i] *= p1 + 2 * p2;
	}
	if (op == 0) {
		cout << ans[N] << '\n';
	} else {
		for (int i = 0; i <= N; i++) {
			cout << ans[i] << " \n"[i+1==N];
		}
	}

	return 0;
}
