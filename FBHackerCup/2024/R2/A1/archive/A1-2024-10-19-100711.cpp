#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	std::vector<int64_t> peaks;
	for (int m = 1; m <= 9; m++) {
		for (int k = 0; k < m; k++) {
			int64_t v = 0;
			for (int i = k; i > 0; i--) {
				v *= 10;
				assert(m-i > 0);
				v += m-i;
			}
			for (int i = 0; i <= k; i++) {
				v *= 10;
				v += m-i;
			}
			peaks.push_back(v);
			cerr << v << '\n';
		}
	}

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {

		int64_t A, B, M; cin >> A >> B >> M;
		int ans = 0;
		for (auto v : peaks) {
			if (A <= v && v <= B && v % M == 0) ans++;
		}

		cout << "Case #" << case_num << ": " << ans << '\n';
	}

	return 0;
}
