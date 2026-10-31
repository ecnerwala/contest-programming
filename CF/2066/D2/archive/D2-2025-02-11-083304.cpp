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

	int T; cin >> T;
	while (T--) {
		int N, C, M; cin >> N >> C >> M;
		std::vector<int> A(M); for (auto& a : A) { cin >> a; a--; }

		// top floor must throw C airplanes
		// each floor downwards throws some number
		// if you throw K airplanes, they must be among the first C you see, so (C choose K) ways
		using num = modnum<int(1e9)+7>;
		std::vector<num> fact(C+1);
		fact[0] = 1;
		for (int i = 1; i <= C; i++) fact[i] = fact[i-1] * num(i);
		std::vector<num> ifact(C+1);
		ifact[C] = inv(fact[C]);
		for (int i = C; i >= 1; i--) ifact[i-1] = ifact[i] * num(i);

		std::vector<int> lastA(N, -1);
		std::vector<int> cntA(N);
		for (int j = 0; j < M; j++) {
			if (A[j] != -1) {
				lastA[A[j]] = j;
				cntA[A[j]]++;
			}
		}

		auto choose = [&](int n, int r) -> num {
			if (0 <= r && r <= n) return fact[n] * ifact[r] * ifact[n-r];
			else return 0;
		};

		std::vector<int> num_open(M+1);
		for (int j = 0; j < M; j++) {
			num_open[j+1] = num_open[j] + (A[j] == -1);
		}

		// you have the first C people, and some of those are the first person's
		std::vector<num> dp(M-C+1);
		dp[0] = 1;
		std::vector<num> ndp(M-C+1);
		for (int i = 0; i < N-1; i++) {
			std::fill(ndp.begin(), ndp.end(), num(0));
			for (int st = std::max(lastA[i] - (C-1), 0); st <= M-C; st++) {
				num v = dp[st];
				if (v == 0) continue;
				for (int c = cntA[i]; c <= C && st + c <= M-C; c++) {
					ndp[st + c] += v * choose(num_open[st + C] - st + cntA[i], c - cntA[i]);
				}
			}
			std::swap(dp, ndp);
		}
		cout << dp[M-C] << '\n';
	}

	return 0;
}
