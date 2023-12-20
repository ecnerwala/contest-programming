#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::array<int, 26> cnt;
		int ans = 0;
		while (N--) {
			char c; cin >> c; c -= 'A';
			ans += cnt[c]++ == c;
		}
		cout << ans << '\n';
	}

	return 0;
}
