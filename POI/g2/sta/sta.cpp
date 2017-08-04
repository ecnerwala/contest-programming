#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const int MAXN = 1010;
int N[3];
ll V[3][MAXN];

ll S;

int L;
ll A[MAXN * MAXN];

int main() {
	int M[3];
	scanf("%d %d %d", &M[0], &M[1], &M[2]);
	S = 0;
	for (int z = 0; z < 3; z++) {
		scanf("%d", &N[z]);
		for (int i = 0; i < N[z]; i++) {
			int v; scanf("%d", &v);
			V[z][i] = v;
			V[z][i] *= M[z];
		}
		sort(V[z], V[z] + N[z]);
		S += V[z][N[z]-1];
	}

	L = 0;
	for (int i = 0; i < N[1]; i++) {
		for (int j = 0; j < N[2]; j++) {
			A[L++] = V[1][i] + V[2][j];
		}
	}
	sort(A, A + L);
	int j = L;
	int res = 0;
	for (int i = 0; i < N[0]; i++) {
		while(j > 0 && A[j-1] + V[0][i] > S / 2) {
			j--;
		}
		res += L - j;
	}
	printf("%d\n", res);

	return 0;
}
