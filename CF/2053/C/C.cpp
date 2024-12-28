#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int64_t K, N; cin >> N >> K;
		int64_t num_segs = 1;
		int64_t tot_left = 1;
		int64_t cur_len = N;
		int64_t ans = 0;
		while (cur_len >= K) {
			if (cur_len & 1) {
				ans += tot_left + (cur_len / 2) * num_segs;
			}
			tot_left += tot_left + ((cur_len + 1) / 2) * num_segs;
			cur_len /= 2;
			num_segs *= 2;
		}
		cout << ans << '\n';
	}

	return 0;
}
