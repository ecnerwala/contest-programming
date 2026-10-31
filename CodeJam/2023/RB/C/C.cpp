#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {

		int N, K; cin >> N >> K;
		std::vector<std::pair<int, int>> A(N);
		for (int i = 0; i < N; i++) {
			cin >> A[i].first;
			A[i].second = i;
		}
		std::sort(A.begin(), A.end(), [&](const auto& a, const auto& b) { return a.first < b.first; });

		std::vector<int> ans(N, -1);
		for (int z = 0; z < 2; z++) {
			std::reverse(A.begin(), A.end());
			for (auto& [v, _] : A) v = -v;

			int cnt = 0;
			int prv = 0;
			for (auto [v, i] : A) {
				if (cnt == 0 || v - prv >= K) {
					cnt++;
					prv = v;
				}
				ans[i] += cnt;
			}
		}

		cout << "Case #" << case_num << ": ";
		for (int i = 0; i < N; i++) {
			cout << ans[i] << " \n"[i+1==N];
		}
	}

	return 0;
}
