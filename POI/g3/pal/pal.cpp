#include<bits/stdc++.h>
using namespace std;
const int MAXL = int(1e3) + 200;
int L;
char N[MAXL];

char T[MAXL];

int V;
char C[MAXL * 5];

bool is_pal() {
	for (int i = 0; i < V - 1 - i; i++) {
		if (C[i] != C[V - 1 - i]) {
			return false;
		}
	}
	return true;
}

int main() {
	scanf("%s", N);
	L = int(strlen(N));
	assert(L >= 1);
	for (int i = 0; i < L; i++) N[i] -= '0';
	assert(N[0]);
	bool printed = false;
	for (int b = 2; b <= 10; b++) {
		memcpy(T, N, size_t(L));
		V = 0;
		while (true) {
			int has_more = false;
			int rem = 0;
			for (int i = 0; i < L; i++) {
				rem *= 10;
				rem += T[i];
				T[i] = char(rem / b);
				if (T[i]) has_more = true;
				rem %= b;
			}
			C[V++] = char('0' + rem);
			if (!has_more) {
				assert(rem != 0);
				break;
			}
		}
		if (is_pal()) {
			C[V] = '\0';
			printf("%d %s\n", b, C);
			printed = true;
		}
	}
	if (!printed) {
		printf("NIE\n");
	}

	return 0;
}
