#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

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
	using ll = long long;

	int v;

public:

	modnum() : v(0) {}
	modnum(ll v_) : v(int(v_ % MOD)) { if (v < 0) v += MOD; }
	explicit operator int() const { return v; }
	friend std::ostream& operator << (std::ostream& out, const modnum& n) { return out << int(n); }
	friend std::istream& operator >> (std::istream& in, modnum& n) { ll v_; in >> v_; n = modnum(v_); return in; }

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
		v = int(ll(v) * ll(o.v) % MOD);
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

class FalseAbove {

public:

	vector <int> solve(int N) {
		using num = modnum<int(1e9)+7>;
		int A = N / 2;
		num B = 0;
		num C = 0;

		std::vector<num> fact(N+1);
		fact[0] = 1;
		for (int i = 1; i <= N; i++) fact[i] = fact[i-1] * num(i);
		std::vector<num> ifact(N+1);
		ifact[N] = inv(fact[N]);
		for (int i = N; i >= 1; i--) ifact[i-1] = ifact[i] * num(i);

		std::vector<num> Ccoeff(N);
		Ccoeff[0] = 1;
		for (int i = 1; i < N; i++) Ccoeff[i] = Ccoeff[i-1] * num(N+1);

		if (N % 2 == 0) {
			B = fact[N/2];
			num mean_big = num(N/2 + 1 + N) / num(2);
			for (int i = 0; i < N; i ++) {
				C += Ccoeff[i] * (i % 2 ? num((i+1)/2) : mean_big);
			}
			C *= B;
		} else {
			B = num(N/2+1) * fact[N/2+1];
			for (int i = 0; i < N; i++) {
				if (i == 0) {
					C += Ccoeff[i] * num(N/2+1) * fact[N/2] * num(N/2) * num(N/2 + 2 + N) / num(2);
					C += Ccoeff[i] * num(N/2+1) * fact[N/2] * num(1 + N/2 + 1) / num(2);
				} else if (i % 2 == 1) {
					// must be cnd if missing is bigger than cnd
					int cnd = (i+1)/2;
					C += Ccoeff[i] * num(N/2 + 1 - cnd) * fact[N/2 + 1] * num(cnd);
					// otherwise, it's just random
					C += Ccoeff[i] * num(cnd) * fact[N/2] * num(N/2) * num(N/2 + 2 + N) / num(2);
					C += Ccoeff[i] * num(cnd) * fact[N/2] * num(1 + cnd) / num(2);
				} else if (i % 2 == 0) {
					int cnd = i/2 + 1;
					C += Ccoeff[i] * num(cnd - 1) * fact[N/2 + 1] * num(cnd);
					// otherwise, it's just random
					C += Ccoeff[i] * num(N/2 + 1 - cnd + 1) * fact[N/2] * num(N/2) * num(N/2 + 2 + N) / num(2);
					C += Ccoeff[i] * num(N/2 + 1 - cnd + 1) * fact[N/2] * num(cnd + N/2 + 1) / num(2);
				} else assert(false);
			}
			B -= num(N/2) * fact[N/2]; // ways for adjacent to match
			for (int i = 0; i < N; i++) {
				if (i == 0) {
					C -= Ccoeff[i] * num(N/2) * fact[N/2] * num(N/2 + 2 + N) / num(2);
				} else if (i % 2 == 1) {
					int cnd = (i+1) / 2;
					C -= Ccoeff[i] * num(N/2 - cnd + 1) * fact[N/2] * num(cnd);
					C -= Ccoeff[i] * num(cnd - 1) * fact[N/2] * num(N/2 + 2 + N) / num(2);
				} else if (i % 2 == 0) {
					int cnd = i/2 + 1;
					C -= Ccoeff[i] * num(cnd - 1) * fact[N/2] * num(cnd);
					C -= Ccoeff[i] * num(N/2 - cnd + 1) * fact[N/2] * num(N/2 + 2 + N) / num(2);
				} else assert(false);
			}
		}

		return {A, int(B), int(C)};
	}

	vector <int> solve_slow(int N) {
		using num = modnum<int(1e9)+7>;
		int A = 0;
		num B = 0;
		num C = 0;

		std::vector<int> P(N);
		std::iota(P.begin(), P.end(), 1);

		do {
			std::vector<bool> dp(N+1);
			dp[0] = true;
			for (int i = 0; i < N; i++) {
				std::vector<bool> ndp(N+1);

				// assign to false
				for (int v = 0; v < N; v++) ndp[v+1] = dp[v];

				// assign to true
				if (dp[P[i]]) {
					ndp[P[i]] = true;
				}
				dp = ndp;
			}

			int a = N;
			while (!dp[N-a]) a--;

			if (a > A) {
				A = a;
				B = 0;
				C = 0;
			}

			if (a == A) {
				B++;
				num c = 0;
				for (int i = 0; i < N; i++) {
					c += pow(num(N+1), i) * P[i];
				}
				C += c;
			}

		} while (std::next_permutation(P.begin(), P.end()));
		return {A, int(B), int(C)};
	}
};

void my_tests() {
	// FILL IN TESTS
	assert(FalseAbove().solve(10)[2] == 680141183);
	for (int N = 1; N <= 9; N++) {
		cerr << "testing " << N << '\n';
		auto fast = FalseAbove().solve(N);
		auto slow = FalseAbove().solve_slow(N);
		for (auto v : fast) { cerr << v << ' '; } cerr << '\n';
		for (auto v : slow) { cerr << v << ' '; } cerr << '\n';
		assert(fast == slow);
	}
}

}  // namespace ecnerwala

using ecnerwala::FalseAbove;

// vim:ft=cpp
