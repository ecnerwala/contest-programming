#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	int ans = 0;
	while (N--) {
		int a, b; cin >> a >> b;
		ans += (a > b);
	}
	cout << ans << '\n';

	return 0;
}
