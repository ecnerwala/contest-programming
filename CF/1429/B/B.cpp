#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int N, M; cin >> N >> M;
	vector<string> G(N); for (auto& s : G) cin >> s;

	vector<int> c1(N);
	vector<int> c2(M);
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			if (G[i][j] == '*') c1[i]++, c2[j]++;
		}
	}

	int64_t ans = 0;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			if (G[i][j] == '*') {
				ans += int64_t(c1[i]-1) * (c2[j]-1);
			}
		}
	}
	cout << ans << '\n';

	return 0;
}
