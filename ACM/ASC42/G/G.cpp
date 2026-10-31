#include<bits/stdc++.h>
using namespace std;

const int MAXN = 1.1e5;
const int MAXM = 1.1e5;
int N, M;
int D[MAXN];
vector<int> inEdges[MAXN];
int outDeg[MAXN];

pair<int, int> evts[MAXN];

int ans[MAXN];

bool go(int t) {
	for (int i = 0; i < N; i++) {
		outDeg[i] = 0;
	}
	for (int i = 0; i < N; i++) {
		for (int v : inEdges[i]) outDeg[v]++;
	}

	for (int i = 0; i < N; i++) {
		evts[i].first = min(N-1, D[i] + t);
		evts[i].second = i;
	}

	sort(evts, evts + N);
	reverse(evts, evts + N);
	int ind = 0;
	stack<int> st;
	for (int d = N-1; d >= 0; d--) {
		while (ind < N && evts[ind].first >= d) {
			int i = evts[ind].second;
			if (outDeg[i] == 0) st.push(i);
			ind++;
		}
		if (st.empty()) {
			return false;
		}
		ans[d] = st.top(); st.pop();
		for (int v : inEdges[ans[d]]) {
			outDeg[v] --;
			if (D[v] + t >= d && outDeg[v] == 0) {
				st.push(v);
			}
		}
	}
	return true;
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	if (fopen("grand.in", "r")) {
		freopen("grand.in", "r", stdin);
		freopen("grand.out", "w", stdout);
	}

	while (true) {
		cin >> N;
		if (N == 0) exit(0);

		for (int i = 0; i < N; i++) {
			cin >> D[i]; D[i] --;
		}

		cin >> M;
		for (int e = 0; e < M; e++) {
			int u, v; cin >> u >> v; u--, v--;
			inEdges[v].push_back(u);
		}

		int mi = -1;
		int ma = N;
		while (ma - mi > 1) {
			int md = mi + (ma - mi) / 2;
			if (go(md)) {
				ma = md;
			} else {
				mi = md;
			}
		}
		go(ma);
		cout << ma << '\n';
		for (int i = 0; i < N; i++) {
			cout << ans[i]+1 << " \n"[i+1==N];
		}

		for (int i = 0; i < N; i++) {
			inEdges[i] = {};
		}
	}

	return 0;
}
