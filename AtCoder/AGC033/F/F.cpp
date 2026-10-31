#include<bits/stdc++.h>
using namespace std;

const int MAXN = 2000;
using bs = bitset<MAXN>;
int N;
vector<int> Tadj[MAXN];
bs onPath[MAXN][MAXN];

int M;
bs Gadj[MAXN];

void dfs(int root, int cur, int prv = -1) {
	onPath[root][cur][cur] = true;
	for (int nxt : Tadj[cur]) {
		if (nxt == prv) continue;
		onPath[root][nxt] = onPath[root][cur];
	}
	for (int nxt : Tadj[cur]) {
		if (nxt == prv) continue;
		dfs(root, nxt, cur);
		onPath[root][cur] |= onPath[root][nxt];
	}
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N >> M;
	for (int i = 0; i < N-1; i++) {
		int a, b; cin >> a >> b; a--, b--;
		//int a = i, b = i+1;
		Tadj[a].push_back(b);
		Tadj[b].push_back(a);
	}
	vector<pair<int, int>> edges;
	for (int i = 0; i < M; i++) {
		int a, b; cin >> a >> b; a--, b--;
		//int a = i%N, b = (i+1)%N;
		Gadj[a][b] = Gadj[b][a] = true;
		edges.emplace_back(a, b);
	}

	for (int i = 0; i < N; i++) {
		dfs(i, i);
	}
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			onPath[i][j] |= onPath[j][i];
		}
	}

	for (int i = 0; i < N; i++) {
		Gadj[i][i] = true;
	}

	for (int e = 0; e < int(edges.size()); e++) {
		int i, j; tie(i,j) = edges[e];
		assert(Gadj[i][j]);
		assert(Gadj[j][i]);
		bs cnds = onPath[i][j] & (Gadj[i] ^ Gadj[j]);
		for (int k = cnds._Find_first(); k < cnds.size(); k = cnds._Find_next(k)) {
			if (Gadj[j][k] == false) {
				Gadj[j][k] = Gadj[k][j] = true;
				edges.emplace_back(j, k);
			} else if (Gadj[i][k] == false) {
				Gadj[i][k] = Gadj[k][i] = true;
				edges.emplace_back(i, k);
			}
		}
	}

	cout << edges.size() << '\n';
	return 0;
}
