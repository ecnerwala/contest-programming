#include<bits/stdc++.h>
using namespace std;

const int MAXN = 400;
int N;
int cap[MAXN][MAXN];
int flow[MAXN][MAXN];

bool dest[MAXN];

bool vis[MAXN];

bool dfs(int cur) {
	if (vis[cur]) return false;
	vis[cur] = true;

	if (dest[cur]) {
		dest[cur] = false;
		return true;
	}

	for (int nxt = 0; nxt < N; nxt++) {
		assert(cap[cur][nxt] >= 0);
		if (cap[cur][nxt] == 0) continue;
		if (dfs(nxt)) {
			cap[cur][nxt] --;
			cap[nxt][cur] ++;
			if (flow[nxt][cur]) flow[nxt][cur] --;
			else flow[cur][nxt] ++;
			return true;
		}
	}
	return false;
}

bool findflow(int source) {
	memset(vis, 0, sizeof(vis));
	return dfs(source);
}

const int MAXK = MAXN;
int K;
int A[MAXK];
int B[MAXK];

unordered_map<int, int> adj[MAXN];

bool has[MAXN];

int cntgood;

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N >> K;
	for (int i = 0; i < K; i++) {
		cin >> A[i]; A[i] --;
	}
	for (int i = 0; i < K; i++) {
		cin >> B[i]; B[i] --;
	}
	for (int i = 0; i < N; i++) {
		string s; cin >> s;
		for (int j = 0; j < N; j++) {
			char c = s[j];
			cap[i][j] = (c - '0') ? 2*N+1 : 0;
		}
	}

	for (int i = 0; i < K; i++) {
		dest[B[i]] = true;
	}

	for (int i = 0; i < K; i++) {
		if (!findflow(A[i])) {
			cout << "NO" << '\n';
			return 0;
		}
	}

	for (int i = 0; i < N; i++) {
		assert(!dest[i]);
	}
	for (int i = 0; i < K; i++) {
		dest[B[i]] = true;
	}

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			for (int k = 0; k < flow[i][j]; k++) {
				adj[i][j] ++;
			}
		}
	}

	cntgood = 0;
	for (int i = 0; i < K; i++) {
		has[A[i]] = true;
		cntgood += dest[A[i]];
	}

	vector<pair<int, int>> moves;

	while (cntgood < K) {
		for (int i = 0; i < K; i++) {
			const int cur = A[i];
			assert(has[cur]);
			for (auto it : adj[cur]) {
				assert(it.second > 0);
				const int nxt = it.first;
				if (!has[nxt]) {
					moves.emplace_back(cur, nxt);
					adj[cur][nxt] --;
					if (adj[cur][nxt] == 0) {
						adj[cur].erase(nxt);
					}

					has[A[i]] = false;
					cntgood -= dest[A[i]];

					A[i] = nxt;

					has[A[i]] = true;
					cntgood += dest[A[i]];

					goto foundone;
				}
			}
		}
		assert(false);
foundone:;
	}
	assert(cntgood <= K);
	assert(cntgood == K);

	cout << "YES" << '\n';
	cout << moves.size() << '\n';
	for (auto it : moves) {
		cout << it.first+1 << ' ' << it.second+1 << '\n';
	}

	return 0;
}
