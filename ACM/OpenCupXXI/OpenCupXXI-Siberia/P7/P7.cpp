#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int N, T; cin >> N >> T;
	vector<int> P(N); for (int& p : P) cin >> p;

	vector<int> ans(N);

	priority_queue<pair<int, int>> pq;
	for (int i = 0; i < N; i++) {
		pq.push({P[i], -i});
	}
	for (int t = 1; t <= T; t++) {
		int i = -pq.top().second; pq.pop();
		ans[i]++;
		pq.push({P[i] - t, -i});
	}

	for (int i = 0; i < N; i++) {
		cout << ans[i] << " \n"[i+1==N];
	}

	return 0;
}
