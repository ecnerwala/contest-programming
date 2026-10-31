#include<bits/stdc++.h>
using namespace std;

using ll = long long;
const ll INF = ll(1e18);

const int MAXN = 60;
const int MAXM = 1.1e3;
int N, M;
int X[MAXM], Y[MAXM];
ll Z[MAXM];

ll dist[MAXN][MAXN];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int T; cin >> T;
	for (int t = 1; t <= T; t++) {
		cin >> N >> M;
		for (int e = 0; e < M; e++) {
			cin >> X[e] >> Y[e] >> Z[e];
			X[e] --, Y[e] --;
			if (X[e] > Y[e]) swap(X[e], Y[e]);
			assert(X[e] < Y[e]);
		}

		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				dist[i][j] = (i == j) ? 0 : INF;
			}
		}
		for (int e = 0; e < M; e++) {
			dist[X[e]][Y[e]] = min(dist[X[e]][Y[e]], Z[e]);
			dist[Y[e]][X[e]] = min(dist[Y[e]][X[e]], Z[e]);
		}

		for (int k = 0; k < N; k++) {
			for (int i = 0; i < N; i++) {
				for (int j = 0; j < N; j++) {
					dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
				}
			}
		}

		bool isGood = true;
		set<pair<int, int>> edges;
		for (int e = 0; e < M; e++) {
			assert(X[e] < Y[e]);
			edges.emplace(X[e], Y[e]);
			if (dist[X[e]][Y[e]] != Z[e]) {
				isGood = false;
				break;
			}
		}

		cout << "Case #" << t << ": ";
		if (isGood) {
			cout << edges.size() << '\n';
			for (auto it : edges) {
				int i = it.first, j = it.second;
				cout << i+1 << ' ' << j+1 << ' ' << dist[i][j] << '\n';
			}
		} else {
			cout << "Impossible" << '\n';
			continue;
		}
	}

	return 0;
}
