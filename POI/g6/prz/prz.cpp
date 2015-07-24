#include<bits/stdc++.h>
using namespace std;

const int MAXN = 1.2e6;
int N, M;
int V[MAXN];
int mx = 0;
int bs = 0;

void update(int a) {
	if(a == N) {
		bs = mx;
	} else {
		if(V[a] < bs) {
			V[a] = bs;
		}
		V[a] ++;
		mx = max(mx, V[a]);
	}
}

int main() {
	scanf("%d %d", &N, &M);
	for(int i = 0; i < M; i++) {
		int a; scanf("%d", &a);
		a--;
		update(a);
	}
	for(int i = 0; i < N; i++) {
		update(i);
		V[i] --;
	}
	printf("%d", V[0]);
	for(int i = 1; i < N; i++) {
		printf(" %d", V[i]);
	}
	printf("\n");
	return 0;
}
