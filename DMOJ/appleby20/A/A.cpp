#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		array<int64_t, 3> a;
		cin >> a[0] >> a[1] >> a[2];
		sort(a.begin(), a.end());
		int64_t d = a[2] * a[2] - a[1] * a[1] - a[0] * a[0];
		if (d > 0) cout << 'O' << '\n';
		else if (d == 0) cout << 'R' << '\n';
		else cout << 'A' << '\n';
	}

	return 0;
}
