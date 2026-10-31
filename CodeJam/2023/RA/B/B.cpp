#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {

		int64_t M, R; int N; cin >> M >> R >> N;
		std::vector<int64_t> X(N);
		for (auto& x : X) cin >> x;

		int ans = 0;
		bool good = true;

		int64_t cur = -R;
		auto it = X.begin();
		while (cur < M-R) {
			int64_t nxt = cur;
			while (it != X.end() && *it <= cur + 2 * R) {
				nxt = *it;
				++it;
			}
			if (nxt == cur) {
				good = false;
				break;
			}
			cur = nxt;
			ans++;
		}

		cout << "Case #" << case_num << ": ";
		if (good) {
			cout << ans << '\n';
		} else {
			cout << "IMPOSSIBLE" << '\n';
		}
	}

	return 0;
}
