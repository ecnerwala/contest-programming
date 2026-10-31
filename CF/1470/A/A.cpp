#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M; cin >> N >> M;
		vector<int> W(M); while (N--) { int v; cin >> v; ++W[--v]; }
		vector<int> C(M); for (auto& c : C) { cin >> c; }
		int64_t ans = 0;
		while (M--) while (W[M]--) ans += C[min(M,++N)];
		cout << ans << '\n';
	}

	return 0;
}
