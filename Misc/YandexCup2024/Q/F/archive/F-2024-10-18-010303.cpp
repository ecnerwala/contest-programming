#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	cout << 11 << '\n';
	for (int i = 0; i < 11; i++) {
		cout << (1 << i);
		for (int v = (1 << i) + 1; v <= 2024; v++) {
			if (v & (1 << i)) {
				cout << ' ' << v;
			}
		}
		cout << '\n';
	}
	cout << flush;
	int T; cin >> T;
	while (T--) {
		int ans = 0;
		for (int i = 0; i < 11; i++) {
			int a; cin >> a;
			assert(a == 0 || a == 1);
			ans |= (a << i);
		}
		cout << ans << '\n';
	}

	return 0;
}
