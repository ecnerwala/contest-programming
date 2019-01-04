#include<bits/stdc++.h>
using namespace std;

int main() {
	int N;
	scanf("%d", &N);
	int res = N;
	for (int p = 2; p <= N; p++) {
		if (N % p == 0) {
			res /= p;
			res *= (p-1);
			while (N % p == 0) N /= p;
		}
	}
	printf("%d\n", res);

	return 0;
}
