#include <bits/stdc++.h>

const int MOD = int(1e9)+7;
int modinv(int a, int m) {
	return a == 1 ? 1 : m - int(int64_t(modinv(m % a, a)) * m / a);
}
struct modnum {
	int v;
	modnum() : v(0) {}
	modnum(int64_t v_) : v(int(v_ % MOD)) { v += (v < 0) ? MOD : 0; }
	modnum& operator += (modnum o) {
		v -= MOD - o.v;
		v += (v < 0) ? MOD : 0;
		return *this;
	}
	modnum& operator -= (modnum o) {
		v -= o.v;
		v += (v < 0) ? MOD : 0;
		return *this;
	}
	modnum& operator *= (modnum o) {
		v = int(int64_t(v) * int64_t(o.v) % MOD);
		return *this;
	}
	modnum inv() const {
		modnum r;
		r.v = modinv(v, MOD);
		return r;
	}
	friend modnum inv(modnum a) { return a.inv(); }
	modnum& operator /= (modnum o) {
		return *this *= o.inv();
	}
	friend modnum operator + (modnum a, modnum b) { return a += b; }
	friend modnum operator - (modnum a, modnum b) { return a -= b; }
	friend modnum operator * (modnum a, modnum b) { return a *= b; }
	friend modnum operator / (modnum a, modnum b) { return a /= b; }
	friend std::ostream& operator << (std::ostream& o, modnum a) { return o << a.v; }
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, Q; cin >> N >> Q;
	std::vector<std::pair<int, int>> ops(Q);
	for (auto& [c, v] : ops) { cin >> c >> v; c--; }

	// range is number of distinct allowed values
	auto ways = [&](int num_mid, int range) -> modnum {
		if (range <= 1) return 0;
		assert(range >= 2);
		assert(num_mid >= 0);
		if (num_mid == 0) {
			return 1;
		}
		assert(num_mid > 0);
		// we're allowed to take O(num_mid)
		// we can take pairs of hi .. lo
		auto choose = [](int n, int r) -> modnum {
			assert(0 <= r && r <= n);
			r = std::min(r, n - r);
			modnum numer = 1, denom = 1;
			for (int i = 0; i < r; i++) {
				numer *= modnum(n-i);
				denom *= modnum(i+1);
			}
			return numer / denom;
		};
		std::vector<modnum> choose_range_2(num_mid+1);
		choose_range_2[0] = 1;
		for (int i = 1; i <= num_mid; i++) {
			choose_range_2[i] = choose_range_2[i-1];
			choose_range_2[i] *= modnum(range-1 + i);
			choose_range_2[i] /= modnum(i);
		}
		modnum ans = 0;
		// we start at 2, end at <= range after taking num_mid+1 steps
		// extra way to take 0 steps at any point
		for (int cnt = 0; cnt <= num_mid; cnt++) {
			ans += choose_range_2[cnt] * choose(num_mid, cnt);
		}
		return ans;
	};

	// dp[0] means lower only, dp[1] means higher has occurred
	std::array<modnum, 2> dp{1, 0};
	for (int i = 0; i+1 < Q; i++) {
		// c .. v
		int num_mid = ops[i+1].first - ops[i].first - 1;
		int range_biggest = (ops[i+1].second + 1) - (ops[i].second - 1) + 1;
		modnum wb = ways(num_mid, range_biggest);
		modnum wm = ways(num_mid, range_biggest - 1);
		modnum ws = ways(num_mid, range_biggest - 2);
		dp = {
			dp[0] * wm + dp[1] * ws,
			dp[0] * (wb - wm) + dp[1] * (wm - ws)
		};
	}
	cout << dp[0] + dp[1] << '\n';

	return 0;
}
