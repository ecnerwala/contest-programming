#include<bits/stdc++.h>
using namespace std;

const int MAXN = 40;
const int MAXK = 16;
const int MAXS = int(1e6) + 200;
int N, K, S;

int A[MAXS];

int main() {
	scanf("%d %d %d", &N, &K, &S);
	A[0] = 0;
	for (int i = 1; i <= S; i++) {
		A[i] = K + 1;
	}
	for (int i = 0; i < N; i++) {
		int v; scanf("%d", &v);
		for (int a = S - v; a >= 0; a--) {
			A[a+v] = min(A[a+v], A[a] + 1);
		}
	}
	for (int i = S; i >= 0; i--) {
		if (A[i] <= K) {
			printf("%d\n", i);
			return 0;
		}
	}
	assert(false);
	return 0;
}
