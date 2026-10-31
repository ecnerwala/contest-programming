#include<bits/stdc++.h>
using namespace std;

const int MAXN = 1.1e5;
int N, M;
vector<int> outEdges[MAXN];
vector<int> inEdges[MAXN];
int inDeg[MAXN];

bool isGood[MAXN];
int loc[MAXN];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N >> M;
	for (int i = 1; i <= N; i++) {
		outEdges[0].push_back(i);
		inEdges[i].push_back(0);
		inDeg[i] ++;

		outEdges[i].push_back(N+1);
		inEdges[N+1].push_back(i);
		inDeg[N+1] ++;
	}
	for (int e = 0; e < M; e++) {
		int u, v; cin >> u >> v;
		outEdges[u].push_back(v);
		inEdges[v].push_back(u);
		inDeg[v]++;
	}

	vector<int> q;
	q.push_back(0);

	for (int z = 0; z <= N+1; z++) {
		int cur = q[z];
		loc[cur] = z;
		for (int nxt : outEdges[cur]) {
			--inDeg[nxt];
			if (inDeg[nxt] == 0) {
				q.push_back(nxt);
			}
		}
	}



	for (int i = 1; i <= N; i++) {
		isGood[i] = true;
	}

	{
		int nxtCnd = 0;
		for (int z = 0; z <= N+1; z++) {
			int cur = q[z];
			if (z != nxtCnd) {
				isGood[cur] = false;
			}
			int nxtStep = N+1;
			for (int nxt : outEdges[cur]) {
				nxtStep = min(nxtStep, loc[nxt]);
			}
			nxtCnd = max(nxtCnd, nxtStep);
		}
	}
	{
		int nxtCnd = N+1;
		for (int z = N+1; z >= 0; z--) {
			int cur = q[z];
			if (z != nxtCnd) {
				isGood[cur] = false;
			}
			int nxtStep = 0;
			for (int nxt : inEdges[cur]) {
				nxtStep = max(nxtStep, loc[nxt]);
			}
			nxtCnd = min(nxtCnd, nxtStep);
		}
	}

	vector<int> ans;
	for (int i = 1; i <= N; i++) {
		if (isGood[i]) ans.push_back(i);
	}
	if (ans.empty()) {
		for (int i = 1; i <= N; i++) {
			ans.push_back(i);
		}
	}

	cout << ans.size() << '\n';
	for (int i = 0; i < int(ans.size()); i++) {
		cout << ans[i] << " \n"[i+1==int(ans.size())];
	}

	return 0;
}
