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

int query(ll v) {
	if(v >= S) return 1;

	for(int l = 0, r = 0; l < N; l++) {
		while(v >= A[r]) {
			v -= A[r];
			r++;
			if(r == N) r = 0;
		}

		nxt[l] = r;
		assert(r != l);

		v += A[l];
	}

	int st = 0;
	for (int i = 0; i < N; i++) st = nxt[st];

	int res = 0;
	int loops = 0;
	int cur = st;
	do {
		res++;
		int n = nxt[cur];
		loops += n < cur;
		cur = n;
	} while (cur != st);
	return (res + loops - 1) / loops;
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
