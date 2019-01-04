#include<bits/stdc++.h>
using namespace std;

const int INF = 1e9;

const int MAXN = 1e5 + 20;
const int MAXM = 1e5 + 20;
const int MAXT = MAXM;

const int L = 17;
const int S = 1 << L;
const int MAXS = 2 * S + 20;

int cnt[MAXS];
int maxv[MAXS];

void insert(int a, int b, int v, int i, int l, int r) {
	if(!(a < b)) cerr << i << '\n';
	assert(l <= a && a < b && b <= r);
	if(r - l == b - a) {
		cnt[i] += v;
		maxv[i] += v;
	} else {
		assert(r - l > 1);
		int m = (l + r) / 2;
		if(a < m) insert(a, min(b, m), v, 2 * i + 0, l, m);
		if(m < b) insert(max(a, m), b, v, 2 * i + 1, m, r);
		maxv[i] = max(maxv[2 * i], maxv[2 * i + 1]) + cnt[i];
	}
}

int N, M, T; // max time
int E[MAXM];
int P[MAXM];
int C[MAXM];

int mi[MAXN], ma[MAXN];

int nump[MAXT];
int nxt[MAXT];
int prv[MAXT];

int go() {
	T = M;

	for(int i = 1; i < 2 * S - 1; i++) {
		maxv[i] = -INF;
		cnt[i] = (i >= S) ? -INF : 0;
	}

	for(int i = 0; i <= T + 1; i++) {
		nump[i] = -1;
	}
	nump[0] = nump[T + 1] = 0;

	for(int i = 1; i <= N; i++) {
		mi[i] = ma[i] = -1;
	}

	for(int i = 0; i < M; i++) {
		C[i] ++;

		if(nump[E[i]] != -1) {
			if(nump[E[i]] != C[i]) {
				M = i;
				break;
			}
		} else {
			insert(E[i], E[i] + 1, INF - C[i], 1, 1, T + 1);
		}

		int p = P[i];
		if(ma[p] == -1) {
			mi[p] = ma[p] = E[i];
			insert(mi[p], ma[p] + 1, 1, 1, 1, T + 1);
		} else if(ma[p] < E[i]) {
			insert(ma[p] + 1, E[i] + 1, 1, 1, 1, T + 1);
			ma[p] = E[i];
		} else if(E[i] < mi[p]) {
			insert(E[i], mi[p], 1, 1, 1, T + 1);
			mi[p] = E[i];
		}
		if(maxv[1] > 0) {
			M = i;
			break;
		}

		if(nump[E[i]] == -1) {
			nump[E[i]] = C[i];
		} else {
			E[i] = -1;
		}
	}

	assert(nump[0] == 0);
	long long val = 0;
	for(int t = 1, p = 0; t <= T + 1; t++) {
		if(nump[t] == -1) continue;
		prv[t] = p;
		nxt[p] = t;
		val += abs(nump[t] - nump[p]);
	}
	//cerr << val << endl;
	for(; M > 0 && val > 2 * N; M--) {
		int i = M - 1;
		if(E[i] == -1) continue;
		int t = E[i];
		int p = prv[t];
		int n = nxt[t];
		val -= abs(nump[t] - nump[p]);
		val -= abs(nump[t] - nump[n]);
		val += abs(nump[p] - nump[n]);
		nxt[p] = n;
		prv[n] = p;
	}

	return M;
}

int main() {
	int Q;
	scanf("%d", &Q);
	for(int q = 0; q < Q; q++) {
		scanf("%d %d", &N, &M);
		for(int i = 0; i < M; i++) {
			scanf("%d %d %d", &E[i], &P[i], &C[i]);
		}
		int res = go();
		printf("%d\n", res);
		fflush(stdout);
	}
	return 0;
}
