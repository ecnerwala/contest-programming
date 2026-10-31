#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		const int V = N * 2;
		std::vector<std::pair<int, int>> pref(V+1, {-1, -1});
		for (int i = 0; i < N; i++) {
			int a, b; cin >> a >> b; a--, b--;
			pref[a] = std::max(pref[a], {b, i});
		}
		std::vector<int> ans; ans.reserve(V+1);
		for (auto [b, i] : pref) {
			if (i != -1) ans.push_back(i);
		}
		cout << ans.size() << '\n';
		for (int i = 0; i < int(ans.size()); i++) {
			cout << ans[i]+1 << " \n"[i+1==int(ans.size())];
		}
	}

	return 0;
}
