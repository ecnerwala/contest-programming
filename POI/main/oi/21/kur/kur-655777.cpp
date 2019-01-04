#include<bits/stdc++.h>
using namespace std;

const int MAXN = 1e6;
const int MAXM = 1e6;

int N, M;
int P[MAXN];

long long S[MAXN];

int getnum(long long v, int a, int b) {
	v *= N;
	return lower_bound(S, S + N, v + b) - lower_bound(S, S + N, v + a);
}

int query(int a, int b) {
	b++;
	int l = b - a;

	for(int i = 0; i <= 30; i++) {
		int v = P[a + rand() % l];
		int n = getnum(v, a, b);
		//cerr << a << ' ' << b << ' ' << v << ' ' << n << '\n';
		if(n * 2 > l) return v;
	}
	return -1;
}

int main() {
	srand(10);

	scanf("%d%d", &N, &M);
	for(int i = 0; i < N; i++) {
		scanf("%d", &P[i]);
		P[i] --;
		S[i] = ((long long)(P[i])) * N + i;
	}

	sort(S, S + N);

	for(int i = 0; i < M; i++) {
		int a, b; scanf("%d%d", &a, &b);
		a--, b--;
		printf("%d\n", query(a, b) + 1);
	}
	return 0;
}
