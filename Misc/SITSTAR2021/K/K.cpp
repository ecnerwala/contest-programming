#include <bits/stdc++.h>

template <int MOD_> struct modnum {
	static constexpr int MOD = MOD_;
	static_assert(MOD_ > 0, "MOD must be positive");

private:
	using ll = long long;

	int v;

	static int minv(int a, int m) {
		a %= m;
		assert(a);
		return a == 1 ? 1 : int(m - ll(minv(m, a)) * ll(m) / a);
	}

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
		res.v = minv(v, MOD);
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

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		vector<string> G(3); for (auto& r : G) cin >> r;
		int tot_msk = 0;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				tot_msk |= (G[i][j] == 'x') << (3 * i + j);
			}
		}

		using num = modnum<int(1e9)+7>;

		vector<int> can_together(512, 0);
		for (int m = tot_msk; m > 0; m = (m-1) & tot_msk) {
			can_together[m] = true;
		}

		auto remove_two = [&](int a, int b) {
			int cur_msk = (1 << a) | (1 << b);
			if ((tot_msk & cur_msk) != cur_msk) return;
			for (int m = tot_msk; m > 0; m = (m-1) & tot_msk) {
				if ((m & cur_msk) == cur_msk) can_together[m] = false;
			}
		};
		for (int i = 0; i < 3; i++) {
			if (G[i][1] == 'x') {
				remove_two(3*i+0, 3*i+1);
				remove_two(3*i+1, 3*i+2);
			} else {
				remove_two(3*i+0, 3*i+2);
			}
		}
		for (int j = 0; j < 3; j++) {
			if (G[1][j] == 'x') {
				remove_two(3*0+j, 3*1+j);
				remove_two(3*1+j, 3*2+j);
			} else {
				remove_two(3*0+j, 3*2+j);
			}
		}

		vector<array<num, 10>> dp(512);
		dp[0][0] = 1;
		for (int m = 1; m <= tot_msk; m++) {
			if ((tot_msk & m) != m) continue;
			for (int v = m; v > (v ^ m); v = (v-1) & m) {
				if (!can_together[v]) continue;
				for (int c = 0; c < 9; c++) {
					dp[m][c+1] += dp[v ^ m][c];
				}
			}
		}

		num ans = 0;
		for (int cnt = 0; cnt <= 9; cnt++) {
			num v = dp[tot_msk][cnt];
			for (int i = 0; i < cnt; i++) v *= num(N - i);
			ans += v;
		}
		cout << ans << '\n';
	}

	return 0;
}
