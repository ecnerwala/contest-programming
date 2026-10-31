#include<bits/stdc++.h>
using namespace std;

const int MAXR = 600;
const int MAXC = 600;
int R, C;
char G[MAXR][MAXC];

const int MAXQ = int(2.1e5);
int Q;
int queryVerts[MAXQ];

const int MAXV = 2.1e5;
int V;
int height[MAXV];
int failure[MAXV];
int ch[MAXV][26];
int ans[MAXV];

pair<int, int> value[MAXV];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> R >> C >> Q;
	for (int i = 0; i < R; i++) {
		cin >> G[i];
	}
	V = 1;
	for (int q = 0; q < Q; q++) {
		string s; cin >> s;
		int cur = 0;
		for (char c : s) {
			if (!ch[cur][c - 'A']) {
				ch[cur][c - 'A'] = V++;
			}
			cur = ch[cur][c - 'A'];
		}
		queryVerts[q] = cur;
	}

	vector<int> bfs;
	bfs.reserve(V);
	bfs.push_back(0);
	height[0] = 0;
	failure[0] = -1;
	for (int z = 0; z < int(bfs.size()); z++) {
		int cur = bfs[z];
		for (int c = 0; c < 26; c++) {
			if (!ch[cur][c]) continue;
			bfs.push_back(ch[cur][c]);

			height[ch[cur][c]] = height[cur] + 1;

			int cnd = failure[cur];
			while (cnd != -1 && !ch[cnd][c]) {
				cnd = failure[cnd];
			}
			failure[ch[cur][c]] = cnd == -1 ? 0 : ch[cnd][c];
		}
	}
	assert(int(bfs.size()) == V);

	auto nextState = [&](int cur, int c) {
		while (true) {
			if (cur == -1) return 0;
			else if (ch[cur][c]) return ch[cur][c];
			else cur = failure[cur];
		}
	};

	for (int i = 0; i < R; i++) {
		for (int j = 0; j < C; j++) {
			int cur = 0;
			for (int k = 0; k < j; k++) {
				cur = nextState(cur, G[i][k] - 'A');
			}
			for (int k = i; k < R; k++) {
				cur = nextState(cur, G[k][j] - 'A');
				value[cur].first++;
			}
		}
	}

	for (int j = 0; j < C; j++) {
		int cur = 0;
		for (int i = 0; i < R; i++) {
			cur = nextState(cur, G[i][j] - 'A');
			value[cur].first -= i+1;
			value[cur].second += 1;
		}
	}

	for (int z = int(bfs.size()) - 1; z > 0; z--) {
		int cur = bfs[z];
		value[failure[cur]].first += value[cur].first;
		value[failure[cur]].second += value[cur].second;
	}

	for (int v = 0; v < V; v++) {
		ans[v] = value[v].first + height[v] * value[v].second;
	}

	for (int q = 0; q < Q; q++) {
		cout << ans[queryVerts[q]] << '\n';
	}

	return 0;
}
