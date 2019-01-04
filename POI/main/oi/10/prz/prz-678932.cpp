#include<bits/stdc++.h>
using namespace std;

const int INF = 5e8;

typedef pair<int, int> pii;
#define A first
#define B second

const int MAXN = 1e4;
int N;
int P[MAXN];

int M;
vector<pii> adj[2][MAXN];

int mindist[2][MAXN];

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N;
	for(int i = 0; i < N; i++) {
		cin >> P[i];
		P[i] /= 2;
	}
	cin >> M;
	for(int i = 0; i < M; i++) {
		int a, b, c; cin >> a >> b >> c;
		a--, b--;
		adj[0][a].push_back(pii(b, c));
		adj[1][b].push_back(pii(a, c));
	}

	for(int t = 0; t < 2; t++) {
		for(int i = 0; i < N; i++) {
			mindist[t][i] = INF;
		}
		priority_queue<pii, vector<pii>, greater<pii> > pq;
		mindist[t][0] = 0;
		pq.push(pii(mindist[t][0], 0));
		while(!pq.empty()) {
			int d = pq.top().A, c = pq.top().B;
			pq.pop();
			if(d != mindist[t][c]) continue;
			for(vector<pii>::iterator it = adj[t][c].begin(); it != adj[t][c].end(); it++) {
				int n = it->A;
				int nd = it->B + d;
				if(nd < mindist[t][n]) {
					mindist[t][n] = nd;
					pq.push(pii(nd, n));
				}
			}
		}
	}
	int res = P[0];
	for(int i = 1; i < N; i++) {
		res = min(res, mindist[0][i] + mindist[1][i] + P[i]);
	}
	cout << res << '\n';
}
