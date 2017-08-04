#include<bits/stdc++.h>
using namespace std;

const int MAXR = 100;
const int MAXC = 100;
int R, C;

char G[MAXR][MAXC];
char G2[MAXR][MAXC];

const int MAXE = 55 * 55 * 2 * 2;
struct disjoint_set {
	int par[MAXE];
	void reset() {
		for (int i = 0; i < MAXE; i++) {
			par[i] = i;
		}
	}
	int getpar(int a) {
		return par[a] == a ? a : (par[a] = getpar(par[a]));
	}
	bool merge(int a, int b) {
		a = getpar(a), b = getpar(b);
		if (a == b) return false;
		par[a] = b;
		return true;
	}
} ds;

int V;
vector<int> adj[MAXE];

const int HORZ = 0;
const int VERT = 1;

int ctov(int x, int y, int dir) {
	return (x * (C + 2) + y) * 2 + dir;
}

bool dfs(int v) {
	int x = v / 2 / (C + 2);
	int y = v / 2 % (C + 2);
	int dir = v % 2;
	assert(G[x][y] == '?' || G[x][y] == '-' || G[x][y] == '|');
	if (G[x][y] != '?') {
		return (G[x][y] == '|') == (dir == VERT);
	}
	G[x][y] = (dir == VERT) ? '|' : '-';
	for (int i : adj[v]) {
		if (!dfs(i)) {
			return false;
		}
	}
	return true;
}

bool go() {
	V = (R + 2) * (C + 2) * 2;
	for (int i = 0; i < V; i++) {
		adj[i].clear();
	}
	ds.reset();

	G[0][0] = '?';
	for (int i = 1; i <= R; i++) {
		for (int j = 1; j <= C; j++) {
			if (G[i][j] == '-' || G[i][j] == '|') G[i][j] = '?';
		}
	}

	for (int i = 1; i <= R; i++) {
		for (int j = 1; j <= C; j++) {
			int up = ctov(i - 1, j, VERT);
			int down = ctov(i, j, VERT);
			int left = ctov(i, j - 1, HORZ);
			int right = ctov(i, j, HORZ);
			if (G[i][j] == '#') continue;
			else if (G[i][j] == '?') continue;
			else if (G[i][j] == '/') {
				ds.merge(up, left);
				ds.merge(down, right);
			} else if (G[i][j] == '\\'){
				ds.merge(up, right);
				ds.merge(down, left);
			} else if (G[i][j] == '.') {
				ds.merge(up, down);
				ds.merge(left, right);
			} else assert(false);
		}
	}
	unordered_multimap<int, int> mp;
	for (int i = 1; i <= R; i++) {
		for (int j = 1; j <= C; j++) {
			int up = ds.getpar(ctov(i - 1, j, VERT));
			int down = ds.getpar(ctov(i, j, VERT));
			int left = ds.getpar(ctov(i, j - 1, HORZ));
			int right = ds.getpar(ctov(i, j, HORZ));
			if (G[i][j] == '?') {
				if (up == left || up == right || down == left || down == right) {
					return false;
				}
				if (up == down && left == right) {
					return false;
				}
				mp.emplace(up, ctov(i, j, VERT));
				mp.emplace(down, ctov(i, j, VERT));
				mp.emplace(left, ctov(i, j, HORZ));
				mp.emplace(right, ctov(i, j, HORZ));
			}
		}
	}
	adj[false].push_back(true);
	for (auto &it : mp) {
		if (mp.count(it.first) != 1) {
			adj[it.second].push_back(false);
			adj[true].push_back(it.second ^ 1);
		}
	}
	for (int i = 1; i <= R; i++) {
		for (int j = 1; j <= C; j++) {
			int up = ds.getpar(ctov(i - 1, j, VERT));
			int down = ds.getpar(ctov(i, j, VERT));
			int left = ds.getpar(ctov(i, j - 1, HORZ));
			int right = ds.getpar(ctov(i, j, HORZ));
			if (G[i][j] == '.') {
				assert(up == down && left == right);
				if (mp.count(up) != 1 && mp.count(left) != 1) {
					return false;
				}
				int v = (mp.count(up) == 1) ? mp.find(up)->second : int(false);
				int h = (mp.count(left) == 1) ? mp.find(left)->second : int(false);
				adj[h ^ 1].push_back(v);
				adj[v ^ 1].push_back(h);
			}
		}
	}
	if (!dfs(true)) {
		return false;
	}
	for (int i = 1; i <= R; i++) {
		for (int j = 1; j <= C; j++) {
			if (G[i][j] == '?') {
				memcpy(G2, G, sizeof(G));
				if (!dfs(ctov(i, j, VERT))) {
					memcpy(G, G2, sizeof(G));
					if (!dfs(ctov(i, j, HORZ))) {
						return false;
					}
				}
			}
		}
	}
	return true;
}

int main() {
	ios_base::sync_with_stdio(0);
	int T; cin >> T;

	for(int case_num = 1; case_num <= T; case_num ++) {
		cin >> R >> C;
		for (int i = 1; i <= R; i++) {
			cin >> (G[i] + 1);
			G[i][0] = '#';
			assert(G[i][C + 1] == 0);
			G[i][C + 1] = '#';
		}
		for (int i = 0; i <= C + 1; i++) {
			G[0][i] = '#';
			G[R + 1][i] = '#';
		}

		bool res = go();
		cout << "Case #" << case_num << ": ";
		if (res) {
			cout << "POSSIBLE" << '\n';
			for (int i = 1; i <= R; i++) {
				G[i][C + 1] = 0;
				cout << (G[i] + 1) << '\n';
			}
		} else {
			cout << "IMPOSSIBLE" << '\n';
		}
	}

	return 0;
}
