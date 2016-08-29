#include<bits/stdc++.h>
using namespace std;

const int MAXN = 1e6 + 20;

int N;
int K;
char S[MAXN];
int L;
int val[MAXN];
int pref[MAXN];

int R;
int res[MAXN];

int main() {
	scanf("%d %d %s", &N, &K, S);
	L = 0;
	R = 0;
	pref[0] = 0;
	for(int i = 0; i < N; i++) {
		val[L] = i;
		pref[L + 1] = pref[L] + (S[i] == 'c');
		L++;
		if(L >= K + 1 && pref[L] - pref[L - (K + 1)] == 1) {
			for(int j = 0; j < K + 1; j++) {
				res[R++] = val[--L];
			}
		}
	}
	assert(R == N && L == 0);
	for(int i = R - 1; i >= 0; i--) {
		printf("%d%c", res[i]+1,((i % (K + 1) == 0) ? '\n' : ' '));
	}
	return 0;
}
