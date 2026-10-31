#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int N, M; cin >> N >> M;
	vector<vector<int>> stones(M);
	for (int i = 0; i < M; i++) {
		int r; cin >> r; stones[i].resize(r);
		for (auto& v : stones[i]) {
			cin >> v; v--;
		}
		reverse(stones[i].begin(), stones[i].end());
	}

	vector<pair<int, int>> ops; ops.reserve(int(5.1e5));
	auto move = [&](int from, int to) {
		assert(!stones[from].empty());
		stones[to].push_back(stones[from].back());
		stones[from].pop_back();
		ops.push_back({from, to});
	};

	// move everything to the start
	for (int i = 1; i < M; i++) {
		while (!stones[i].empty()) move(i, 0);
	}

	// move everything from the start to the right point
	while (!stones[0].empty()) {
		int to = max(1, stones[0].back());
		move(0, to);
	}

	while (!stones[1].empty()) {
		int to = stones[1].back();
		assert(to == 0 || to == 1);
		move(1, to * 2);
	}

	while (!stones[2].empty() && stones[2].back() == 1) {
		move(2, 1);
	}

	for (int i = 0; i < M; i++) {
		for (int v : stones[i]) assert(v == i);
	}

	cout << ops.size() << '\n';
	for (auto [a, b] : ops) {
		cout << a+1 << ' ' << b+1 << '\n';
	}

	return 0;
}
