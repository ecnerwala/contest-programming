#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const int MAXN = 1e6 + 20;
const int MAXQ = 100;

int N;
int Q;

int A[MAXN];

ll S; // sum
int M; // max value of A

int nxt[MAXN];
int vis[MAXN];

int query(ll v) {
	if(v >= S) return 1;

	for(int l = 0, r = 0; l < N; l++) {
		while(v >= A[r % N]) {
			v -= A[r % N];
			r++;
		}

		nxt[l] = r - l;

		v += A[l];
	}

	int res = N;

	memset(vis, 0, sizeof(vis));

	int cnt = 0;
	for(int i = 0; i < N; i++) {

		int st = cnt;
		int cur = i;
		while(!vis[cur]) {
			cnt++;
			vis[cur] = cnt;
			cur += nxt[cur];
			if(cur >= N) cur -= N;
		}

		if(vis[cur] > st) {
			// cur is part of a cycle
			int l = cur;
			int r = cur;

			int d = 0;
			do {
				while(r - l < N) {
					r += nxt[r % N];
					d++;
				}
				res = min(res, d);
				d--;
			} while(l % N != cur);
		}
	}

	return res;
}

int main() {
	scanf("%d%d", &N, &Q);
	M = 0;
	for(int i = 0; i < N; i++) {
		scanf("%d", &A[i]);
		S += A[i];
		M = max(M, A[i]);
	}

	for(int q = 0; q < Q; q++) {
		ll v;
		scanf("%lld", &v);
		if(v < M) {
			printf("NIE\n");
		} else {
			printf("%d\n", query(v));
		}
	}
	return 0;
}
