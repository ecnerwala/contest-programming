#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<bool> has_diff(N+1);
		bool good = true;
		int64_t extra_diff = 0;
		int64_t cur = 0;
		for (int i = 0; i < N-1; i++) {
			int64_t a; cin >> a;
			int64_t diff = a - cur;
			assert(diff > 0);
			if (diff <= N && !has_diff[diff]) {
				has_diff[diff] = true;
			} else {
				if (extra_diff) {
					good = false;
				} else {
					extra_diff = diff;
				}
			}
			cur = a;
		}
		if (!good) {
			cout << "NO" << '\n';
			continue;
		}
		if (extra_diff == 0) {
			cout << "YES" << '\n';
		} else {
			if (cur == int64_t(N) * int64_t(N+1) / 2) {
				cout << "YES" << '\n';
			} else {
				cout << "NO" << '\n';
			}
		}
	}

	return 0;
}
