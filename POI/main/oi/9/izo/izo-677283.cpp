#include<bits/stdc++.h>

using namespace std;

const int MAXN = 2e5;

typedef long long ll;
int N;
int A[MAXN];

int main() {
	scanf("%d", &N);
	for(int i = 0; i < N; i++) {
		scanf("%d", &A[i]);
	}
	sort(A, A + N);
	for(int i = 0; i < (N - i - 1); i++) {
		A[i] = A[N - i - 1];
	}

	long long res = 0;
	for(int i = 0; i < N; i++) {
		res += A[i];
	}
	printf("%lld\n", res);
}
