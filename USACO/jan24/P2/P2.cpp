#include <bits/stdc++.h>

int mod_inv_in_range(int a, int m) {
	return a == 1 ? 1 : m - int(int64_t(mod_inv_in_range(m % a, a)) * m / a);
}

template <int MOD_> struct modnum {
	static const int MOD = MOD_;
	int v;
	modnum() : v(0) {}
	explicit modnum(int64_t a) : v(int(a % MOD)) { if (v < 0) v += MOD; }
	explicit operator int() const { return v; }
	modnum& operator += (modnum o) { v -= MOD - o.v; v = (v < 0) ? v + MOD : v; return *this; }
	friend modnum operator + (modnum a, modnum b) { return a += b; }
	modnum& operator -= (modnum o) { v -= o.v; v = (v < 0) ? v + MOD : v; return *this; }
	friend modnum operator - (modnum a, modnum b) { return a -= b; }

	modnum& operator *= (modnum o) {
		v = int(int64_t(v) * int64_t(o.v) % MOD);
		return *this;
	}
	friend modnum operator * (modnum a, modnum b) { return a *= b; }

	friend modnum inv(modnum a) {
		modnum r;
		r.v = mod_inv_in_range(a.v, MOD);
		return r;
	}
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<int> S(N);
	for (auto& s : S) cin >> s;
	std::vector<int> pref(N+1);
	pref[0] = 0;
	for (int i = 0; i < N; i++) pref[i+1] = pref[i] + S[i];

	using num = modnum<int(1e9) + 7>;
	std::vector<num> invs(N+1);
	{
		num f = num(1);
		for (int i = 1; i <= N; i++) {
			invs[i] = f;
			f *= num(i);
		}
		f = inv(f);
		for (int i = N; i >= 1; i--) {
			invs[i] *= f;
			f *= num(i);
		}
	}

	std::vector<num> dp((N+2) * (N+2));
	auto coords = [&](int x, int y) -> int {
		assert(-1 <= x && x <= N);
		assert(0 <= y && y <= N+1);
		return (N+2) * (x+1) + y;
	};
	// inclusive
	auto rect_incr = [&](int xlo, int ylo, int xhi, int yhi, num v) {
		assert(xlo - 1 <= xhi);
		assert(ylo - 1 <= yhi);
		dp[coords(xlo-1, ylo-1)] += v;
		dp[coords(xhi, yhi)] += v;
		dp[coords(xlo-1, yhi)] -= v;
		dp[coords(xhi, ylo-1)] -= v;
	};
	rect_incr(0, N, 0, N, num(1));
	for (int l = N; l >= 1; l--) {
		int k_mid = 0;
		for (int i = 0, j = l; j <= N; i++, j++) {
			// pick a split uniformly at random
			num p = dp[coords(i, j)] += dp[coords(i+1, j)] + dp[coords(i, j+1)] - dp[coords(i+1, j+1)];
			if (l == 1) {
				cout << p.v << '\n';
				continue;
			}
			p *= invs[l-1];
			while (k_mid <= i || (k_mid <= j-1 && pref[j] - pref[k_mid] >= pref[k_mid] - pref[i])) k_mid++;
			cerr << i << ' ' << j << ' ' << k_mid << '\n';
			assert(i+1 <= k_mid && k_mid <= j);
			rect_incr(i+1, j, k_mid-1, j, p);
			rect_incr(i, k_mid, i, j-1, p);
		}
	}
	return 0;
}

// Consider the merges in reverse
