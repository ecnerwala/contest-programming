#include<bits/stdc++.h>
using namespace std;

const int MAXN = int(1e6) + 200;
int N;

char S[MAXN];

int L;
char stk[MAXN];

int main() {
	scanf("%d", &N);
	scanf("%s", S);
	int res = 0;
	for (int i = 0; i < N; i++) {
		char c = S[i];
		if (c == ')' || c == ']' || c == '}') {
			if (L == 0) {
				printf("NIE\n");
				return 0;
			}
			L--;
			if (!(stk[L] == c-1 || stk[L] == c - 2)) {
				printf("NIE\n");
				return 0;
			}
		} else {
			stk[L++] = c;
		}
		res = max(res, L);
	}

	if (L) {
		printf("NIE\n");
	} else {
		printf("%d\n", res);
	}
	return 0;
}
