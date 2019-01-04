#include<bits/stdc++.h>

using namespace std;

typedef pair<int, int> pii;
#define A first
#define B second

const int MAXN = 6e4;
const int MAXM = 3e5;

int N, M;

pii E[MAXM];

int prv[MAXM];
int nxt[MAXM];

vector<pii> adj[MAXN]; //stores pair of dest, start edge index

int inDeg[MAXN];
int outDeg[MAXN];

stack<int> res;

void dfs(int a) {
	while(!adj[a].empty()) {
		int b = adj[a].back().first;
		int e = adj[a].back().second;
		adj[a].pop_back();
		dfs(b);
		res.push(e);
	}
}

int main() {
	ios_base::sync_with_stdio(0);
	scanf("%d%d", &N, &M);
	for(int i = 0; i < M; i++) {
		int a, b;
		scanf("%d%d", &a, &b);
		a--, b--;
		E[i] = pii(a, b);
	}

	sort(E, E + M);

	memset(prv, -1, sizeof(prv));
	memset(nxt, -1, sizeof(nxt));

	int T;
	cin >> T;
	for(int i = 0; i < T; i++) {
		int K; scanf("%d", &K);
		int l; scanf("%d", &l); l--;
		int p = -1; // prv edge
		for(int i = 1; i < K; i++) {
			int r; scanf("%d", &r); r--;
			int e = lower_bound(E, E + M, pii(l, r)) - E;
			if(E[e] != pii(l, r)) {
				printf("NIE\n");
				return 0;
			}

			if(p != -1) {
				if(nxt[p] == e && prv[e] == p) {
					//already done
				} else if(nxt[p] == -1 && prv[e] == -1) {
					nxt[p] = e;
					prv[e] = p;
				} else {
					printf("NIE\n");
					return 0;
				}
			}

			l = r;
			p = e;
		}
	}

	for(int i = 0; i < M; i++) {
		if(prv[i] == -1) {
			int j;
			for(j = i; nxt[j] != -1; j = nxt[j]);
			int st = E[i].A;
			int en = E[j].B;
			adj[st].push_back(pii(en, i));
			inDeg[en] ++, outDeg[st]++;
		}
	}

	for(int i = 0; i < N; i++) {
		if(inDeg[i] != outDeg[i]) {
			printf("NIE\n");
			return 0;
		}
	}

	if(outDeg[0] == 0) {
		printf("NIE\n");
		return 0;
	}

	dfs(0);
	printf("TAK\n1\n");
	while(!res.empty()) {
		int i = res.top();
		res.pop();
		while(i != -1) {
			printf("%d\n", E[i].B + 1);
			i = nxt[i];
		}
	}
}
