#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M; cin >> N >> M;
	vector<int> X(N); for (auto& x : X) cin >> x;

	vector<vector<pair<int, bool>>> switch_boxes(M); // true if it opens it
	int R; cin >> R;
	for (int r = 0; r < R; r++) {
		int s, b; cin >> s >> b; s--, b--;
		string op; cin >> op;
		if (op == "open") {
			switch_boxes[s].push_back({b, true});
		} else {
			switch_boxes[s].push_back({b, false});
		}
	}

	for (int dir = 0; dir < 2; dir++) {
		vector<vector<int>> closers(N);
		vector<int> num_closers(M, 0);
		for (int m = 0; m < M; m++) {
			for (auto [b, opens] : switch_boxes[m]) {
				if (!opens) {
					closers[b].push_back(m);
					num_closers[m]++;
				}
			}
		}
		
		vector<int> cur_closers(M);
		vector<bool> is_opened(N);
		vector<int> q; q.reserve(N);
		auto is_interval_good = [&](int lo, int hi) -> bool {
			fill(cur_closers.begin(), cur_closers.end(), 0);

			q.clear();
			fill(is_opened.begin(), is_opened.end(), false);

			for (int i = lo; i < hi; i++) {
				is_opened[i] = true;
				q.push_back(i);
			}

			auto check_m = [&](int m) {
				if (cur_closers[m] == num_closers[m]) {
					// we can actually use this switch
					for (auto [b, _] : switch_boxes[m]) {
						if (!is_opened[b]) {
							is_opened[b] = true;
							q.push_back(b);
						}
					}
				}
			};

			for (int m = 0; m < M; m++) check_m(m);

			for (int z = 0; z < int(q.size()); z++) {
				int cur = q[z];
				for (int m : closers[cur]) {
					// now these guys are more free
					cur_closers[m]++;
					check_m(m);
				}
			}

			return int(q.size()) == N;
		};

		int ans = max(X.back(), 0) - min(X.front(), 0);
		for (int i = 0, j = 0; i < N; i++) {
			while (j <= N && !is_interval_good(i, j)) j++;
			if (j == N+1) break;
			if (i == j) {
				ans = 0;
				break;
			}
			ans = min(ans, max(X[j-1], 0) - min(X[i], 0));
		}

		cout << ans * 2 << '\n';

		for (int m = 0; m < M; m++) {
			for (auto& b : switch_boxes[m]) b.second = !b.second;
		}
	}

	return 0;
}
