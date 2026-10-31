#include <bits/stdc++.h>

template <int MOD_> struct modnum {
	static const int MOD = MOD_;
private:
	int v;
public:
	modnum() : v(0) {}
	modnum(int64_t v_) : v(int(v_ % MOD)) { if (v < 0) v += MOD; }
	explicit operator int() const { return v; }
	friend std::ostream& operator << (std::ostream& o, modnum m) { return o << int(m); }
	friend std::istream& operator >> (std::istream& i, modnum& m) { int64_t n; i >> n; m = modnum(n); return i; }

	friend bool operator == (modnum a, modnum b) { return a.v == b.v; }
	friend bool operator != (modnum a, modnum b) { return a.v != b.v; }

	modnum operator + () const { return *this; }
	modnum operator - () const { modnum r; r.v = v ? MOD - v : 0; return r; }
	friend modnum neg(modnum a) {
		return -a;
	}
private:
	static int minv(int a, int m) {
		return a == 1 ? 1 : int(m - int64_t(minv(m % a, a)) * m / a);
	}
public:
	friend modnum inv(modnum a) {
		assert(a.v);
		modnum r;
		r.v = minv(a.v, MOD);
		return r;
	}

	modnum& operator ++ () {
		v++;
		if (v == MOD) v = 0;
		return *this;
	}
	modnum& operator -- () {
		if (!v) v = MOD;
		v--;
		return *this;
	}
	modnum operator ++ (int) { modnum r = *this; operator ++ (); return r; }
	modnum operator -- (int) { modnum r = *this; operator -- (); return r; }

	modnum& operator += (modnum o) {
		v -= MOD - o.v;
		v = v < 0 ? v + MOD : v;
		return *this;
	}
	modnum& operator -= (modnum o) {
		v -= o.v;
		v = v < 0 ? v + MOD : v;
		return *this;
	}
	modnum& operator *= (modnum o) {
		v = int(int64_t(v) * int64_t(o.v) % MOD);
		return *this;
	}
	modnum& operator /= (modnum o) {
		return *this *= o.inv();
	}

	friend modnum operator + (modnum a, modnum b) { return a += b; }
	friend modnum operator - (modnum a, modnum b) { return a -= b; }
	friend modnum operator * (modnum a, modnum b) { return a *= b; }
	friend modnum operator / (modnum a, modnum b) { return a /= b; }
};

template <typename T> T pow(T a, int64_t b) {
	T r = 1;
	for (; b; b >>= 1, a *= a) if (b & 1) r *= a;
	return r;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	using num = modnum<int(1e9)+7>;

	int N; cin >> N;

	// we'll just use closed intervals, and N * l + r
	array<vector<num>, 4> dp;
	array<vector<num>, 4> ndp;
	// 0 is expanding, 1 is strictly contracting
	for (int z = 0; z < 4; z++) {
		dp[z].resize(N*N);
		ndp[z].resize(N*N);
	}

	num ans = 0;

	for (int row_num = 0; row_num < N; row_num++) {
		string cur_row; cin >> cur_row; assert(int(cur_row.size()) == N);
		for (int l = 0; l < N; l++) {
			bool is_good = true;
			for (int r = l; r < N; r++) {
				if (cur_row[r] != 'G') is_good = false;
				if (!is_good) {
					for (int z = 0; z < 4; z++) {
						dp[z][l*N+r] = 0;
					}
					continue;
				}

				dp[0][l*N+r] ++;
				for (int z = 0; z < 4; z++) {
					ans += dp[z][l*N+r];
				}
			}
		}

		// Now, we can relax the z, allow them to contract
		for (int l = 0; l < N; l++) {
			for (int r = l+1; r < N; r++) {
				dp[1][(l+1)*N+r] += dp[0][l*N+r];
				dp[3][(l+1)*N+r] += dp[2][l*N+r];
			}
		}
		for (int l = 0; l < N; l++) {
			for (int r = l+1; r < N; r++) {
				dp[2][l*N+(r-1)] += dp[0][l*N+r];
				dp[3][l*N+(r-1)] += dp[1][l*N+r];
			}
		}

		// Now, we can let them optionally grow/contract further
		// Contraction first
		for (int l = 0; l < N; l++) {
			for (int r = l+1; r < N; r++) {
				dp[1][(l+1)*N+r] += dp[1][l*N+r];
				dp[3][(l+1)*N+r] += dp[3][l*N+r];
			}
		}
		for (int l = 0; l < N; l++) {
			for (int r = N-1; r > l; r--) {
				dp[2][l*N+(r-1)] += dp[2][l*N+r];
				dp[3][l*N+(r-1)] += dp[3][l*N+r];
			}
		}

		for (int l = 0; l < N; l++) {
			for (int r = l; r+1 < N; r++) {
				dp[0][l*N+(r+1)] += dp[0][l*N+r];
				dp[1][l*N+(r+1)] += dp[1][l*N+r];
			}
		}
		for (int l = N-1; l > 0; l--) {
			for (int r = l; r < N; r++) {
				dp[0][(l-1)*N+r] += dp[0][l*N+r];
				dp[2][(l-1)*N+r] += dp[2][l*N+r];
			}
		}
	}

	cout << ans << '\n';

	return 0;
}
