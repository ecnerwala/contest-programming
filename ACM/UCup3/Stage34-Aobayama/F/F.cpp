#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		std::string L, R, X; cin >> L >> R >> X;
		std::reverse(L.begin(), L.end());
		std::reverse(R.begin(), R.end());
		std::reverse(X.begin(), X.end());
		assert(int(L.size()) <= int(R.size()));
		L.resize(R.size(), 0);
		X.resize(R.size(), 0);
		std::array<int, 8> dp;
	}

	return 0;
}
