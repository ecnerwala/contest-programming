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
bool special[MAXN];

int dist[MAXN];
int prv[MAXN]; // stores previous edge

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
		special[S[i]] = true;
	}

	for(int i = 0; i < M; i++) {
		adj[L[i]].push_back(pii(R[i], D[i]));
		adj[R[i]].push_back(pii(L[i], D[i]));
	}

	for(int i = 0; i < N; i++) {
		dist[i] = INF;
	}

	priority_queue<pii, vector<pii>, greater<pii> > pq;

	int st = S[0];
	special[st] = false;
	dist[st] = 0;
	pq.push(pii(0, st));

	int cost = 0;
	vector<pii> res;

	while(!pq.empty()) {
		int d = pq.top().A;
		int p = pq.top().B;
		pq.pop();

		if(d != dist[p]) continue;

		cerr << d << ' ' << p << '\n';
		
		if(special[p]) {
			special[p] = false;
			cost += dist[p];
			while(dist[p]) {
				int n = prv[p];

				res.push_back(pii(n, p));

				dist[p] = 0;
				pq.push(pii(0, p));

				p = n;
			}
		} else {
			for(int i = 0; i < int(adj[p].size()); i++) {
				int n = adj[p][i].A;
				int di = d + adj[p][i].B;
				if(di < dist[n]) {
					dist[n] = di;
					prv[n] = p;
					pq.push(pii(dist[n], n));
				}
			}
		}
	}

	cout << cost << ' ' << res.size() << '\n';
	for(int i = 0; i < int(res.size()); i++) {
		cout << res[i].A+1 << ' ' << res[i].B+1 << '\n';
	}
	return 0;
}
