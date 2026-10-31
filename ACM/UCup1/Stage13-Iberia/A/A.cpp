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
	using bs = bitset<2020>;

	auto gen_coeffs_slow = [&](int L) -> std::vector<num> {
		// our base field
		const int q = 2;
		std::vector<num> pow_q(L+1);
		pow_q[0] = 1;
		for (int i = 1; i <= L; i++) pow_q[i] = pow_q[i-1] * q;

		std::vector<num> q_num(L+1);
		q_num[0] = 0;
		for (int i = 1; i <= L; i++) q_num[i] = q_num[i-1] + pow_q[i-1];

		std::vector<num> q_fact(L+1);
		q_fact[0] = 1;
		for (int i = 1; i <= L; i++) q_fact[i] = q_fact[i-1] * q_num[i];

		std::vector<num> q_ifact(L+1);
		q_ifact[L] = inv(q_fact[L]);
		for (int i = L; i >= 1; i--) q_ifact[i-1] = q_ifact[i] * q_num[i];

		// We want to compute the coefficients for the PIE:
		// we want sum_{subspace A includes S} f(A) = Indicator[S is everything]
		// Clearly, the coefficients only depend on the dimension of (everything / A).

		std::vector<num> coeffs(L+1);
		coeffs[0] = 1;
		for (int i = 1; i <= L; i++) {
			num v = 0;
			for (int j = 0; j < i; j++) {
				v += q_fact[i] * q_ifact[i-j] * q_ifact[j] * coeffs[j];
			}
			coeffs[i] = -v;
		}

		// We can see that the q-EGF of the coeffs is the q-exponential function,
		// whose inverse is the (1/q)-exponential e_{1/q}(-z), so the coefficients are
		// coeffs[n] = (-1)^n * [n]_q! / [n]_{1/q}! = (-1)^n q^(n choose 2)
		// Here, n is the difference in degree.

		return coeffs;
	};

	auto solve = [&](int L, bs X) -> num {
		//cerr << L << '\n';
		//for (int i = 0; i < L; i++) { cerr << X[i]; } cerr << '\n';

		if (L == 0) return 1;
		if (!X[0]) return 0;

		// our base field
		const int q = 2;
		std::vector<num> pow_q(L+1);
		pow_q[0] = 1;
		for (int i = 1; i <= L; i++) pow_q[i] = pow_q[i-1] * q;

		// Coeffs are based on difference in dimension
		std::vector<num> coeffs(L+1);
		coeffs[0] = 1;
		for (int i = 1; i <= L; i++) {
			coeffs[i] = -coeffs[i-1] * pow_q[i-1];
		}

		std::vector<num> pow_pow_2(L+1);
		pow_pow_2[0] = 2;
		// really, these are to the qth power, but q = 2, so normal multiplication suffices
		for (int i = 1; i <= L; i++) pow_pow_2[i] = pow_pow_2[i-1] * pow_pow_2[i-1];

		std::vector<num> dp_lower(L+1);
		std::vector<num> dp_matched(L+1);
		dp_matched[0] = 1;
		dp_lower[0] = 1;

		for (int i = L-1; i >= 0; i--) {
			for (int d = L-1-i; d >= 0; d--) {
				{
					num v = dp_matched[d];
					dp_matched[d] = 0;

					// we could include this in the basis
					dp_matched[d+1] += v * pow_q[L-1-i-d] * (X[i] ? pow_pow_2[d] : 1);

					// otherwise we could not include this in the basis
					if (i != 0) {
						// divide by 2 since it needs to match
						dp_matched[d] += v / num(2);
					}
				}
				{
					num v = dp_lower[d];
					dp_lower[d] = 0;

					// we could be exactly the first mismatch
					dp_matched[d] += v * (X[i] ? pow_pow_2[d] / num(2) : inv(num(2))) * (i == 0 ? 1 : inv(num(2)));

					// we could be in the basis
					dp_lower[d+1] += v * pow_q[L-1-i-d];

					// we could be not in the basis
					dp_lower[d] += v;
				}
			}
		}

		num ans = 0;
		for (int i = 0; i <= L; i++) {
			ans += dp_matched[i] * coeffs[L-i];
		}

		return ans;
	};

	int N, M; cin >> N >> M;

	auto read_bs = [&]() -> bs {
		string s; cin >> s;
		bs b;
		for (int j = 0; j < M; j++) {
			b[j] = (s[j] == '1');
		}
		return b;
	};

	std::vector<bs> basis(M);
	for (int z = 0; z < N; z++) {
		bs b = read_bs();
		int prv = -1;
		while (true) {
			int cur = (prv == -1) ? int(b._Find_first()) : int(b._Find_next(prv));
			if (cur >= M) {
				// B is not a basis!!
				cout << 0 << '\n';
				exit(0);
			}

			if (!basis[cur][cur]) {
				basis[cur] = b;
				break;
			} else {
				b ^= basis[cur];
				prv = cur;
			}
		}
	}

	/*
	cerr << "basis " << '\n';
	for (int i = 0; i < M; i++) {
		if (basis[i][i]) {
			for (int j = 0; j < M; j++) { cerr << basis[i][j]; } cerr << '\n';
		}
	}
	*/

	bs X_orig = read_bs();
	bs cnd;
	bs X_new; int X_new_idx = 0;
	for (int cur = 0; cur < M; cur++) {
		if (basis[cur][cur]) {
			X_new[X_new_idx++] = X_orig[cur];
			if (cnd[cur] != X_orig[cur]) cnd ^= basis[cur];
			assert(cnd[cur] == X_orig[cur]);
		} else {
			if (cnd[cur] != X_orig[cur]) {
				// this is a guaranteed mismatch
				if (X_orig[cur]) {
					// cnd is always valid, so set the rest of the bits to 1
					while (X_new_idx < N) {
						X_new[X_new_idx++] = true;
					}
				} else {
					// cnd is always big, so subtract one from X_new
					// since cnd[cur] == true, we know something in X_new is set to true
					X_new_idx--;
					while (!X_new[X_new_idx]) X_new_idx--;
					X_new[X_new_idx++] = false;
					while (X_new_idx < N) {
						X_new[X_new_idx++] = true;
					}
				}
				break;
			}
		}
	}
	assert(X_new_idx == N);

	cout << solve(N, X_new) << '\n';

	return 0;
}
