#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, X; cin >> N >> X;
		std::vector<int> cnt(N);
		for (int i = 0; i < N; i++) {
			int v; cin >> v;
			if (v < N) cnt[v]++;
		}
		int ans = 0;
		for (; ans < N && cnt[ans] > 0; ans++) {
			if (ans + X < N) cnt[ans + X] += cnt[ans] - 1;
		}
		cout << ans << '\n';
	}

	return 0;
}
