#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int N, M; cin >> N >> M;
	vector<string> grid(N);
	for (int i = 0; i < N; i++) {
		cin >> grid[i];
	}

	vector<vector<std::optional<pair<int, int>>>> vis(N, vector<std::optional<pair<int, int>>>(M, std::nullopt));

	int numDim = 0;
	pair<int, int> curVec;
	int curVecMult;

	vector<pair<int, int>> q;
	q.push_back({0,0});

	for (int z = 0; z < int(q.size()); z++) {
		auto [x,y] = q[z];

		int i = x%N; if (i < 0) i += N;
		int j = y%M; if (j < 0) j += M;
		if (grid[i][j] == '#') continue;

		if (vis[i][j]) {
			int dx = (vis[i][j]->first - x) / N;
			int dy = (vis[i][j]->second - y) / M;
			if (dx < 0 || (dx == 0 && dy < 0)) dx = -dx, dy = -dy;
			if (dx || dy) {

				int g = gcd(dx, dy);
				dx /= g, dy /= g;

				if (numDim == 0) {
					numDim = 1;
					curVec = {dx, dy};
					curVecMult = g;
				} else if (numDim == 1) {
					if (curVec != pair<int, int>(dx, dy)) {
						numDim = 2;
					} else {
						curVecMult = gcd(curVecMult, g);
					}
				}
			}

			continue;
		}

		vis[i][j] = {x,y};
		for (auto [dx, dy] : {pair<int, int>{-1, 0}, {0, 1}, {1, 0}, {0, -1}}) {
			q.push_back({x + dx, y + dy});
		}
	}

	auto query = [&](int x, int y) -> bool {
		int i = x%N; if (i < 0) i += N;
		int j = y%M; if (j < 0) j += M;
		if (!vis[i][j]) return false;

		if (numDim == 2) return true;

		int dx = (vis[i][j]->first - x) / N;
		int dy = (vis[i][j]->second - y) / M;

		if (dx < 0 || (dx == 0 && dy < 0)) dx = -dx, dy = -dy;

		if (dx == 0 && dy == 0) return true;
		if (numDim == 0) return false;

		int g = gcd(dx, dy);
		dx /= g, dy /= g;

		return (curVec == pair<int, int>(dx, dy)) && (g % curVecMult == 0);
	};

	int Q; cin >> Q;
	while (Q--) {
		int x, y; cin >> x >> y;
		bool ans = query(x, y);
		cout << (ans ? "yes" : "no") << '\n';
	}

	return 0;
}
