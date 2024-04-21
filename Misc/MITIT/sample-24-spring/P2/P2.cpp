#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M; cin >> N >> M;
		std::vector<int> D(N);
		for (auto& d : D) cin >> d;
		std::sort(D.begin(), D.end());
		int ans = M;
		for (auto& d : D) {
			if (d > M) {
				break;
			}
			ans++;
			M -= d;
		}
		cout << ans << '\n';
	}

	return 0;
}
