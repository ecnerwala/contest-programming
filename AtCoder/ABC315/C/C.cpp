#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::map<int, int> mp;
	int ans = 0;
	for (int i = 0; i < N; i++) {
		int f, s; cin >> f >> s;
		if (mp.count(f)) {
			ans = std::max(ans, mp[f] + s - std::min(mp[f], s) / 2);
			mp[f] = std::max(mp[f], s);
		} else {
			mp[f] = s;
		}
	}
	int best = -1;
	for (auto [_, s] : mp) {
		if (best != -1) {
			ans = std::max(ans, best + s);
		}
		best = std::max(best, s);
	}
	cout << ans << '\n';

	return 0;
}
