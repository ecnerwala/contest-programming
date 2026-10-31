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

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> P(N);
		for (auto& p : P) cin >> p;

		std::vector<int> cyc_idx(N);
		std::vector<int> cyc_loc; cyc_loc.reserve(N);
		for (int i = 0; i < N; i ++) {
			if ((i & 1) == 0) {
				cyc_idx[i] = int(cyc_loc.size());
				cyc_loc.push_back(i);
			}
		}
		int N_even = int(cyc_loc.size());
		for (int i = N-1; i >= 0; i--) {
			if ((i & 1) == 1) {
				cyc_idx[i] = int(cyc_loc.size());
				cyc_loc.push_back(i);
			}
		}

		auto flip = [&]() {
			for (auto& p : P) {
				if (p != -1) p = N+1-p;
			}
		};

		using num = modnum<int(1e9)+7>;

		auto solve_even_1 = [&]() -> num {
			std::vector<num> dp(N);
			for (int i = 0; i < N_even; i++) {
				if (P[cyc_loc[i]] == -1 || P[cyc_loc[i]] == 1) {
					dp[i] = num(1);
				}
			}
			std::vector<num> ndp(N);
			for (int v = 2; v < N; v++) {
				std::fill(ndp.begin(), ndp.end(), num(0));
				for (int i = 0; i < N; i++) {
					if (dp[i] == 0) continue;
					// stored by right endpoint

					// try appending on the left (but only if v < N)
					do {
						int lo = i-(v-1);
						int hi = i;
						if (N+lo == N_even-1) break;
						if (lo + hi < 0) break;
						if (P[cyc_loc[(lo+N)%N]] == -1 || P[cyc_loc[(lo+N)%N]] == v) {
							ndp[i] += dp[i];
						}
					} while (false);

					// try appending on the right, make sure it doesn't overhang or wrap around
					do {
						int lo = i+1-(v-1);
						int hi = i+1;
						if (i+1 == N) break;
						if (lo + hi >= N) break;
						if (P[cyc_loc[i+1]] == -1 || P[cyc_loc[i+1]] == v) {
							ndp[i+1] += dp[i];
						}
					} while (false);
				}
				std::swap(dp, ndp);
			}
			num ans = 0;
			for (int i = N_even; i < N; i++) {
				if (P[cyc_loc[i]] == -1 || P[cyc_loc[i]] == N) {
					ans += dp[i-1];
				}
			}
			return ans;
		};

		num final_ans = solve_even_1();
		flip();
		final_ans += solve_even_1();
		cout << final_ans << '\n';
	}

	return 0;
}
