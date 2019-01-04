#include<bits/stdc++.h>

using namespace std;

const int MAXN = 3e3;
int N;
bool B[MAXN][MAXN];

bool V[MAXN];
int main() {
	scanf("%d", &N);
	for(int i = 0; i < N; i++) {
		for(int j = 0; j < N; j++) {
			char c; scanf(" %c", &c);
			B[i][j] = c == '1';
		}
	}

	int res = 0;
	for(int l = 0; l < N; l++) {
		memset(V, 0, sizeof(V));
		for(int r = l; r < N; r++) {
			int cnt = 0;
			for(int i = 0; i < N; i++) {
				if(B[r][i]) V[i] = true;
				if(V[i]) {
					cnt = 0;
				} else {
					res = max(res, cnt += r - l + 1);
				}
			}
		}
	}
	printf("%d\n", res);
	return 0;
}
