#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		string S; cin >> S;
		std::array<int, 26> cnt; cnt.fill(0);
		for (char c : S) cnt[c - 'A']++;

		std::array<std::array<int, 26>, 26> dist;
		for (int i = 0; i < 26; i++) {
			dist[i].fill(-1);
			dist[i][i] = 0;
		}

		int K; cin >> K;
		for (int z = 0; z < K; z++) {
			char a, b; cin >> a >> b;
			assert(a != b);
			dist[b-'A'][a-'A'] = 1;
		}

		for (int z = 0; z < 3; z++) {
			for (int k = 0; k < 26; k++) {
				for (int i = 0; i < 26; i++) {
					for (int j = 0; j < 26; j++) {
						if (dist[i][k] == -1 || dist[k][j] == -1) continue;
						if (dist[i][j] == -1 || dist[i][k] + dist[k][j] < dist[i][j]) dist[i][j] = dist[i][k] + dist[k][j];
					}
				}
			}
		}

		int ans = -1;
		for (int i = 0; i < 26; i++) {
			int cnd = 0;
			for (int j = 0; j < 26; j++) {
				if (cnt[j] == 0) continue;
				if (dist[i][j] == -1) goto bad;
				cnd += cnt[j] * dist[i][j];
			}
			if (ans == -1 || cnd < ans) ans = cnd;
			bad:;
		}

		cout << "Case #" << case_num << ": " << ans << '\n';
	}

	return 0;
}
