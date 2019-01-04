#include<bits/stdc++.h>
using namespace std;

const int INF = 1e9;

const int MAXF = 200;
const int MAXV = 200;
int F, V;
int A[MAXF][MAXV];
bool take[MAXF][MAXV];
int res[MAXF];

int main() {
	scanf("%d %d", &F, &V);
	assert(F <= V);
	for (int i = 0; i <= F; i++) {
		A[i][0] = -INF;
	}
	for (int i = 0; i <= V; i++) {
		A[0][i] = 0;
	}
	for (int i = 1; i <= F; i++) {
		for (int j = 1; j <= V; j++) {
			int a;
			scanf("%d", &a);
			a += A[i-1][j-1];
			if (a > A[i][j-1]) {
				take[i][j] = true;
				A[i][j] = a;
			} else {
				take[i][j] = false;
				A[i][j] = A[i][j-1];
			}
		}
	}
	printf("%d\n", A[F][V]);
	for (int i = F, j = V; j > 0; j--) {
		if (take[i][j]) {
			res[i] = j;
			i--;
		}
	}
	for (int i = 1; i <= F; i++) {
		printf("%d%c", res[i], " \n"[i == F]);
	}

	return 0;
}
