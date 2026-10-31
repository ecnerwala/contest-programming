#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		const int V = N * 2;
		std::vector<int> pref(V+1);
		for (int i = 0; i < N; i++) {
			int a, b; cin >> a >> b; a--, b--;
			pref[a]++;
			pref[b]--;
		}
		int cur = 0;
		int ans = 0;
		for (int i = 0; i <= V; i++) {
			cur += pref[i];
			if (cur) ans++;
		}
		cout << ans << '\n';
	}

	return 0;
}
