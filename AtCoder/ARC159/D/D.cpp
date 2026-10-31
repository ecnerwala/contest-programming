#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::map<int, int> mp;
	for (int i = 0; i < N; i++) {
		int l, r; cin >> l >> r;
		auto it = mp.upper_bound(l);
		if (it != mp.begin() && std::prev(it)->second >= l) {
			--it;
			l = it->first;
		}
		int to_erase = r-l+1;
		while (to_erase > 0 && it != mp.end()) {
			int cur_len = it->second - it->first + 1;
			if (cur_len > to_erase) {
				int x = it->first + to_erase;
				int y = it->second;
				assert(x <= y);
				it = mp.erase(it);
				mp.insert({x, y});
				to_erase = 0;
				break;
			} else {
				it = mp.erase(it);
				to_erase -= cur_len;
			}
		}
		mp.insert({l, r});
	}

	int ans = 0;
	for (auto [l, r] : mp) ans += r-l+1;
	cout << ans << '\n';

	return 0;
}
