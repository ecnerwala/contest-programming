#include<bits/stdc++.h>
using namespace std;

#define A first
#define B second
const int MAXN = 2048;
int N, M;
int K;
int T[MAXN][MAXN];
int P[MAXN][MAXN]; // prefix sums
pair<short, short> H[MAXN][MAXN];
bool vis[MAXN][MAXN];

int go() {
	for(int i = 0; i < N; i++) {
		P[i][0] = 0;
		for(int j = 0; j < M; j++) {
			P[i][j + 1] = P[i][j] + T[i][j];
		}
	}
	for(int i = 0; i <= M; i++) {
		H[i][M] = pair<short, short>(0, short(N));
		for(int j = M; j >= i; j--) {
			while(H[i][j].A < N && P[H[i][j].A][j] - P[H[i][j].A][i] <= K) {
				H[i][j].A++;
			}
			while(H[i][j].B > 0 && P[H[i][j].B - 1][j] - P[H[i][j].B - 1][i] <= K) {
				H[i][j].B--;
			}
			if(j > i) H[i][j - 1] = H[i][j];
		}
	}

	for(int j = 0; j < M; j++) {
		P[0][j] = 0;
		for(int i = 0; i < N; i++) {
			P[i + 1][j] = P[i][j] + T[i][j];
		}
	}

	memset(vis, 0, sizeof(vis));
	vis[0][M] = true;
	for(int l = M; l >= 0; l--) {
		for(int i = 0; i <= M - l; i++) {
			int j = i + l;
			if(vis[i][j]) {
				if(H[i][j].A == N) {
					return (M - l) + N;
				}
				assert(H[i][j].A < H[i][j].B);
				if(P[H[i][j].B][i] - P[H[i][j].A][i] <= K) {
					vis[i + 1][j] = true;
				}
				if(P[H[i][j].B][j - 1] - P[H[i][j].A][j - 1] <= K) {
					vis[i][j - 1] = true;
				}
			}
		}
	}
	return M + N;
}

int main() {
	scanf("%d %d %d", &K, &M, &N);
	for(int i = 0; i < N; i++) {
		for(int j = 0; j < M; j++) {
			scanf("%d", &T[i][j]);
		}
	}
	int res = go();
	for(int i = 0; i < max(N, M); i++) {
		for(int j = i; j < max(N, M); j++) {
			swap(T[i][j], T[j][i]);
		}
	}
	swap(N, M);
	res = min(res, go());
	printf("%d\n", res);
	return 0;
}
