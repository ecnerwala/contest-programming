#include<bits/stdc++.h>
using namespace std;

const int MAXN = int(1e6) + 200;
int N;
int W[MAXN];
int A[MAXN];
int B[MAXN];

int main() {
	scanf("%d", &N);
	for (int i = 0; i < N; i++) {
		scanf("%d", &W[i]);
	}
	A[0] = W[0];
	for (int i = 1; i < N; i++) {
		A[i] = max(A[i-1], W[i]);
	}
	B[N-1] = W[N-1];
	for (int i = N-2; i >= 0; i--) {
		B[i] = max(B[i+1], W[i]);
	}
	for (int i = 0; i < N; i++) {
		printf("%d %d\n", A[i], B[i]);
	}

	return 0;
}
