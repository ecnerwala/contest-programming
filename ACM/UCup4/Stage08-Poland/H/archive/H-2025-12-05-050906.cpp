#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	int ans = 600;
	while (N--) {
		int t; cin >> t; ans -= t;
	}
	cout << ans << '\n';

	return 0;
}
