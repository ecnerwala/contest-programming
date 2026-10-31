#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::array<std::string, 3> S;
		cin >> S[0] >> S[1] >> S[2];
		int ans = 0;
		for (int z = 0; z < 3; z++) {
			int a = z;
			int b = (z+1)%3;
			if (S[a].back() != S[b].back()) {
				for (int i = N-1; i >= 0; i--) {
					if (S[a][i] == S[a].back() && S[b][i] == S[b].back()) {
						ans++;
					} else {
						break;
					}
				}
			}
		}
		ans = std::min(ans, N);
		cout << ans << '\n';
	}

	return 0;
}
