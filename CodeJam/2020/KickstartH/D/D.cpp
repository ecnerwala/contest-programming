#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		int N, Q; cin >> N >> Q;
		const int INF = 1e9;
		vector<vector<int>> dist(26, vector<int>(26, INF));
		for (int i = 0; i < 26; i++) dist[i][i] = 0;

		vector<int> msks(N);
		for (int z = 0; z < N; z++) {
			string S; cin >> S;
			int msk = 0;
			for (char c : S) {
				msk |= (1 << (c - 'A'));
			}
			msks[z] = msk;
			for (int i = 0; i < 26; i++) {
				if (!(msk & (1 << i))) continue;
				for (int j = i+1; j < 26; j++) {
					if (!(msk & (1 << j))) continue;
					dist[i][j] = dist[j][i] = 1;
				}
			}
		}

		for (int k = 0; k < 26; k++) {
			for (int i = 0; i < 26; i++) {
				for (int j = 0; j < 26; j++) {
					dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
				}
			}
		}

		cout << "Case #" << case_num << ":";
		for (int q = 0; q < Q; q++) {
			int u, v; cin >> u >> v; u--, v--;
			int ans = INF;
			for (int m1 = msks[u]; m1; m1 &= m1-1) {
				int i = __builtin_ctz(m1);
				for (int m2 = msks[v]; m2; m2 &= m2-1) {
					int j = __builtin_ctz(m2);
					ans = min(ans, 2 + dist[i][j]);
				}
			}
			if (ans == INF) ans = -1;
			cout << ' ' << ans;
		}
		cout << '\n';

	}

	return 0;
}
