#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		int ans = 0;
		while (N--) {
			int a; cin >> a; ans += (a >= 1000);
		}
		cout << ans << '\n';
	}

	return 0;
}
