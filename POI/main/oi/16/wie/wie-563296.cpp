#include<iostream>
#include<vector>
#include<queue>

using namespace std;

typedef pair<int, int> pii;
typedef pair<int, pii> pip;

#define A first
#define B second

const int INF = 1e9;

const int MAXN = 220;
const int MAXP = 13;

int N;
int M;
int P;
int K;
int smiths[MAXN];
vector<pip> adj[MAXN]; //dest, time, restrictions
int minDist[MAXN][1 << MAXP];

priority_queue<pip, vector<pip>, greater<pip> > pq;

int go() {
	for(int i = 0; i < N; i++) {
		for(int b = 0; b < (1 << P); b++) {
			minDist[i][b] = INF;
		}
	}

	minDist[0][smiths[0]] = 0;
	pq.push(pip(0, pii(0, smiths[0])));

	while(!pq.empty()) {
		pip cur = pq.top(); pq.pop();
		int dist = cur.A;
		int loc = cur.B.A;
		int swords = cur.B.B;
		if(loc == N - 1) return dist;
		if(minDist[loc][swords] == dist) {
			//cerr << loc << ' ' << swords << ' ' << dist << '\n';
			for(int i = 0; i < adj[loc].size(); i++) {
				int monst = adj[loc][i].B.B;
				if((monst & swords) == monst) {
					int nxt = adj[loc][i].A;
					int ndist = dist + adj[loc][i].B.A;
					int nswo = swords | smiths[nxt];
					int cdist = minDist[nxt][nswo];
					if(ndist < cdist) {
						minDist[nxt][nswo] = ndist;
						pq.push(pip(ndist, pii(nxt, nswo)));
					}
				}
			}
		}
	}

	return -1;
}

int main() {
	cin >> N >> M >> P >> K;

	for(int i = 0; i < K; i++) {
		int w, q; cin >> w >> q;
		w--;
		for(int j = 0; j < q; j++) {
			int r; cin >> r;
			r--;
			smiths[w] |= (1 << r);
		}
	}

	for(int i = 0; i < M; i++) {
		int v, w, t, s; cin >> v >> w >> t >> s;
		v--, w--;
		int b = 0;
		for(int j = 0; j < s; j++) {
			int u; cin >> u; u--;
			b |= (1 << u);
		}

		adj[v].push_back(pip(w, pii(t, b)));
		adj[w].push_back(pip(v, pii(t, b)));
	}

	int val = go();
	cout << val << '\n';
	return 0;
}
