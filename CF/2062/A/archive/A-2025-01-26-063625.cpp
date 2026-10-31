#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		std::string S; cin >> S;
		int ans = 0;
		for (char c : S) ans += (c - '0');
		cout << ans << '\n';
	}

	return 0;
}
