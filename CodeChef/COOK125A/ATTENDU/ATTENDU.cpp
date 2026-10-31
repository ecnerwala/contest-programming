#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		string S; cin >> S;
		N = 120;
		for (char c : S) if (c == '0') N -= 4;
		cout << (N >= 0 ? "YES" : "NO") << '\n';
	}

	return 0;
}
