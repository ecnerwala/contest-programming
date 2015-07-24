#include<bits/stdc++.h>
using namespace std;

typedef pair<int, int> pii;
#define A first
#define B second

const int MAXN = 1.1e6, MAXM = 4e6;
int N, M;

pair<int, int> E[MAXM];
int deg[MAXN];
bool used[MAXN];

int K;

bool dfs(int e, int v) {
	if(e == M) return true;
	if(used[E[e].A] || used[E[e].B]) {
		return dfs(e + 1, v);
	}
	if(!v) return false;
	used[E[e].A] = true;
	if(dfs(e + 1, v - 1)) return true;
	used[E[e].A] = false;
	used[E[e].B] = true;
	if(dfs(e + 1, v - 1)) return true;
	used[E[e].B] = false;
	return false;
}

int main() {
	scanf("%d %d %d", &N, &K, &M);
	K = N - K;
	assert(K > 0);
	for(int i = 0; i < M; i++) {
		scanf("%d %d", &E[i].A, &E[i].B);
		if(E[i].A > E[i].B) {
			swap(E[i].A, E[i].B);
		}
	}
	sort(E, E + M);
	M = int(unique(E, E + M) - E);
	for(int i = 0; i < M; i++) {
		deg[E[i].A] ++;
		deg[E[i].B] ++;
	}

	int L = K;
	for(int i = 1; i <= N; i++) {
		if(deg[i] > L) {
			used[i] = true;
			K--;
		}
	}

	if(K < 0) {
		printf("NIE\n");
		return 0;
	}

	E[M] = pii(-1, -1);

	M = 0;
	for(int i = 0; E[i] != pii(-1, -1); i++) {
		if(used[E[i].A] || used[E[i].B]) continue;
		else E[M++] = E[i];
	}

	if(M > K * L) {
		printf("NIE\n");
	}

	for(int v = 0; v <= K; v++) {
		if(dfs(0, v)) {
			int cnt = 0;
			for(int i = 1; i <= N; i++) {
				if(!used[i]) cnt++;
			}
			printf("%d\n", cnt);
			for(int i = 1; i <= N; i++) {
				if(!used[i]) {
					if(--cnt) {
						printf("%d ", i);
					} else {
						printf("%d\n", i);
					}
				}
			}
			return 0;
		}
	}

	printf("NIE\n");

	return 0;
}
