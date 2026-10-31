#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, X, Y; cin >> N >> X >> Y;
		std::string S; cin >> S;
		assert(int(S.size()) == N);
		int c4 = 0, c8 = 0;
		for (char c : S) {
			if (c == '4') c4++;
			else if (c == '8') c8++;
			else assert(false);
		}
		X = std::abs(X);
		Y = std::abs(Y);
		X = std::max(0, X - c8);
		Y = std::max(0, Y - c8);
		cout << (X + Y <= c4 ? "YES" : "NO") << '\n';
	}

	return 0;
}
