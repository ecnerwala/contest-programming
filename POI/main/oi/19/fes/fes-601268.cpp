#include<bits/stdc++.h>
using namespace std;

typedef pair<int, int> pii;
#define A first
#define B second

const int INF = 1e9;

const int MAXN = 700;
int N;

const int MAXM = 1.5e5;
int M1;
pii D[MAXM]; // A == B - 1
int M2;
pii G[MAXM]; // A <= B

pii par[MAXN]; // par, this - par
pii range[MAXN];

pii getpar(int a) {
	if(par[a].A == a) return par[a];
	pii res = getpar(par[a].A);
	res.B += par[a].B;
	return par[a] = res;
}

// b - a
bool merge(int a, int b, int d = 1) {
	//par[b] - par[a] = (par[b] - b) + (b - a) + (a - par[a])
	pii pa = getpar(a), pb = getpar(b);
	d = -pb.B + d + pa.B;

	a = pa.A;
	b = pb.A;

	if(a == b) {
		return d == 0;
	}

	par[b] = pii(a, d);

	return true;
}

int dist[MAXN][MAXN];
// j - i <= dist[i][j]

bool vis[MAXN];

int go() {

	for(int i = 0; i < N; i++) {
		par[i] = pii(i, 0);
		range[i] = pii(0, 0);
	}

	for(int i = 0; i < M1; i++) {
		if(!merge(D[i].A, D[i].B)) {
			return false;
		}
	}


	for(int i = 0; i < N; i++) {
		for(int j = 0; j < N; j++) {
			if(i == j) dist[i][j] = 0;
			else dist[i][j] = INF;
		}
	}

	for(int i = 0; i < N; i++) {
		pii p = getpar(i);
		//cerr << i << ' ' << p.A << ' ' << p.B << '\n';
		dist[p.A][i] = p.B;
		dist[i][p.A] = -p.B;
	}

	for(int i = 0; i < M2; i++) {
		int a = G[i].A, b = G[i].B;
		pii pa = getpar(a), pb = getpar(b);
		// par[b] - par[a] = -(b - par[b]) + (a - par[a]) + ((b - a) >= 0)
		// par[b] - par[a] >= d
		// par[a] - par[b] <= -d
		int d = -pb.B + pa.B;
		a = pa.A, b = pb.A;
		//cerr << a << ' ' << b << ' ' << d << '\n';
		dist[b][a] = min(dist[b][a], -d);
	}

	for(int k = 0; k < N; k++) {
		for(int i = 0; i < N; i++) {
			for(int j = 0; j < N; j++) {
				dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
			}
		}
	}

	for(int i = 0; i < N; i++) {
		if(dist[i][i] < 0) return false;

		vis[i] = false;
	}

	vector<pair<int, pii> > edges;
	for(int i = 0; i < N; i++) {
		for(int j = 0; j < N; j++) {
			if(dist[i][j] < INF && dist[j][i] < INF) {
				edges.push_back(make_pair(dist[i][j], pii(i, j)));
			}
		}
	}
	sort(edges.begin(), edges.end());

	int res = 0;

	for(int i = int(edges.size()) - 1; i >= 0; i--) {
		int d = edges[i].A;
		int a = edges[i].B.A, b = edges[i].B.B;
		assert(vis[a] == vis[b]);
		if(vis[a]) continue;
		vis[a] = true;
		for(int z = 0; z < N; z++) {
			if(dist[a][z] < INF && dist[z][a] < INF) vis[z] = true;
		}

		res += d + 1;
	}

	return res;
}

int main() {
	scanf("%d %d %d", &N, &M1, &M2);
	for(int i = 0; i < M1; i++) {
		scanf("%d %d", &D[i].A, &D[i].B);
		D[i].A --, D[i].B --;
	}
	for(int i = 0; i < M2; i++) {
		scanf("%d %d", &G[i].A, &G[i].B);
		G[i].A --, G[i].B --;
	}

	int res = go();
	if(!res) printf("NIE\n");
	else printf("%d\n", res);

	return 0;
}
