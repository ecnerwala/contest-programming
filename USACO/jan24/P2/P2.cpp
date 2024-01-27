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
	modnum& operator += (modnum& o) { v -= MOD - o.v; v = (v < 0) ? v + MOD : v; return *this; }
	friend modnum operator + (modnum a, modnum b) { return a += b; }
	modnum& operator -= (modnum& o) { v -= o.v; v = (v < 0) ? v + MOD : v; return *this; }
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

	std::vector<num> dp((N+1) * (N+1));
	dp[(N+1) * 0 + N] = num(1);
	for (int l = N; l > 1; l--) {
		for (int i = 0, j = l; j <= N; i++, j++) {
			// pick a split uniformly at random
			num p = dp[(N+1) * i + j] * invs[l-1];
			for (int k = i+1; k <= j-1; k++) {
				if (pref[j] - pref[k] >= pref[k] - pref[i]) {
					dp[(N+1) * k + j] += p;
				} else {
					dp[(N+1) * i + k] += p;
				}
			}
		}
	}
	for (int i = 0; i < N; i++) {
		cout << dp[(N+1) * i + (i+1)].v << '\n';
	}

	return 0;
}

// Consider the merges in reverse
