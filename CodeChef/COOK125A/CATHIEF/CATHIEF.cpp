#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int T; cin >> T;
	while (T--) {
		int x, y, k, n; cin >> x >> y >> k >> n;
		cout << ((x-y) % (2*k) ? "No" : "Yes") << '\n';
	}

	return 0;
}
