#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> H(N);
		for (auto& h : H) cin >> h;
		int64_t cnt_incr = 0;
		int64_t cnt_decr = 0;
		int64_t ans = 0;
		for (int i = 0; i+1 < N; i++) {
			if (H[i+1] > H[i]) {
				cnt_incr++;
				cnt_decr = 0;
			} else if (H[i+1] < H[i]) {
				cnt_decr += cnt_incr;
				cnt_incr = 0;
				ans += cnt_decr;
			} else {
				cnt_incr = cnt_decr = 0;
			}
		}
		cout << ans << '\n';
	}

	return 0;
}
