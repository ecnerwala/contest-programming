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

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int R, C; cin >> R >> C;
	std::vector<std::vector<int>> A(R, std::vector<int>(C));
	for (int i = 0; i < R; i++) {
		for (int j = 0; j < C; j++) {
			cin >> A[i][j];
		}
	}

	using num = modnum<998244353>;

	using state_t = std::array<int, 7>;
	std::map<state_t, num> dp;
	dp[state_t{0,0,0,0,0,0,0}] = 1;
	for (int i = 0; i < R; i++) {
		for (int j = 0; j < C; j++) {
			std::map<state_t, num> ndp;
			for (const auto [st, w] : dp) {
				if (A[i][j] != 0) {
					if (i-1 >= 0 && j-1 >= 0 && A[i-1][j-1] && st[j] != 0) {
						continue;
					}
					state_t nst = st;
					nst[j] = A[i][j];
					if (i-1 >= 0 && !A[i-1][j]) {
						nst[j] -= st[j+1];
					}
					if (j-1 >= 0 && !A[i][j-1]) {
						nst[j] -= st[j-1];
					}
					if (nst[j] < 0) continue;
					ndp[nst] += w;
				} else {
					for (int v = 0; v < 4; v++) {
						if (i-1 >= 0 && j-1 >= 0 && A[i-1][j-1] && st[j] - v != 0) {
							continue;
						}
						state_t nst = st;
						nst[j] = v;
						if (j-1 >= 0 && A[i][j-1]) {
							nst[j-1] -= v;
							if (nst[j-1] < 0) continue;
						}
						if (i-1 >= 0) {
							if (A[i-1][j]) {
								nst[j+1] -= v;
								if (nst[j+1] < 0) continue;
							}
							if (j+1 < C && A[i-1][j+1]) {
								nst[j+2] -= v;
								if (nst[j+2] < 0) continue;
							}
						}
						ndp[nst] += w;
					}
				}
			}
			dp = std::move(ndp);
		}
		{
			std::map<state_t, num> ndp;
			for (const auto [st, w] : dp) {
				if (i-1 >= 0 && A[i-1][C-1] && st[C]) continue;
				state_t nst;
				for (int j = C; j > 0; j--) {
					nst[j] = st[j-1];
				}
				nst[0] = 0;
				ndp[nst] += w;
			}
			dp = std::move(ndp);
		}
	}

	num ans = 0;
	for (const auto [st, w] : dp) {
		bool good = true;
		for (int j = 0; j < C; j++) {
			if (A[R-1][j] && st[j+1]) {
				good = false;
				break;
			}
		}
		if (good) ans += w;
	}
	cout << ans << '\n';

	return 0;
}
