#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		map<int, int> mp;
		for (int i = 0; i < N; i++) {
			int a; cin >> a; mp[a]++;
		}
		int cnt = 0;
		for (auto& it : mp) cnt = max(cnt, it.second);
		int rep = 0;
		for (auto& it : mp) rep += (it.second == cnt);
		cout << (N-rep) / (cnt-1) - 1 << '\n';
	}

	return 0;
}
