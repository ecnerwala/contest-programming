#include<bits/stdc++.h>
using namespace std;

const int MAXN = 1e6 + 20;
const int MAXM = MAXN;
const int MAXC = MAXN;
int N;
int A[MAXN];

int M;
int C[MAXM];
int L[MAXM];

int cnt[MAXC];

int len;
int bad;
int res;

inline void ch(int c, int v) {
	if(cnt[c] == 0) {
		bad++;
	}
	cnt[c] += v;
	if(cnt[c] == 0) {
		bad--;
	}
}

int main() {
	scanf("%d %d", &N, &M);
	for(int i = 0; i < M; i++) {
		scanf("%d", &L[i]);
	}
	for(int i = 0; i < M; i++) {
		scanf("%d", &C[i]);
	}

	len = 0;
	bad = M;

	for(int i = 0; i < M; i++) {
		cnt[C[i]] = -L[i];
		len += L[i];
		if(len > N) {
			printf("0\n");
			return 0;
		}
	}

	for(int i = 0; i < N; i++) {
		scanf("%d", &A[i]);
		ch(A[i], 1);
		if(i >= len) ch(A[i - len], -1);
		if(!bad) {
			res++;
		}
	}

	printf("%d\n", res);

	return 0;
}
