#include <bits/stdc++.h>

int powmod(int a, int b, int m) {
	int r = 1;
	while (b) {
		if (b & 1) r = int(int64_t(r) * int64_t(a) % m);
		a = int(int64_t(a) * int64_t(a) % m);
		b >>= 1;
	}
	return r;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, MOD; cin >> N >> MOD;
		vector<int> par(N);
		par[0] = -1;
		for (int i = 1; i < N; i++) cin >> par[i], par[i]--;
		vector<int> V(N);
		for (int i = 0; i < N; i++) cin >> V[i];

		for (int i = N-1; i > 0; i--) {
			auto& a = V[par[i]] += V[i];
			a = (a >= MOD ? a - MOD : a);
		}

		int S = __builtin_ctz(MOD-1);
		int Q = (MOD-1) >> S;
		int qnr = 2;
		while (powmod(qnr, (MOD-1)/2, MOD) != MOD-1) qnr++;

		int num_ways = 1;
		for (int i = 0; i < N; i++) {
			if (V[i] == 0) {
				// already done
				continue;
			}
			if (powmod(V[i], (MOD-1)/2, MOD) != 1) {
				// QNR :(
				num_ways = 0;
				break;
			}

			num_ways <<= 1;
			num_ways = (num_ways >= 998244353 ? num_ways - 998244353 : num_ways);

			// otherwise, take the modular square root, by tonelli shanks
			int M = S, c = powmod(qnr, Q, MOD), t = powmod(V[i], Q, MOD), R = powmod(V[i], (Q+1)/2, MOD);
			while (t != 1) {
				int a = 0;
				for (int v = t; v != 1; ) a++, v = int(int64_t(v) * int64_t(v) % MOD);
				assert(a <= M-1);
				int b = c;
				for (int z = 0; z < M-a-1; z++) b = int(int64_t(b) * int64_t(b) % MOD);
				M = a;
				c = int(int64_t(b) * int64_t(b) % MOD);
				t = int(int64_t(t) * int64_t(c) % MOD);
				R = int(int64_t(R) * int64_t(b) % MOD);
			}
			assert((int(int64_t(R) * int64_t(R) % MOD) == V[i]));
			V[i] = R;
		}

		if (num_ways != 0) {
			for (int i = 1; i < N; i++) {
				auto& a = V[par[i]] -= V[i];
				a = (a < 0 ? a + MOD : a);
			}

			cout << num_ways << '\n';
			for (int i = 0; i < N; i++) {
				cout << V[i] << " \n"[i+1==N];
			}
		} else {
			cout << 0 << '\n';
			cout << -1 << '\n';
		}
	}

	return 0;
}
