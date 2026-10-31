#include<bits/extc++.h>
using namespace std;

const int MAXN = 2.1e5;
int N;
const int MAXM = 2.1e5;
int M;
vector<int> adj[MAXN];

bool vis[MAXN];
double dist[MAXN];
double fallbackFac[MAXN];
double fallbackCost[MAXN];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N >> M;
	for (int e = 0; e < M; e++) {
		int u, v; cin >> u >> v;
		u--, v--;
		adj[u].push_back(v);
		adj[v].push_back(u);
	}

	__gnu_pbds::priority_queue<pair<double, int>, greater<pair<double, int>>> q;
	vector<decltype(q)::point_iterator> its(N);

	for (int i = 0; i < N; i++) {
		fallbackFac[i] = 1;
		fallbackCost[i] = -1;
	}

	dist[N-1] = 0;
	its[N-1] = q.push({dist[N-1], N-1});

	while (!q.empty()) {
		int cur = q.top().second; q.pop();
		vis[cur] = true;
		//cerr << cur << ' ' << dist[cur] << '\n';
		for (int nxt : adj[cur]) {
			if (vis[nxt]) continue;

			if (its[nxt] == q.end()) {
				fallbackCost[nxt] = dist[cur] + 2;
				dist[nxt] = dist[cur] + 1.5;
				its[nxt] = q.push({dist[nxt], nxt});
				fallbackFac[nxt] = 0.25;
			} else {
				double d = 0.75 * fallbackCost[nxt] - (1 + 0.75 * dist[cur]);
				if (d > 0) {
					dist[nxt] -= fallbackFac[nxt] * d;
					fallbackFac[nxt] /= 4.0;
				}
				q.modify(its[nxt], {dist[nxt], nxt});
			}
		}
	}

	cout << fixed << setprecision(12) << dist[0] << '\n';

	return 0;
}
