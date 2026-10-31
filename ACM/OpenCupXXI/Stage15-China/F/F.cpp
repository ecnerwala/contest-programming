#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M; cin >> N >> M;
	vector<pair<array<int, 2>, int>> P(N+M);
	for (int i = 0; i < N+M; i++) {
		cin >> P[i].first[0] >> P[i].first[1];
		P[i].second = i;
	}
	string ans(N+M, '0');

	for (int z = 0; z < 2; z++) {
		for (auto& v : P) swap(v.first[0], v.first[1]);
		sort(P.begin(), P.end());
		for (int i = 0; i+1 < int(P.size()); i++) {
			if (P[i].first[0] != P[i+1].first[0]) continue;
			if ((P[i].second < N) != (P[i+1].second < N)) {
				ans[P[i].second] = '1';
				ans[P[i+1].second] = '1';
			}
		}
	}

	cout << ans.substr(0, N) << '\n';
	cout << ans.substr(N) << '\n';

	return 0;
}
