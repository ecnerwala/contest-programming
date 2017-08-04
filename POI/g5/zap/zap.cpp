#include<bits/stdc++.h>
using namespace std;

int main() {
	int N; scanf("%d", &N);
	int n1 = 0, n0 = 0;
	for (int i = 0; i < N; i++) {
		int v; scanf("%d", &v);
		n1 += (v != 1);
		n0 += (v != 0);
		n0 = min(n1, n0);
	}
	printf("%d\n", n0);

	return 0;
}
