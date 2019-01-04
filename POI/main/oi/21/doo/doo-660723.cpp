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

ll B[MAXQ];

int nxt[MAXN];
int vis[MAXN];

int query(ll v) {
	if(v < M) return 0;
	if(v >= S) return 1;

	for(int l = 0, r = 0; l < N; l++) {
		while(v >= A[r % N]) {
			v -= A[r % N];
			r++;
		}
		assert(r > l);
		assert(r < l + N);

		nxt[l] = r - l;

		assert(nxt[l] > 0);
		assert(nxt[l] < N);

		v += A[l];
	}

	int res = N;

	memset(vis, 0, sizeof(vis));

	int cnt = 0;
	for(int i = 0; i < N; i++) {
		if(vis[i]) continue;

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

	assert(cnt == N);

	return res;
}

int main() {
	scanf("%d%d", &N, &Q);
	M = 0;
	for(int i = 0; i < N; i++) {
		scanf("%d", &A[i]);
		A[i + N] = A[i];
		S += A[i];
		M = max(M, A[i]);
	}

	for(int q = 0; q < Q; q++) {
		ll v;
		scanf("%lld", &v);
		int res = query(v);
		if(res) {
			printf("%d\n", res);
		} else {
			printf("NIE\n");
		}
	}
	return 0;
}
