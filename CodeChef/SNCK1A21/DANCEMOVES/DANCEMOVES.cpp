#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int X, Y; cin >> X >> Y;
		Y -= X;
		if (Y > 0) cout << ((Y+1)/2) + (Y & 1) << '\n';
		else cout << -Y << '\n';
	}

	return 0;
}
