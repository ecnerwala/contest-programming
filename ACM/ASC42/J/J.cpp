#include<bits/stdc++.h>
using namespace std;

const int MAXN = 1.1e5;
int N, M;
vector<int> inEdges[MAXN];
vector<int> outEdges[MAXN];

bool vis[MAXN];
int que[MAXN];
int bfsForwards(int st, int maxSz = N+1) {
	int Q = 0;
	vis[que[Q++] = st] = true;

	for (int i = 0; i < Q; i++) {
		for (int nxt : outEdges[que[i]]) {
			if (vis[nxt]) continue;
			vis[que[Q++] = nxt] = true;
			if (Q >= maxSz) goto is_done;
		}
	}

is_done:

	for (int i = 0; i < Q; i++) vis[que[i]] = false;
	return Q;
}

int sampCnt[MAXN];
void bfsBackwards(int st) {
	int Q = 0;
	vis[que[Q++] = st] = true;

	for (int i = 0; i < Q; i++) {
		for (int nxt : inEdges[que[i]]) {
			if (vis[nxt]) continue;
			vis[que[Q++] = nxt] = true;
		}
	}

	for (int i = 0; i < Q; i++) sampCnt[que[i]]++, vis[que[i]] = false;
}

int go() {
	const int B = 500;
	vector<int> bigs;
	pair<int, int> maxSeen(0, -1);
	for (int i = 0; i < N; i++) {
		if (!inEdges[i].empty()) continue;
		int len = bfsForwards(i, B);
		if (len >= B) bigs.push_back(i);
		else maxSeen = max(maxSeen, pair<int, int>(len, i));
	}
	if (bigs.empty()) {
		return maxSeen.second;
	}

	mt19937 mt(48);
	const int S = 500;
	for (int i = 0; i < N; i++) sampCnt[i] = 0;
	for (int i = 0; i < S; i++) {
		bfsBackwards(mt() % N);
	}
	maxSeen = pair<int, int>(0, -1);
	for (int i : bigs) {
		maxSeen = max(maxSeen, pair<int, int>(sampCnt[i], i));
	}
	return maxSeen.second;
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	if (fopen("journey.in", "r")) {
		freopen("journey.in", "r", stdin);
		freopen("journey.out", "w", stdout);
	}
	while (true) {
		cin >> N;
		if (N == 0) exit(0);
		cin >> M;
		for (int e = 0; e < M; e++) {
			int a, b; cin >> a >> b; a--, b--;
			outEdges[a].push_back(b);
			inEdges[b].push_back(a);
		}

		int v = go();
		cout << v+1 << ' ' << bfsForwards(v) << '\n';

		for (int i = 0; i < N; i++) {
			inEdges[i] = outEdges[i] = {};
		}
	}

	return 0;
}
