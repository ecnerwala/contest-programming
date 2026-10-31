#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M; cin >> N >> M;
		vector<int> K(N); for (auto& v : K) { cin >> v; v--; }
		vector<int> C(M); for (auto& c : C) { cin >> c; }
		int64_t ans = 0;
		sort(K.begin(), K.end());
		int num_presents = 0;
		int cur_present = M;
		for (int i = N-1; i >= 0; i--) {
			if (cur_present == 0 || K[i] <= num_presents) {
				ans += C[K[i]];
			} else {
				ans += C[num_presents++];
				cur_present = min(cur_present-1, K[i]);
			}
		}
		cout << ans << '\n';
	}

	return 0;
}
