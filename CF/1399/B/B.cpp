#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		array<vector<int>, 2> A{vector<int>(N), vector<int>(N)};
		for (int z = 0; z < 2; z++) {
			for (int i = 0; i < N; i++) {
				cin >> A[z][i];
			}
			int v = *min_element(A[z].begin(), A[z].end());
			for (int i = 0; i < N; i++) {
				A[z][i] -= v;
			}
		}
		int64_t ans = 0;
		for (int i = 0; i < N; i++) {
			ans += max(A[0][i], A[1][i]);
		}
		cout << ans << '\n';
	}

	return 0;
}
