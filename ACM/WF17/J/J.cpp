#include<bits/stdc++.h>
using namespace std;

const double INF = 1e9;
const double EPS = 1e-9;

const int MAXN = 400;
int N;
double adj[MAXN][MAXN];
double flow[MAXN][MAXN]; // flow graph

double run_flow(int s, int t) {
	double totflow = 0;
	while (true) {
		double value[MAXN];
		int prv[MAXN];
		queue<int> q;
		memset(value, 0, sizeof(value));
		memset(prv, -1, sizeof(prv));

		prv[s] = -1;
		value[s] = INF;
		q.push(s);
		while (!q.empty()) {
			int cur = q.front(); q.pop();
			if (cur == t) break;
			for (int nxt = 0; nxt < N; nxt++) {
				if (value[nxt] > EPS) continue;
				if (adj[cur][nxt] - flow[cur][nxt] < EPS) continue;
				prv[nxt] = cur;
				value[nxt] = min(value[cur], adj[cur][nxt] - flow[cur][nxt]);
				q.push(nxt);
			}
		}

		if (value[t] < EPS) {
			break;
		}

		totflow += value[t];
		for (int cur = t; cur != s; cur = prv[cur]) {
			flow[prv[cur]][cur] += value[t];
			flow[cur][prv[cur]] -= value[t];
		}
	}
	return totflow;
}

int M;
vector<pair<int, int>> edges;

double V, A, B;

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N;
	cin >> M >> V >> A;
	B = 1-A;
	for (int i = 0; i < M; i++) {
		int j, k; cin >> j >> k;
		int c; cin >> c;
		edges.emplace_back(j, k);
		adj[j][k] += c;
		adj[k][j] += c;
	}

	memcpy(cap, adj, sizeof(adj));

	adj[0][1] += INF;
	adj[0][2] += INF;

	double totflow = run_flow(0, 3);
	double flowA = flow[0][1];
	double flowB = flow[0][2];

	if (flowA > totflow * A) {
	}

	return 0;
}
