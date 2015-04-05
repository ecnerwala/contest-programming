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
		while(v >= A[r]) {
			v -= A[r];
			r++;
			if(r == N) r = 0;
		}

		nxt[l] = r - l;
		if(nxt[l] < 0) nxt[l] += N;

		v += A[l];
	}

	int res = N;

	for(int i = 0; i < N; i++) {
		vis[i] = -1;
	}

	for(int i = 0; i < N; i++) {

		int cur = i;
		while(vis[cur] == -1) {
			vis[cur] = i;
			cur += nxt[cur];
			if(cur >= N) cur -= N;
		}

		if(vis[cur] == i) {
			// cur is part of a cycle
			int l = cur;
			int r = cur;

			int d = 0;
			do {
				while(r - l < N) {
					r += nxt[r % N];
					d++;
				}
				if(d < res) res = d;
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
		if(A[i] > M) M = A[i];
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
