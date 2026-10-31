#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		int X; cin >> X;
		std::vector<std::pair<int, int>> A(N);
		for (auto& [x, y] : A) { int a; cin >> a; x = a % X, y = a / X; }
		std::sort(A.begin(), A.end());
		int ans = N;
		auto it = A.begin();
		for (int x = 0; x < X && x < ans; x++) {
			while (it != A.end() && it->first < x) ++it;
			int i = 0;
			while (it != A.end() && it->first == x && it->second <= i) {
				i++;
				it++;
			}
			// i is excluded
			int64_t cnd = int64_t(i) * int64_t(X) + x;
			if (cnd < ans) ans = int(cnd);
		}
		cout << ans << '\n';
	}

	return 0;
}
