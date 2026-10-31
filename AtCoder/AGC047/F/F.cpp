#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int N; cin >> N;
	vector<int64_t> Xs(N);
	vector<int64_t> Ys(N);
	vector<array<int64_t, 2>> rooks(N);
	for (int i = 0; i < N; i++) {
		int x, y; cin >> x >> y;
		Xs[i] = x, Ys[i] = y;
		rooks[i] = {x, y};
	}
	sort(Xs.begin(), Xs.end());
	sort(Ys.begin(), Ys.end());
	vector<int> P(N);
	vector<int> invP(N);
	vector<int> rookID(N);
	for (int i = 0; i < N; i++) {
		int x = int(lower_bound(Xs.begin(), Xs.end(), rooks[i][0]) - Xs.begin());
		int y = int(lower_bound(Ys.begin(), Ys.end(), rooks[i][1]) - Ys.begin());
		rookID[x] = i;
		P[x] = y;
		invP[y] = x;
	}

	vector<int64_t> ans(N);

	vector<array<int, 2>> runs; runs.reserve(N);
	for (int i = 0; i < N; i++) {
		int j = i;
		while (j+1 < N && abs(P[j] - P[j+1]) == 1) {
			j++;
		}
		runs.push_back({i,j});
		i = j;
	}

	int R = int(runs.size());
	for (int st = 0; st < R; st++) {
		int a = st, b = st;
		int x0 = runs[st][0], y0 = P[x0];
		int x1 = runs[st][1], y1 = P[x1];
		int64_t cost[2][2];
		cost[0][0] = cost[1][1] = 2 * (abs(Xs[x0] - Xs[x1]) + abs(Ys[y0] - Ys[y1]));
		cost[0][1] = cost[1][0] = abs(Xs[x0] - Xs[x1]) + abs(Ys[y0] - Ys[y1]);
		while (true) {
			assert(x0 <= x1 && abs(x0 - x1) == abs(y0 - y1));
			bool can_left = x0-1 >= 0 && abs(P[x0-1] - y1) == 1;
			bool can_right = x1+1 < N && abs(P[x1+1] - y0) == 1;
			if (!(can_left || can_right)) break;
			if (can_left) a--;
			if (can_right) b++;
			int nx0, ny0, nx1, ny1;
			if (can_left) {
				nx0 = runs[a][0];
				ny0 = P[nx0];
			} else {
				nx0 = x0;
				ny0 = y1;
			}
			if (can_right) {
				nx1 = runs[b][1];
				ny1 = P[nx1];
			} else {
				nx1 = x1;
				ny1 = y0;
			}

			for (int s = 0; s < 2; s++) {
				int64_t nc0 = min(
					cost[s][0] + abs(Xs[x0] - Xs[nx0]) + abs(Ys[y0] - Ys[ny0]),
					cost[s][1] + abs(Xs[x1] - Xs[nx0]) + abs(Ys[y1] - Ys[ny0])
				);
				int64_t nc1 = min(
					cost[s][0] + abs(Xs[x0] - Xs[nx1]) + abs(Ys[y0] - Ys[ny1]),
					cost[s][1] + abs(Xs[x1] - Xs[nx1]) + abs(Ys[y1] - Ys[ny1])
				);
				int64_t nd = abs(Xs[nx0] - Xs[nx1]) + abs(Ys[ny0] - Ys[ny1]);
				if (can_left && can_right) {
					cost[s][0] = nc1 + nd;
					cost[s][1] = nc0 + nd;
				} else if (can_left) {
					cost[s][0] = nc0;
					cost[s][1] = nc0 + nd;
				} else if (can_right) {
					cost[s][0] = nc1 + nd;
					cost[s][1] = nc1;
				} else assert(false);
			}

			x0 = nx0;
			y0 = ny0;
			x1 = nx1;
			y1 = ny1;
		}

		int num_rooks = x1 - x0 + 1;
		for (int i = runs[st][0]; i <= runs[st][1]; i++) {
			int64_t cnds[2];
			for (int z = 0; z < 2; z++) {
				cnds[z] = abs(Xs[i] - Xs[runs[st][z]]) + abs(Ys[P[i]] - Ys[P[runs[st][z]]]) + min(cost[z][0], cost[z][1]);
			}
			ans[rookID[i]] = min(cnds[0], cnds[1]) - (num_rooks - 1);
		}
	}

	for (int i = 0; i < N; i++) {
		cout << ans[i] << '\n';
	}

	return 0;
}
