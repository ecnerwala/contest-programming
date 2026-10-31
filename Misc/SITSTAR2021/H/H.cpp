#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int N; cin >> N;
	array<priority_queue<int, vector<int>, greater<int>>, 2> costs;
	for (int i = 0; i < N; i++) {
		int c, t; cin >> c >> t;
		costs[t-1].push(c);
	}

	// You need at least floor N/3 of each type, and
	int64_t cur_cost = 0;
	array<int, 2> cur_cnt{0,0};
	for (int i = 1; i <= N; i++) {
		bool is_possible = true;
		for (int z = 0; z < 2; z++) {
			while (cur_cnt[z] < i/3) {
				if (costs[z].empty()) {
					is_possible = false;
					break;
				}
				cur_cost += costs[z].top();
				cur_cnt[z]++;
				costs[z].pop();
			}
		}
		if (!is_possible) {
			cout << -1 << '\n';
			continue;
		}
		while (cur_cnt[0] + cur_cnt[1] < i) {
			int dir;
			if (costs[0].empty()) {
				dir = 1;
			} else if (costs[1].empty()) {
				dir = 0;
			} else {
				dir = (costs[1].top() < costs[0].top());
			}
			cur_cost += costs[dir].top();
			cur_cnt[dir]++;
			costs[dir].pop();
		}
		cout << cur_cost << '\n';
	}

	return 0;
}
