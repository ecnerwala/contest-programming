#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, Q; cin >> N >> Q;
	map<int64_t, int64_t> mp;
	for (int i = 0; i < N; i++) {
		int a; cin >> a; mp[a]++;
	}
	while (Q--) {
		int t; cin >> t;
		if (t == 1) {
			int x; cin >> x;
			int64_t v = mp[x];
			mp[x] = 0;
			mp[x/2] += v;
			mp[(x+1)/2] += v;
		} else if (t == 2) {
			int y; cin >> y;
			cout << mp[y] << '\n';
		}
	}

	return 0;
}
