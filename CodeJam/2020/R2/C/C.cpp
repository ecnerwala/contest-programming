#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
	int T; cin >> T;

	for (int case_num = 1; case_num <= T; case_num ++) {

		int N; cin >> N;
		vector<pair<int, int>> pts(N);
		for (int i = 0; i < N; i++) cin >> pts[i].first >> pts[i].second;

		sort(pts.begin(), pts.end());

		map<pair<int, int>, set<int>> slopes;
		for (int i = 0; i < N; i++) {
			for (int j = i+1; j < N; j++) {
				int dx = pts[j].first - pts[i].first;
				int dy = pts[j].second - pts[i].second;
				assert(dx || dy);
				int g = __gcd(dx, dy);
				dx /= g, dy /= g;
				slopes[{dx, dy}].insert(i);
				slopes[{dx, dy}].insert(j);
			}
		}

		int ans = 2;

		for (auto& it : slopes) {
			int totCnt = int(it.second.size());
			++totCnt;
			ans = max(ans, totCnt + (totCnt&1));
		}

		ans = min(ans, N);

		cout << "Case #" << case_num << ": " << ans << '\n';
	}

	return 0;
}
