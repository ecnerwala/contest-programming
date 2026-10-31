#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M; cin >> N >> M;
	vector<array<int, 2>> bounds(M);
	for (auto& b : bounds) cin >> b[0] >> b[1], b[0]--, b[1]--;

	vector<vector<int>> by_start(N);
	vector<vector<int>> by_end(N);
	for (int j = 0; j < M; j++) {
		by_start[bounds[j][0]].push_back(j);
		by_end[bounds[j][1]].push_back(j);
	}

	// We'll get a 2 coloring problem
	vector<vector<int>> adj(M);

	int num_2 = 0;
	vector<int> cover_cnt(N);

	set<int> cur_cover;
	for (int j = 0; j < M; j++) {
		if (bounds[j][0] > bounds[j][1]) {
			cur_cover.insert(j);
		}
	}
	for (int i = 0; i < N; i++) {
		for (int j : by_start[i]) cur_cover.insert(j);

		cover_cnt[i] = int(cur_cover.size());

		if (cur_cover.size() <= 1) {
			cout << "impossible" << '\n';
			exit(0);
		} else if (cur_cover.size() == 2) {
			num_2++;
			auto it = cur_cover.begin();
			int x = *it;
			++it;
			int y = *it;
			adj[x].push_back(y);
			adj[y].push_back(x);
		}

		for (int j : by_end[i]) cur_cover.erase(j);
	}

	vector<bool> vis(M, false);
	vector<bool> alive(M, false);
	vector<int> q; q.reserve(M);
	for (int st = 0; st < M; st++) {
		if (!vis[st]) {
			vis[st] = true;
			alive[st] = true;
			q.push_back(st);
			for (int z = int(q.size())-1; z < int(q.size()); z++) {
				int cur = q[z];
				for (int nxt : adj[cur]) {
					if (vis[nxt]) {
						if (alive[nxt] == alive[cur]) {
							cout << "impossible" << '\n';
							exit(0);
						}
						continue;
					}
					vis[nxt] = true;
					alive[nxt] = !alive[cur];
					q.push_back(nxt);
				}
			}
		}
	}

	auto interval_len = [&](int x, int y) -> int {
		if (x <= y) return y - x + 1;
		else return N + y - x + 1;
	};

	cur_cover.clear();
	for (int j = 0; j < M; j++) {
		if (!alive[j]) continue;
		if (bounds[j][0] > bounds[j][1]) {
			cur_cover.insert(j);
		}
	}
	for (int i = 0; i < N; i++) {
		for (int j : by_start[i]) if (alive[j]) cur_cover.insert(j);

		assert(!cur_cover.empty());

		int most_left = -1;
		int best_left = -1;
		int most_right = -1;
		int best_right = -1;
		for (int j : cur_cover) {
			assert(alive[j]);
			int cur_left = interval_len(bounds[j][0], i);
			if (cur_left > most_left) most_left = cur_left, best_left = j;
			int cur_right = interval_len(i, bounds[j][1]);
			if (cur_right > most_right) most_right = cur_right, best_right = j;
			assert(cur_left + cur_right - 1 == interval_len(bounds[j][0], bounds[j][1]));
		}
		assert(best_left != -1 && best_right != -1);
		for (int j : cur_cover) {
			if (j != best_left && j != best_right) {
				alive[j] = false;
			}
		}
		cur_cover = set<int>({best_left, best_right});
		assert(cur_cover.size() <= 2);
		assert(cur_cover.size() >= 1);

		for (int j : by_end[i]) if (alive[j]) cur_cover.erase(j);
	}

	for (int j = 0; j < M; j++) { cout << alive[j]; } cout << '\n';

	return 0;
}
