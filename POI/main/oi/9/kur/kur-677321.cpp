#include<bits/stdc++.h>

using namespace std;

const int MAXN = 1e3 + 20;
int N, NP;

const int MAXK = 1e4;
int K;

const int MAXM = 500;
int M;

const int MAXE = MAXK + MAXM;
int E;
int L[MAXE];
int R[MAXE];
int V[MAXE];

const int MAXS = 2e3 + 20;
const int MAXV = MAXN * MAXS;

int B, S;

bool vis[MAXV];
vector<int> adj[MAXN];

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N >> NP;
	cin >> K;
	for(int i = 0; i < K; i++) {
		cin >> L[E] >> R[E];
		V[E] = 0;
		E++;
	}

	cin >> M;
	for(int i = 0; i < M; i++) {
		cin >> L[E] >> R[E] >> V[E];
		E++;
	}

	for(int i = 0; i < E; i++) {
		L[i] --, R[i] --;
		adj[L[i]].push_back(i);
	}

	cin >> B >> S;
	B--;

	stack<int> q;
	vis[S * N + B] = true;
	q.push(S * N + B);

	while(!q.empty()) {
		int cur = q.top(); q.pop();

		int s = cur / N;
		int n = cur % N;

		for(int i = 0; i < int(adj[n].size()); i++) {
			int e = adj[n][i];
			if(s >= V[e]) {
				int nxt = (s - V[e]) * N + R[e];
				if(vis[nxt]) continue;
				vis[nxt] = true;
				q.push(nxt);
			}
		}
	}
	for(int s = 0; s <= S; s++) {
		for(int i = 0; i < NP; i++) {
			if(vis[s * N + i]) {
				cout << s << '\n';
				return 0;
			}
		}
	}
	assert(false);
	return 0;
}
