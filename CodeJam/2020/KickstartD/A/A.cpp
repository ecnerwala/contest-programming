#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		int N; cin >> N;
		vector<int> V(N);
		for (int i = 0; i < N; i++) {
			cin >> V[i];
		}
		int cur_max = -1;
		int ans = 0;
		for (int i = 0; i < N; i++) {
			ans += V[i] > cur_max && (i+1==N || V[i] > V[i+1]);
			cur_max = max(cur_max, V[i]);
		}

		cout << "Case #" << case_num << ": " << ans << '\n';
	}

	return 0;
}
