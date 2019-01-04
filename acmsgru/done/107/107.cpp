#include<bits/stdc++.h>
using namespace std;

int main() {
	int N;
	scanf("%d", &N);
	if (N < 9) {
		printf("0\n");
	} else if (N == 9) {
		printf("8\n");
	} else {
		printf("72");
		for (int i = 0; i < N-10; i++) {
			printf("0");
		}
		printf("\n");
	}

	return 0;
}
