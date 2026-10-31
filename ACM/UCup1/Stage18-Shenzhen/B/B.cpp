#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M; cin >> N >> M;
		std::vector<std::pair<int, int>> locs(N*M);
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M; j++) {
				int v; cin >> v;
				locs[v] = {i+j, i-j};
			}
		}
		std::map<int, int> mp;
		int ans = 0;
		while (ans < N*M) {
			auto [s, d] = locs[ans];
			auto [it, did_insert] = mp.insert({s, d});
			if (!did_insert) {
				break;
			}
			if (std::next(it) != mp.end()) {
				auto nt = std::next(it);
				if (std::abs(nt->second - it->second) > nt->first - it->first) {
					break;
				}
			}
			if (it != mp.begin()) {
				auto pt = std::prev(it);
				if (std::abs(pt->second - it->second) > it->first - pt->first) {
					break;
				}
			}
			ans++;
		}
		cout << ans << '\n';
	}

	return 0;
}
