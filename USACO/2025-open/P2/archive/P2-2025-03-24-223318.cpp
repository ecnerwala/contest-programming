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
	if (false) {
		int R = ops.back().second+2;
		std::vector<modnum> dp(R);
		dp[ops.front().second] = 1;
		for (int i = 1, it = 1; i < N; i++) {
			if (ops[it].first == i) {
				int val = ops[it].second;
				++it;
				std::vector<modnum> ndp(R);
				for (int v = 0; v < R && v <= val + 1; v++) ndp[std::max(v, val)] += dp[v];
				dp = ndp;
			} else {
				std::vector<modnum> ndp = dp;
				for (int a = 1; a < R; a++) ndp[a] += ndp[a-1];
				for (int a = 1; a < R; a++) ndp[a] += dp[a];
				dp = ndp;
			}
		}
		cout << std::accumulate(dp.begin(), dp.end(), modnum(0)) << '\n';
		exit(0);
	}

	// range is number of distinct allowed values
	auto ways = [&](int num_mid, int range) -> modnum {
		if (range <= 1) return 0;
		assert(range > 0);
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
		auto p2 = [](int i) -> modnum {
			modnum a = 1;
			for (int z = 0; z < i; z++) a += a;
			return a;
		};
		modnum ans = choose(num_mid + range-1, range-1);
		for (int cnt = 1; cnt * 2 <= num_mid && cnt <= range - 1; cnt++) {
			for (int i = 0; i <= num_mid - 2 * cnt; i++) {
				ans +=
					choose(range-1, cnt) * choose(i+cnt-1, cnt-1) * p2(i)
					* choose(num_mid - 2 * cnt - i + range - 1, range - 1);
				;
			}
		}
		return ans;
	};
	auto ways_slow = [&](int n, int r) -> modnum {
		std::vector<modnum> dp(r);
		dp[0] = 1;
		for (int z = 0; z < n; z++) {
			std::vector<modnum> ndp = dp;
			for (int i = 1; i < r; i++) ndp[i] += ndp[i-1];
			for (int i = 1; i < r; i++) ndp[i] += dp[i];
			dp = ndp;
		}
		return std::accumulate(dp.begin(), dp.end(), modnum(0));
	};

	for (int n = 0; n <= 10; n++) {
		for (int r = 1; r <= 10; r++) {
			assert(ways_slow(n, r).v == ways(n, r).v);
		}
	}

	// dp[0] means lower only, dp[1] means higher has occurred
	std::array<modnum, 2> dp{1, 0};
	for (int i = 0; i+1 < Q; i++) {
		// c .. v
		int num_mid = ops[i+1].first - ops[i].first - 1;
		int range_biggest = (ops[i+1].second + 1) - (ops[i].second - 1) + 1;
		assert(ops[i+1].second >= ops[i].second);
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
