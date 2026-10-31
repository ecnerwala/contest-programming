#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	int ans = N;
	int prv = 0;
	for (int i = 0; i < N; i++) {
		int cur; cin >> cur;
		ans += 2 * max(0, cur - prv);
		prv = cur;
	}
	cout << ans << '\n';

	return 0;
}
