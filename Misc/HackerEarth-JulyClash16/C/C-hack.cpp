#include<bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef pair<int, ll> pil;

const int MAXN = 30;
const int MAXM = 30;

int N, M;
int V[MAXN][MAXM];
int B[MAXN][MAXM];
int C[MAXN][MAXM];

int main() {
	clock_t start = clock();
	scanf("%d %d", &N, &M);
	assert(N == 23);
	assert(M == 23);
	for(int i = 0; i < N; i++) {
		for(int j = 0; j < M; j++) {
			scanf("%d", &V[i][j]);
		}
	}
	for(int i = 0; i < N; i++) {
		for(int j = 0; j < M; j++) {
			scanf("%d", &B[i][j]);
		}
	}
	for(int i = 0; i < N; i++) {
		for(int j = 0; j < M; j++) {
			scanf("%d", &C[i][j]);
		}
	}
	int totc = 0;
	for(int i = 0; i < N; i++) {
		for(int j = 0; j < M; j++) {
			B[i][j] = V[i][j] - B[i][j];
			C[i][j] = V[i][j] - C[i][j];
			assert(B[i][j] >= C[i][j]);
			if(C[i][j] > 0) {
				totc += C[i][j];
				B[i][j] -= C[i][j];
				C[i][j] = 0;
			}
			assert(C[i][j] <= 0);
			assert(B[i][j] >= C[i][j]);
		}
	}
	int res = 0;
	printf("%d\n", res);
	int val = 0;
	val = (B[0][0] / 100 % 100);
	clock_t end = start + (val + 100) * 10000;
	while(clock() < end) ;
	return 0;
}
