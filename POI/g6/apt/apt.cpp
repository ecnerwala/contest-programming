#include<bits/stdc++.h>
using namespace std;

const int MAXN = 1.5e6;
int N;
int A[MAXN];

int main() {
	scanf("%d", &N);
	for(int i = 0; i < N; i++) {
		scanf("%d", &A[i]);
	}
	int v = A[N - 1];
	long long res = 0;
	for(int i = N - 1; i >= 0; i--) {
		v = min(v, A[i]);
		res += v;
	}
	printf("%lld\n", res);
	return 0;
}
