#include <bits/stdc++.h>

template <typename T> void setmax(T& a, T b) { if (b > a) a = b; }

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	const int V = int(1e6) + 10;
	vector<int> dp(V+1, -1);
	vector<int> ans(V+1, -1);
	dp[0] = 0;
	for (int v = 0; v <= V; v++) {
		if (dp[v] == -1) continue;
		int cnd = dp[v]+1;
		for (int f = 2; f * (v+1) <= V; f++) {
			setmax(dp[f*(v+1)], cnd);
			if (f >= 3) {
				setmax(ans[f*(v+1)], cnd);
			}
		}
	}

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		int N; cin >> N;
		cout << "Case #" << case_num << ": " << ans[N] << '\n';
	}

	return 0;
}
