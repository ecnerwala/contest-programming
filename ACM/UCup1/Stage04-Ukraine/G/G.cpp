#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<uint32_t> adj(N);
	for (int i = 0; i < N; i++) {
		string s; cin >> s;
		for (int j = 0; j < N; j++) {
			adj[i] += uint32_t(s[j] - '0') << j;
		}
	}

	std::vector<int> dp((1<<(N-1)));
	dp[0] = 1;
	for (int msk = 0; msk < (1 << (N-1)); msk ++) {
		uint32_t nxt = 0;
		int v = dp[msk];
		for (int i = 0; i < N; i++) {
			if (v & (1 << i)) {
				nxt |= adj[i];
			}
		}
		nxt &= ~(msk<<1);
		for (int i = 1; i < N; i++) {
			if (nxt & (1 << i)) {
				dp[msk+(1<<(i-1))] |= (1 << i);
			}
		}
	}

	std::vector<std::string> ans(N, std::string(N, '0'));
	for (int i = 0; i < N; i++) {
		int res = 0;
		for (int msk = 0; msk < (1 << (N-1)); msk++) {
			if (dp[msk] & (1 << i)) res |= dp[(1<<(N-1))-1-msk];
		}
		for (int j = 0; j < N; j++) {
			if (i == j) continue;
			if (res & (1 << j)) ans[i][j] = '1';
		}
	}

	for (int i = 0; i < N; i++) {
		cout << ans[i] << '\n';
	}

	return 0;
}
