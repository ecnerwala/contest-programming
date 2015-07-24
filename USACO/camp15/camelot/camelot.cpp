#include<bits/stdc++.h>
using namespace std;

const int INF = 1e9;

const int MAXR = 25;
const int MAXC = 25;

const int MAXV = MAXR * MAXC;

const int MAXN = 300;

int R, C;
int V;
int N;
int W[MAXN];

int dist[MAXV][MAXV];

int mx[8] = {-1, -1, 1, 1, -2, -2, 2, 2}, my[8] = {-2, 2, -2, 2, -1, 1, -1, 1};

int main() {
	cin >> N >> R >> C;
	V = R * C;

	for(int i = 0; i < V; i++) {
		for(int j = 0; j < V; j++) {
			dist[i][j] = INF;
		}
		dist[i][i] = 0;
	}

	for(int i = 0; i < R; i++) {
		for(int j = 0; j < C; j++) {
			for(int d = 0; d < 8; d++) {
				int x = i + mx[d];
				int y = j + my[d];

				if(x < 0 || R <= x) continue;
				if(y < 0 || C <= y) continue;

				dist[i * C + j][x * C + y] = 1;
			}
		}
	}

	for(int k = 0; k < V; k++) {
		for(int i = 0; i < V; i++) {
			for(int j = 0; j < V; j++) {
				dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
			}
		}
	}

	for(int i = 0; i < N; i++) {
		int x, y; cin >> x >> y;
		x--, y--;
		W[i] = x * C + y;
	}

	int res = INF;
	for(int l = 0; l < V; l++) {
		for(int r = 0; r < V; r++) {
			vector<int> c1, c2;
			int val = 0;
			for(int i = 0; i < N; i++) {
				int d1 = dist[W[i]][l], d2 = dist[W[i]][r];
				val += min(d1, d2);
				if(d1 < d2) {
					c1.push_back(d2 - d1);
				} else {
					c2.push_back(d1 - d2);
				}
			}
			if(c1.size() < c2.size()) {
				swap(c1, c2);
			}
			assert(int(c1.size()) >= N / 2);
			sort(c1.begin(), c1.end());
			for(int i = 0; i < int(c1.size()) - N / 2; i++) {
				val += c1[i];
			}

			res = min(res, val);
		}
	}

	cout << res << '\n';
	return 0;
}
