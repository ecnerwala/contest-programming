#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> L(N);
		for (auto& a : L) cin >> a;
		std::sort(L.begin(), L.end());
		int64_t ans = -1;
		for (int i = 0; i+2 < N; i++) {
			int a = L[i], b = L[i+1], c = L[i+2];
			if (a+b >= c) {
				int s = (a+b+c) / 2;
				int64_t cnd = int64_t(s) * int64_t(s-a) * int64_t(s-b) * int64_t(s-c);
				ans = std::max(ans, cnd);
			}
		}
		cout << ans << '\n';
	}

	return 0;
}
