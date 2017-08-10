#include "message.h"
#ifndef INPUT_HEADER
#define INPUT_HEADER "lemming.h"
#endif
#include INPUT_HEADER
#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const int NODES = NumberOfNodes();
const int ID = MyNodeId();

const int N = int(GetRows());
const int M = int(GetColumns());
const int S = N * M;

int get_next(int a) {
	int x = a / M, y = a % M;
	char c = GetDirection(x,y);
	switch(c) {
		case '^': x--; break;
		case '<': y--; break;
		case '>': y++; break;
		case 'v': x++; break;
		default: assert(false);
	}
	if (x < 0 || x >= N || y < 0 || y >= M) {
		return -1;
	}
	return x * M + y;
}

template <typename T> T split (T V, T i, T tot) {
	return V / tot * i + min(i, V % tot);
}

const bool is_small = N < NODES * 2;
const int A = is_small ? ((ID == 0) ? 0 : N) : split(N, ID, NODES);
const int B = is_small ? ((ID == 0) ? N : N) : split(N, ID+1, NODES);

const int MAXM = int(3.2e4);
const int MAXN = int(3.2e4);
const int MAXH = MAXN / 100;

int tot_cnt = 0;

int dest[MAXH][MAXM]; // dest is M+y for above, 2*M+y for below, and -1 for loop
int dfs(int a) {
	int x = a / M, y = a % M;
	assert(0 <= y && y < M);
	if (x < A) {
		assert(x == A-1);
		return M + y;
	} else if (x >= B) {
		assert(x == B);
		return 2 * M + y;
	} else assert(A <= x && x < B);
	if (dest[x-A][y]) {
		if (dest[x-A][y] == -2) {
			dest[x-A][y] = -1;
			tot_cnt ++;
		}
	} else {
		dest[x-A][y] = -2;
		int nxt = get_next(a);
		if (nxt == -1) {
			dest[x-A][y] = -1;
			tot_cnt ++;
		} else {
			int res = dfs(nxt);
			dest[x-A][y] = res;
		}
	}
	return dest[x-A][y];
}

const int MAXNODES = 120;
int nxt[MAXNODES * 2][MAXM];

int dfs2(int a) {
	assert(0 <= a && a < 2 * NODES * M);
	int x = a / M, y = a % M;
	if (nxt[x][y] == -1) return nxt[x][y];
	else if (nxt[x][y] == -2) {
		nxt[x][y] = -1;
		tot_cnt ++;
		return nxt[x][y];
	} else {
		int n = nxt[x][y];
		nxt[x][y] = -2;
		nxt[x][y] = dfs2(n);
		return nxt[x][y];
	}
}

int main() {
	assert(GetRows() * GetColumns() <= int(1e9));
	assert(M);
	memset(dest, 0, sizeof(dest));
	for (int i = A; i < B; i++) {
		for (int j = 0; j < M; j++) {
			dfs(i * M + j);
		}
	}
	if (is_small) {
		assert(N < NODES * 2);
		assert(N < 200);
		if (ID == 0) {
			assert(A == 0 && B == N);
			for (int i = 0; i < N; i++) {
				for (int j = 0; j < M; j++) {
					assert(dest[i][j] == -1);
				}
			}
			cout << tot_cnt << '\n';
		} else {
			assert(A == B);
		}
		return 0;
	}

	assert(A < B);
	assert(B - A >= 2);

	if (true) {
		PutInt(0, tot_cnt);
		for (int i = 0; i < M; i++) {
			PutInt(0, dest[0][i]);
		}
		assert(B-A-1 >= 1);
		for (int i = 0; i < M; i++) {
			PutInt(0, dest[B-A-1][i]);
		}
		Send(0);
	}
	if (ID == 0) {
		for (int src = 0; src < NODES; src++) {
			Receive(src);
			int cnt = GetInt(src);
			if (src) tot_cnt += cnt;
			for (int z = 2 * src; z <= 2 * src + 1; z++) {
				for (int j = 0; j < M; j++) {
					int val = GetInt(src);
					if (val == -1) {
						// pass
					} else if (val >= M && val < 2 * M) {
						assert(src > 0);
						val = (2*src-1) * M + (val % M);
					} else if (val >= 2 * M && val < 3 * M) {
						assert(src < NODES-1);
						val = (2*src+2) * M + (val % M);
					} else assert(false);
					nxt[z][j] = val;
				}
			}
		}

		for (int i = 0; i < 2 * NODES; i++) {
			for (int j = 0; j < M; j++) {
				dfs2(i * M + j);
			}
		}
		cout << tot_cnt << '\n';
	}

	return 0;
}
