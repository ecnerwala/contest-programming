#include<bits/stdc++.h>
using namespace std;

const int INF = 1e8 + 200;

typedef pair<int, int> pii;
#define A first
#define B second

const int MAXN = 2e4;
const int MAXM = 2e5;

int N, M, K;
vector<pii> adj[MAXN];

int dist1[MAXN];
int dist2[MAXN];

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N >> M >> K;
	for(int i = 0; i < M; i++) {
		int a, b, d; cin >> a >> b >> d; a--, b--;
		adj[a].push_back(pii(d, b));
		adj[b].push_back(pii(d, a));
	}

	for(int i = 0; i < N; i++) {
		dist1[i] = dist2[i] = INF;
	}

	priority_queue<pii, vector<pii>, greater<pii>> pq;
	for(int i = 0; i < K; i++) {
		dist1[i] = dist2[i] = 0;
		pq.push(pii(0, i));
	}

	while(!pq.empty()) {
		int v = pq.top().A, c = pq.top().B;
		pq.pop();
		if(v > dist2[c]) continue;
		assert(v == dist2[c]);
		for(pii n : adj[c]) {
			int d = v + n.A;
			int i = n.B;
			if(d < dist1[i]) { 
				if(dist1[i] < dist2[i]) {
					dist2[i] = dist1[i];
					pq.push(pii(dist2[i], i));
				} else {
					// do nothing
				}
				dist1[i] = d;
			} else if(d < dist2[i]) {
				dist2[i] = d;
				pq.push(pii(dist2[i], i));
			}

		}
	}

	int res = dist2[N - 1];
	if(res == INF) {
		cout << -1 << '\n';
	} else {
		cout << res << '\n';
	}
	return 0;
}
