#include<bits/stdc++.h>
using namespace std;

const int INF = 1e8;

const int MAXN = 300;
int N;
vector<int> adjList[MAXN];
bool adj[MAXN][MAXN];

int dist[MAXN][MAXN];

int par[MAXN];
int getpar(int a) { return par[a] == a ? a : (par[a] = getpar(par[a])); }
bool merge(int a, int b) {
	a = getpar(a), b = getpar(b);
	if (a == b) {
		return false;
	}
	par[b] = a;
	return true;
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N;
	for (int i = 0; i < N; i++) {
		string s; cin >> s;
		for (int j = 0; j < N; j++) {
			adj[i][j] = bool(s[j] - '0');
			if (adj[i][j]) {
				adjList[i].push_back(j);
			}
		}
	}

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (i==j) dist[i][j] = 0;
			else if (adj[i][j]) dist[i][j] = 1;
			else dist[i][j] = INF;
		}
	}
	for (int k = 0; k < N; k++) {
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
			}
		}
	}
	int ans = 0;
	for (int i = 0; i < N; i++) {
		bool visited = false;
		for (int j = 0; j < i; j++) {
			if (dist[i][j] != INF) {
				visited = true;
			}
		}
		if (visited) continue;

		vector<int> verts;
		for (int j = i; j < N; j++) {
			if (dist[i][j] == INF) continue;
			verts.push_back(j);
		}

		int best = -1;
		for (int st : verts) {
			int maxDist = 0;
			for (int a : verts) {
				maxDist = max(maxDist, dist[st][a]);
				for (int b : verts) {
					if (adj[a][b]) {
						if (abs(dist[st][a] - dist[st][b]) != 1) {
							goto notGood;
						}
					}
				}
			}
			best = max(best, maxDist + 1);

			notGood:
			;
		}
		if (best == -1) {
			cout << -1 << '\n';
			exit(0);
		}
		ans += best;
	}
	cout << ans << '\n';

	return 0;
}
