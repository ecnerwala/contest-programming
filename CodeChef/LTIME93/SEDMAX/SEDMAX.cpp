#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;

		vector<pair<int, int>> A(N); for (auto& a : A) cin >> a.first;
		for (int i = 0; i < N; i++) A[i].second = i;

		vector<int> ans; ans.reserve(2*N);
		for (int z = 0; z < 2; z++, reverse(A.begin(), A.end())) {
			vector<pair<int, int>> cur_stk; cur_stk.reserve(N);
			for (int i = 0; i < N; i++) {
				while (!cur_stk.empty() && cur_stk.back() < A[i]) cur_stk.pop_back();
				if (!cur_stk.empty()) ans.push_back(cur_stk.back().first - A[i].first);
				cur_stk.push_back(A[i]);
			}
		}
		sort(ans.begin(), ans.end());
		ans.resize(unique(ans.begin(), ans.end()) - ans.begin());
		cout << ans.size() << '\n';
	}

	return 0;
}

// If you fix the 2nd largest, the largest has to be the next guy bigger in either direction
