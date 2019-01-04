/*
ID: he.andr1
LANG: C++
TASK: wormhole
*/

#include<bits/stdc++.h>
using namespace std;

const int MAXN = 20;
int N;
int X[MAXN], Y[MAXN];
int nxt[MAXN];
int p[MAXN];

bool check() {
	for (int st = 0; st < N; st++) {
		int cur = st;
		for (int t = 0; cur != -1 && t < N+1; t++) {
			cur = nxt[p[cur]];
		}
		if (cur != -1) {
			return true;
		}
	}
	return false;
}

int dfs() {
	int i = 0;
	while (i < N && p[i] != -1) i++;
	if (i == N) {
		return check();
	}
	int res = 0;
	for (int j = i+1; j < N; j++) {
		if (p[j] != -1) continue;
		p[i] = j, p[j] = i;
		res += dfs();
		p[i] = p[j] = -1;
	}
	return res;
}

int main() {
	if(fopen("wormhole.in", "r")) {
		freopen("wormhole.in", "r", stdin);
		freopen("wormhole.out", "w", stdout);
	}
	scanf("%d", &N);
	for (int i = 0; i < N; i++) {
		scanf("%d %d", &X[i], &Y[i]);
	}
	for (int i = 0; i < N; i++) {
		nxt[i] = -1;
		for (int j = 0; j < N; j++) {
			if (Y[j] == Y[i] && X[j] > X[i]) {
				if (nxt[i] == -1 || X[j] < X[nxt[i]]) nxt[i] = j;
			}
		}
	}
	memset(p, -1, sizeof(p));
	printf("%d\n", dfs());

	return 0;
}
