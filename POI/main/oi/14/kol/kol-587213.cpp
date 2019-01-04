#include<bits/stdc++.h>

using namespace std;

typedef pair<int, int> pii;
#define A first
#define B second

const int MAXN = 6e3;
const int MAXM = 6e5;
const int MAXP = MAXN;

const int INF = 1e9;

int N;
vector<pii> adj[MAXN];

int M;

int L[MAXM];
int R[MAXM];
int D[MAXM];

int P;
int S[MAXP];

int edgelen[MAXN][MAXN];

int dist[MAXN][MAXN];
int prv[MAXN][MAXN]; // stores previous edge

int pdist[MAXP][MAXP];

bool used[MAXN][MAXN];

void dijkstra(int s) {
	for(int i = 0; i < N; i++) {
		dist[s][i] = INF;
	}
	priority_queue<pii, vector<pii>, greater<pii> > pq;
	dist[s][s] = 0;
	pq.push(pii(dist[s][s], s));
	while(!pq.empty()) {
		int d = pq.top().A;
		int p = pq.top().B;
		pq.pop();
		if(d != dist[s][p]) continue;
		for(int i = 0; i < adj[p].size(); i++) {
			int n = adj[p][i].A;
			int di = d + adj[p][i].B;
			if(di < dist[s][n]) {
				dist[s][n] = di;
				prv[s][n] = p;
				pq.push(pii(dist[s][n], n));
			}
		}
	}
}

int par[MAXP];
int getPar(int a) {
	return (par[a] == a) ? a : (par[a] = getPar(par[a]));
}

bool merge(int a, int b) {
	a = getPar(a), b = getPar(b);
	if(a == b) return false;
	par[a] = b;
	return true;
}

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N >> M;
	for(int i = 0; i < M; i++) {
		cin >> L[i] >> R[i] >> D[i];
		L[i] --, R[i] --;
	}
	cin >> P;
	for(int i = 0; i < P; i++) {
		cin >> S[i];
		S[i] --;
	}

	for(int i = 0; i < M; i++) {
		adj[L[i]].push_back(pii(R[i], D[i]));
		adj[R[i]].push_back(pii(L[i], D[i]));
		edgelen[L[i]][R[i]] = edgelen[R[i]][L[i]] = D[i];
	}

	vector<pair<int, pii> > edg;
	for(int p = 0; p < P; p++) {
		dijkstra(S[p]);
		for(int q = 0; q < P; q++) {
			pdist[p][q] = dist[S[p]][S[q]];
			edg.push_back(make_pair(dist[S[p]][S[q]], pii(S[p], S[q])));
		}
	}
	sort(edg.begin(), edg.end());
	for(int i = 0; i < N; i++) par[i] = i;
	for(int i = 0; i < edg.size(); i++) {
		int l = edg[i].B.A, r = edg[i].B.B;
		if(merge(l, r)) {
			//cerr << l << ' ' << r << '\n';
			while(r != l) {
				int p = prv[l][r];
				used[p][r] = used[r][p] = true;
				r = p;
			}
		}
	}

	int totCost = 0;
	int numCost = 0;
	for(int i = 0; i < N; i++) {
		for(int j = i + 1; j < N; j++) {
			if(used[i][j]) {
				numCost ++;
				totCost += edgelen[i][j];
				//cerr << i+1 << ' ' << j+1 << ' ' << edgelen[i][j] << '\n';
			}
		}
	}
	//cerr << "result is: \n";
	cout << totCost << ' ' << numCost << '\n';
	for(int i = 0; i < N; i++) {
		for(int j = i + 1; j < N; j++) {
			if(used[i][j]) {
				cout << i+1 << ' ' << j+1 << '\n';
			}
		}
	}
}
