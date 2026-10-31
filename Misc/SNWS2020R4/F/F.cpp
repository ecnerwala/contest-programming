#include<bits/stdc++.h>
using namespace std;

const int MAXN = 1010;
const int MAXM = 1010;

const int MAXV = MAXN * MAXM;
int par[MAXV];
void reset() { memset(par, -1, sizeof(par)); }
int getPar(int a) { return par[a] < 0 ? a : (par[a] = getPar(par[a])); }
bool merge(int a, int b) {
	a = getPar(a), b = getPar(b);
	if (a == b) return false;
	if (par[a] > par[b]) swap(a,b);
	// par[a] is more negative
	par[a] += par[b];
	par[b] = a;
	return true;
}

int N, M;
char G[MAXN][MAXM];

bool isCity(char c) {
	switch (c) {
		case '#':
			return true;
		case '?':
		case '+':
		case '.':
		case 'R':
		case 'G':
		case 'B':
		case 'Y':
		case 'K':
			return false;
		default:
			assert(false);
	}
}

bool isField(char c) {
	switch (c) {
		case '?':
		case '+':
		case '#':
			return false;
		case '.':
		case 'R':
		case 'G':
		case 'B':
		case 'Y':
		case 'K':
			return true;
		default:
			assert(false);
	}
}

const int mx[4] = {-1, 0, 1, 0};
const int my[4] = {0, -1, 0, 1};

unordered_set<int> cities[MAXV];
int cnt[MAXV][26];

const string PLAYERS = "RYGBK";

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	reset();
	cin >> N >> M;
	for (int i = 0; i < N; i++) {
		cin >> G[i];
	}

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			if (i+1 < N) {
				if (isCity(G[i][j]) && isCity(G[i+1][j])) {
					merge(i*M+j, (i+1)*M+j);
				}
				if (isField(G[i][j]) && isField(G[i+1][j])) {
					merge(i*M+j, (i+1)*M+j);
				}
			}
			if (j+1 < M) {
				if (isCity(G[i][j]) && isCity(G[i][j+1])) {
					merge(i*M+j, i*M+j+1);
				}
				if (isField(G[i][j]) && isField(G[i][j+1])) {
					merge(i*M+j, i*M+j+1);
				}
			}
		}
	}

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			if (!isField(G[i][j])) continue;
			int p = getPar(i*M+j);
			if (G[i][j] != '.') {
				assert('A' <= G[i][j] && G[i][j] <= 'Z');
				cnt[p][G[i][j]-'A'] ++;
			}

			for (int d = 0; d < 4; d++) {
				int ni = i + mx[d];
				int nj = j + my[d];
				if (0 <= ni && ni < N && 0 <= nj && nj < M) {
					if (isCity(G[ni][nj])) {
						cities[p].insert(getPar(ni*M+nj));
					}
				}
			}
		}
	}

	int score[26];
	memset(score, 0, sizeof(score));
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			if (!isField(G[i][j])) continue;
			int v = i*M+j;
			if (getPar(v) != v) continue;
			int best = 0;
			for (char c : PLAYERS) {
				best = max(best, cnt[v][c-'A']);
			}
			if (best == 0) continue;
			for (char c : PLAYERS) {
				if (cnt[v][c-'A'] == best) {
					score[c-'A'] += int(cities[v].size()) * 3;
				}
			}
		}
	}
	int best = *max_element(score, score + 26);
	string ans;
	for (char c : PLAYERS) {
		if (score[c-'A'] == best) {
			ans += c;
			ans += ' ';
		}
	}
	assert(!ans.empty());
	ans.pop_back();
	cout << best << '\n';
	cout << ans << '\n';

	return 0;
}
