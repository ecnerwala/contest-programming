#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const ll INF = 1e18;

const ll BAD = 1e9;

const int MAXN = 220;
const int MAXM = 220;
int N, M;
ll cost[MAXN][MAXM];

ll u[MAXN], v[MAXM];
int src[MAXM];

ll mind[MAXM];
int prv[MAXM];
bool vis[MAXM];

int main() {

	scanf("%d", &N);
	M = N;
	for(int i = 1; i <= N; i++) {
		int m, a, b, k; scanf("%d %d %d %d", &m, &a, &b, &k);
		for(int j = 1; j <= M; j++) {
			if(a <= j && j <= b) cost[i][j] = k * abs(j - m);
			else cost[i][j] = BAD;
		}
		for(int j = 0; j <= M; j++) mind[j] = INF, vis[j] = false, prv[j] = -1;
		src[0] = i;
		int j0 = 0;
		do {
			vis[j0] = true;
			int j1 = -1;
			ll delta = INF;
			for(int j = 1; j <= M; j++) {
				if(!vis[j]) {
					ll cnd = cost[src[j0]][j] - u[src[j0]] - v[j];
					if(cnd < mind[j]) {
						mind[j] = cnd, prv[j] = j0;
					}
					if(mind[j] < delta) {
						delta = mind[j], j1 = j;
					}
				}
			}
			for(int j = 0; j <= M; j++) {
				if(vis[j]) u[src[j]] += delta, v[j] -= delta;
				else mind[j] -= delta;
			}
			assert(j1 != -1);
			j0 = j1;
		} while(src[j0] != 0);
		do {
			int j1 = prv[j0];
			src[j0] = src[j1];
			j0 = j1;
		} while (j0);
	}
	//for(int i = 1; i <= M; i++) cerr << src[i] << '\n';
	ll res = -v[0];
	if(res >= BAD) {
		printf("NIE\n");
	} else {
		printf("%d\n", int(res));
	}
	return 0;
}
