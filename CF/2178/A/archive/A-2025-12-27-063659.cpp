#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		std::string S; cin >> S;
		int cnt = 0;
		for (char c : S) cnt += c == 'Y';
		cout << (cnt <= 1 ? "YES" : "NO") << '\n';
	}

	return 0;
}
