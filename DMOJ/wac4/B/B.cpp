#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
	int N; cin >> N;
	vector<pair<int64_t, int>> nations;
	int totP = 0;
	int64_t totV = 0;
	for (int i = 0; i < N; i++) {
		int64_t v; int p; cin >> v >> p;
		v = v / 2 + 1;
		nations.emplace_back(v, p);
		totV += v;
		totP += p;
	}
	vector<int64_t> minCost(totP+1, totV+1);
	minCost[0] = 0;
	for (int i = 0; i < N; i++) {
		int64_t v, p; tie(v, p) = nations[i];
		for (int a = totP; a >= p; a--) {
			minCost[a] = min(minCost[a], minCost[a-p] + v);
		}
	}
	int64_t ans = totV+1;
	for (int p = totP / 2 + 1; p <= totP; p++) {
		ans = min(ans, minCost[p]);
	}
	cout << ans << '\n';

	return 0;
}
