#include<iostream>
#include<vector>
#include<cstring>
#include<stack>
#include<cassert>

using namespace std;

const int MAXN = 1500;
const int MAXM = 3000;

int N, M;

int A[MAXM];
int B[MAXM];
int L[MAXM];
int R[MAXM];

vector<int> adj[MAXN];
bool vis[MAXN];
int dir[MAXM]; //stores the direction into which it points

int maxCost;

bool addFlow(int cur) {
	if(vis[cur]) return false;
	vis[cur] = true;
	for(__typeof(adj[cur].begin()) it = adj[cur].begin(); it != adj[cur].end(); it++) {
		int edg = *it;
		int cost;
		if(A[edg] == cur) {
			cost = R[edg];
		} else {
			cost = L[edg];
		}
		if(cost > maxCost) continue;
		if(dir[edg] == cur) {
			continue;
		} else if(dir[edg] == 0) {
			dir[edg] = cur;
			return true;
		} else {
			int nxt = dir[edg];
			dir[edg] = cur;
			if(addFlow(nxt)) {
				return true;
			}
			dir[edg] = nxt;
		}
	}
	return false;
}

bool isGood(int cost) {
	memset(dir, 0, sizeof(dir));
	maxCost = cost;
	for(int i = 1; i <= N; i++) {
		int d = adj[i].size() / 2;
		for(int j = 0; j < d; j++) {
			memset(vis, 0, sizeof(vis));
			if(!addFlow(i)) return false;
		}
	}
	return true;
}

vector<pair<int, int> > out[MAXN];

int S;
int path[MAXM];

void dfs(int cur) {
	while(!out[cur].empty()) {
		int nxt = out[cur].back().first, edg = out[cur].back().second;
		out[cur].pop_back();
		dfs(nxt);
		path[S++] = edg;
	}
}

int binarySearch() {
	int mi = 0;
	int ma = 2000;
	while(ma - mi > 1) {
		int md = (mi + ma) / 2;
		if(isGood(md)) {
			ma = md;
		} else {
			mi = md;
		}
	}
	isGood(ma);
	for(int i = 1; i <= M; i++) {
		int dest = dir[i];
		int src = A[i] + B[i] - dir[i];
		out[src].push_back(make_pair(dest, i));
	}
	S = 0;
	dfs(1);
	return ma;
}

bool goodDeg() {
	for(int i = 1; i <= N; i++) {
		if(adj[i].size() % 2 != 0) return false;
	}
	return true;
}

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N >> M;
	for(int i = 1; i <= M; i++) {
		cin >> A[i] >> B[i] >> L[i] >> R[i];
		adj[A[i]].push_back(i);
		adj[B[i]].push_back(i);
	}

	if(!goodDeg()) {
		cout << "NIE" << '\n';
		return 0;
	} else {
		cerr << isGood(504) << '\n';
		cerr << isGood(505) << '\n';
		int res = binarySearch();
		cout << res << '\n';

		while(S) {
			S--;
			cout << path[S];
			if(S == 0) cout << '\n';
			else cout << ' ';
		}
	}
}
