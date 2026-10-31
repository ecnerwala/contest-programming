#include<bits/stdc++.h>

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

	using num = modnum<998244353>;
	auto ways = [&](std::string S) -> num {
		std::vector<num> dp(S.size() + 1);
		dp[0] = 1;
		for (int i = 0; i < int(S.size()); i++) {
			assert('0' <= S[i] && S[i] <= '9');
			if (S[i] != '0') dp[i+1] += dp[i];
			if (i+1 < int(S.size())) {
				if (S[i] == '1') {
					if ('0' <= S[i+1] && S[i+1] <= '9') {
						dp[i+2] += dp[i];
					}
				} else if (S[i] == '2') {
					if ('0' <= S[i+1] && S[i+1] <= '6') {
						dp[i+2] += dp[i];
					}
				}
			}
		}
		return dp[int(S.size())];
	};

	auto solve = [&](std::string S, int K) -> std::string {
		int cap = int(S.size());
		for (int i = int(S.size())-1; i >= 0; i--) {
			assert(i < cap);
			if (S[i] == '0') {
				assert(i > 0);
				if (S[i-1] == '?') {
					S[i-1] = char('1' + (2 - 1 - K % 2));
					K /= 2;
				} else {
					assert(S[i-1] == '1' || S[i-1] == '2');
				}
				cap = i-1;
				i--;
				continue;
			}
			if (S[i] == '?') {
				if (i+1 == cap) {
					if (i > 0 && S[i-1] == '?') {
						// these two form a pair, anything from 11-19 or 21-26
						int v = 11 + (15 - 1 - K % 15);
						K /= 15;
						if (v >= 20) v++;
						S[i] = char('0' + v % 10);
						S[i-1] = char('0' + v / 10);
						i--;
						continue;
					} else if (i > 0 && S[i-1] == '2') {
						// must be 1-6
						S[i] = char('1' + (6 - 1 - K % 6));
						K /= 6;
					} else {
						// can be 1-9, since that guy is either 1, 0, or 3-9, any of which have the same merging right behavior
						S[i] = char('1' + (9 - 1 - K % 9));
						K /= 9;
					}
				} else if ('1' <= S[i+1] && S[i+1] <= '6') {
					S[i] = char('1' + (2 - 1 - K % 2));
					K /= 2;
				} else if ('7' <= S[i+1] && S[i+1] <= '9') {
					// 1 only
					S[i] = '1';
				} else if (S[i+1] == '0') {
					assert(false);
				} else if (S[i+1] == '?') {
					assert(false);
				} else assert(false);
			}
		}
		assert(K == 0);

		for (char c : S) assert('0' <= c && c <= '9');

		return S;
	};

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		std::string S; int K; cin >> S >> K;
		assert(K >= 1);
		K--;
		std::string cnd = S;
		for (char& c : cnd) if (c == '?') c = '1';
		num v = ways(cnd);
		std::string res = solve(S, K);
		num u = ways(res);
		assert(v == u);
		cout << "Case #" << case_num << ": " << res << ' ' << u << '\n';
	}

	return 0;
}
