#include<bits/stdc++.h>
using namespace std;
const int MAXN = 20000;
const int MAXD = 20;
const int MAXM = 40000;

typedef pair<int, int> pii;
typedef pair<long long, pii> edge;

int N, M, K;
edge E[MAXM];
vector<pair<long long, int>> adj[MAXN];

int cc[MAXN];
int getcc(int a) {
	return cc[a] == a ? a : cc[a] = getcc(cc[a]);
}

bool merge(int a, int b) {
	a = getcc(a), b = getcc(b);
	if(a == b) return false;
	cc[a] = b;
	return true;
}

int depth[MAXN];
int par[MAXN][MAXD];
long long dist[MAXN][MAXD];

void dfs(int cur) {
	for(int i = 0; par[cur][i]; i++) {
		assert(depth[par[cur][i]] + (1 << i) == depth[cur]);
		int p = par[cur][i];
		par[cur][i + 1] = par[p][i];
		dist[cur][i + 1] = max(dist[cur][i], dist[p][i]);
	}

	for(auto it : adj[cur]) {
		int d = it.first;
		int nxt = it.second;
		if(nxt == par[cur][0]) continue;

		depth[nxt] = depth[cur] + 1;
		par[nxt][0] = cur;
		dist[nxt][0] = d;

		dfs(nxt);
	}
}

long long query(int a, int b) {
	assert(a != b);

	long long res = 0;

	if(depth[a] < depth[b]) {
		swap(a, b);
	}
	assert(depth[a] >= depth[b]);

	for(int i = 0, d = depth[a] - depth[b]; d; i ++, d /= 2) {
		if(d & 1) {
			res = max(res, dist[a][i]);
			a = par[a][i];
		}
	}

	assert(depth[a] == depth[b]);

	if(a != b) {
		int i = 0;
		while(par[a][i] != par[b][i]) i ++;

		for(i--; i >= 0; i--) {
			assert(a != b);
			assert(par[a][i + 1] == par[b][i + 1]);
			if(par[a][i] != par[b][i]) {
				res = max(res, dist[a][i]);
				res = max(res, dist[b][i]);
				a = par[a][i];
				b = par[b][i];
			} else {
				// do nothing
			}
		}
		assert(a != b);
		assert(par[a][0] == par[b][0]);
		res = max(res, dist[a][0]);
		res = max(res, dist[b][0]);
	}

	return res;
}

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N >> M >> K;
	for(int i = 1; i <= N; i++) {
		cc[i] = i;
	}

	for(int i = 0; i < M; i++) {
		cin >> E[i].second.first >> E[i].second.second >> E[i].first;
	}

	sort(E, E + M);

	for(int i = 0, j = 0; i < M; i++) {
		if(merge(E[i].second.first, E[i].second.second)) {
			assert(j < N - 1);
			assert(j <= i);
			E[j] = E[i];
			j++;

			adj[E[i].second.first].push_back(make_pair(E[i].first, E[i].second.second));
			adj[E[i].second.second].push_back(make_pair(E[i].first, E[i].second.first));
		}
	}
	M = N - 1;

	dfs(1);

	for(int i = 0; i < K; i++) {
		int a, b; cin >> a >> b;
		cout << query(a, b) << '\n';
	}
}
