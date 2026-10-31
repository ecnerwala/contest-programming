#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int N, K; cin >> N >> K;
	vector<pair<int, int>> ans;
	for (int i = N / 2, j = N-1; i < N; i++, j -= 2) {
		ans.emplace_back(K + i, K + N + j);
	}
	for (int i = 0, j = N - 2; i < N / 2; i++, j -= 2) {
		ans.emplace_back(K + i, K + N + j);
	}
	sort(ans.begin(), ans.end(), [&](pair<int, int> a, pair<int, int> b) {
		return a.first + a.second < b.first + b.second;
	});

	for (int i = 0; i < N; i++) {
		if (ans[i].first + ans[i].second > K + N + N + i) {
			cout << -1 << '\n'; exit(0);
		}
	}
	for (int i = 0; i < N; i++) {
		cout << ans[i].first << ' ' << ans[i].second << ' ' <<  K + N + N + i << '\n';
	}

	return 0;
}

//
// 1,2 3
//
// 1,2,3,4 -> 1,4,5 and 2,3,5 (value 6)
//
// 1,2,3,4,5,6 -> 2,6 = 8, 3,4 = 7, 1,5 = 6
//
//
// 1,2,3,4,5,6,7,8 -> 3,7 = 10, 4,5 = 9, 2,6 = 8, 1,8 = 9
//
// 1,2,3,4,5,6,7,8,9,10 -> 3,10 = 13, 4,8 = 12, 5,6 = 11, 1,9 = 10, 2,7 = 9
