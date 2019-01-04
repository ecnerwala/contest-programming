#include<bits/stdc++.h>
using namespace std;

const int MAXN = 2e5;
int N;

int A[2][MAXN];

#define V(i) (A[(i)%2][(i)/2])

int main() {
	int T; scanf("%d", &T);

	for(int case_num = 1; case_num <= T; case_num ++) {
		scanf("%d", &N);

		for(int i = 0; i < N; i++) {
			scanf("%d", &V(i));
		}

		sort(A[0], A[0] + (N+1)/2);
		sort(A[1], A[1] + N/2);

		int res = 0;
		while (res < N-1) {
			if (V(res) <= V(res+1)) {
				res++;
			} else {
				break;
			}
		}

		printf("Case #%d: ", case_num);
		if (res == N-1) {
			printf("OK");
		} else {
			printf("%d", res);
		}
		printf("\n");
	}

	return 0;
}
