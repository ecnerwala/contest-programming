#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		vector<int> A(N); for (auto& a : A) cin >> a;
		int cnt_decr = 0;
		for (int i = 0; i+1 < N; i++) {
			cnt_decr += A[i] > A[i+1];
		}
		vector<tuple<int, int, int>> ans; ans.reserve(N/2);
		if (cnt_decr > N/2) {
			ans.push_back({4, 1, (1 << 30) - 1});
			for (auto& a : A) a ^= (1 << 30) - 1;
		}
		for (int i = 0; i+1 < N; i++) {
			if (A[i] > A[i+1]) ans.push_back({2, i+2, (1 << 30) - 1});
		}
		cout << ans.size() << '\n';
		for (auto [t, p, k] : ans) cout << t << ' ' << p << ' ' << k << '\n';
	}

	return 0;
}

// Main idea: make decreasing runs increasing by xoring with 2^30-1
