#include<iostream>
#include<vector>

using namespace std;

typedef long long ll;

#define ITER(i, v) for(__typeof((v).begin()) i = (v).begin(); i != (v).end(); i++)

const int MAXN = 2e5;
const int MAXM = 1e6;

int N, M;
vector<int> adj[MAXN];

int numVis;
int vis[MAXN];
ll res[MAXN];
int sz[MAXN];
int minVal[MAXN];

ll sq(ll a) {
	return a * a;
}

void dfs(int cur, int par = -1) {
	vis[cur] = numVis++;
	sz[cur] = 1;
	res[cur] = sq(N - 1);
	ll topSZ = 0;
	ITER(it, adj[cur]) {
		int nxt = *it;
		if(nxt == par) continue;
		if(vis[nxt] == -1) {
			minVal[nxt] = vis[cur];
			dfs(nxt, cur);
			sz[cur] += sz[nxt];
			if(minVal[nxt] == vis[cur]) {
				res[cur] -= sq(sz[nxt]);
			} else {
				topSZ += sz[nxt];
			}
			if(minVal[nxt] < minVal[cur]) {
				minVal[cur] = minVal[nxt];
			}
		} else {
			minVal[cur] = min(minVal[cur], vis[nxt]);
		}
	}
	topSZ += N - sz[cur];
	res[cur] -= sq(topSZ);
	res[cur] += 2 * (N - 1); 
}

void go() {
	numVis = 0;
	for(int i = 0; i < N; i++) vis[i] = -1;
	minVal[0] = -1;
	dfs(0);
}

int main() {
	ios_base::sync_with_stdio(0);

	cin >> N >> M;
	for(int i = 0; i < M; i++) {
		int a, b; cin >> a >> b; a-- ,b--;
		adj[a].push_back(b);
		adj[b].push_back(a);
	}

	go();

	for(int i = 0; i < N; i++) {
		cout << res[i] << '\n';
	}
	return 0;
}

