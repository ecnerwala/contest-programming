#include<bits/stdc++.h>
using namespace std;

const int MAXN = 60;
const int MAXM = 60;
int N, M;
int V;
char G[MAXN][MAXM];
bool vis[MAXN][MAXM];

int par[MAXN * MAXM];
void reset() {
	memset(par, -1, sizeof(par));
}
int getPar(int a) {
	return par[a] == -1 ? a : (par[a] = getPar(par[a]));
}
bool merge(int a, int b) {
	a = getPar(a), b = getPar(b);
	if (a == b) return false;
	par[b] = a;
	return true;
}

void addEdge(int a, int b) {
	if (G[a / M][a % M] == '#') return;
	if (G[b / M][b % M] == '#') return;
	merge(a, b);
}

int go() {
	reset();
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			if (j) addEdge(i * M + j - 1, i * M + j);
			if (i) addEdge(i * M + j - M, i * M + j);
		}
	}
	int cur = -1;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			if (G[i][j] == '.') {
				int v = getPar(i * M + j);
				if (cur == -1) cur = v;
				if (cur != v) return -1;
			}
		}
	}
	assert(cur != -1);
	return cur;
}

bool toTry[MAXN * MAXM];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N >> M;
	V = N * M;
	for (int i = 0; i < N; i++) cin >> G[i];
	int c = go();
	if (c == -1) { cout << "Impossible" << '\n'; exit(0); }
	for (int i = 0; i < N * M; i++) {
		if (getPar(i) == c) toTry[i] = true;
	}
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			if (G[i][j] == '?' && toTry[i * M + j]) {
				G[i][j] = '#';
				if (go() != -1) {
					cout << "Ambiguous" << '\n';
					exit(0);
				}
				G[i][j] = '?';
			}
		}
	}
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			cout << "#."[toTry[i * M + j]];
		}
		cout << '\n';
	}

	return 0;
}
