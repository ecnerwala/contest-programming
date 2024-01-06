#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int a, b; cin >> a >> b;
		cout << (((a + b) & 1) ? "Alice" : "Bob") << '\n';
	}

	return 0;
}
