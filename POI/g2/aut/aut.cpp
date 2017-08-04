#include<bits/stdc++.h>
using namespace std;

typedef pair<int, int> pii;
#define A first
#define B second

bool can_turn(pii v1, pii v2) {
	return (v1.A * v2.A + v1.B * v2.B) >= 0;
}

const int MAXN = 60;
const int MAXM = 510;
const int MAXP = 110;

int N, M, P;
pii V[MAXN];
struct edge {
	pii d;
	int a, b;
	int t;
} E[MAXM];

const int INF = int(1e9) + 200;
int adj[MAXM][MAXM];
bool vis[MAXM][MAXM];

int R[MAXP];

int main() {
	scanf("%d %d %d", &N, &M, &P);
	for (int i = 0; i < N; i++) scanf("%d %d", &V[i].A, &V[i].B);
	//for (int i = 0; i < N; i++) cerr << V[i].A << ' ' << V[i].B << '\n';
	for (int i = 0; i < M; i++) {
		int a, b, t;
		scanf("%d %d %d", &a, &b, &t);
		a--, b--;
		E[i].a = a, E[i].b = b;
		E[i].t = t;
		E[i].d = pii(V[b].A - V[a].A, V[b].B - V[a].B);
		//cerr << E[i].a << ' ' << E[i].b << ' ' << E[i].t << ' ' << E[i].d.A << ' ' << E[i].d.B << '\n';
	}
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < M; j++) {
			if (E[i].b == E[j].a && can_turn(E[i].d, E[j].d)) {
				adj[i][j] = E[i].t + E[j].t;
			} else {
				adj[i][j] = INF;
			}
		}
	}
	//for (int i = 0; i < M; i++) { for (int j = 0; j < M; j++) { cerr << adj[i][j] << ' '; } cerr << '\n'; }
	for (int k = 0; k < M; k++) {
		for (int i = 0; i < M; i++) {
			if (adj[i][k] == INF) continue;
			for (int j = 0; j < M; j++) {
				adj[i][j] = min(adj[i][j], adj[i][k] + adj[k][j]);
			}
		}
	}
	for (int i = 0; i < P; i++) {
		scanf("%d", &R[i]);
		R[i]--;
	}
	for (int i = 0; i+1 < P; i++) {
		R[i] = adj[R[i]][R[i+1]];
		if (R[i] == INF) {
			printf("NIE\n");
			return 0;
		}
	}
	for (int i = 0; i+1 < P; i++) {
		if (i) R[i] += R[i-1];
		printf("%d\n", R[i]);
	}
	return 0;
}
