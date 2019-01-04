#include<bits/stdc++.h>
using namespace std;

int main() {
	int N;
	scanf("%d", &N);

	printf("%d", N);
	for (int i = 0; i < N; i++) {
		for (int j = N-i+1; j < N; j++) {
			printf(" %d", i*N+j+1);
		}
	}
	printf("\n");

	
	for (int z = N, t = N + (N % 2) + 1; z > 0; z--, t += 2) {
		printf("%d", t);
		for (int i = 0; i <= z; i++) {
			int j = z - i;
			if (0 <= i && i < N && 0 <= j && j < N) {
				printf(" %d", i*N+j+1);
			}
		}
		printf("\n");
	}

	return 0;
}
