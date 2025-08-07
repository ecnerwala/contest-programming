#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, X; cin >> N >> X; X--;
		std::string S; cin >> S;
		int l = X-1;
		while (l >= 0 && S[l] == '.') l--;
		int r = X+1;
		while (r < N && S[r] == '.') r++;

		int ans = std::min(l - (-1) + 1, N - r + 1);
		if (l >= 0 && l < X-1) {
			ans = std::max(ans, std::min((X-1) - (-1) + 1, N - r + 1));
		}
		if (r > X+1) {
			ans = std::max(ans, std::min(l - (-1) + 1, N - (X+1) + 1));
		}
		cout << ans << '\n';
	}

	return 0;
}
