#include<bits/stdc++.h>
using namespace std;

const int MAXN = 300;
const int MAXM = 300;
const int MAXA = MAXN * MAXM;

int N, M;
int A[MAXN][MAXM];
int R[MAXN][MAXM];
int C[MAXN][MAXM];

const int MAXR = MAXA;
const int MAXL = MAXA;
int prv[MAXR];
vector<int> nxt[MAXL];
bool vis[MAXL];

bool dfs(int a) {
	if(vis[a]) return false;
	vis[a] = true;
	for(int r : nxt[a]) {
		if(prv[r] == -1) {
			prv[r] = a;
			return true;
		} else {
			if(dfs(prv[r])) {
				prv[r] = a;
				return true;
			}
		}
	}
	return false;
}

int match() {
	int res = 0;
	memset(prv, -1, sizeof(prv));
	for(int i = 0; i < N * M; i++) {
		if(dfs(i)) {
			res++;
			memset(vis, 0, sizeof(vis));
		}
	}
	return res;
}

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N >> M;
	for(int i = 0; i < N; i++) {
		for(int j = 0; j < M; j++) {
			cin >> A[i][j];
		}
	}
	for(int i = 0; i < N; i++) {
		for(int j = 0; j < M; j++) {
			R[i][j] = C[i][j] = i * M + j;
			if(j > 0 && A[i][j - 1] != 2) R[i][j] = R[i][j - 1];
			if(i > 0 && A[i - 1][j] != 2) C[i][j] = C[i - 1][j];
		}
	}

	for(int i = 0; i < N; i++) {
		for(int j = 0; j < M; j++) {
			if(A[i][j] == 0) {
				nxt[R[i][j]].push_back(C[i][j]);
			}
		}
	}

	int cnt = match();
	cout << cnt << '\n';
	for(int r = 0; r < N * M; r++) {
		if(prv[r] != -1) {
			int l = prv[r];
			cout << l / M + 1 << ' ' << r % M + 1 << '\n';
		}
	}
}
