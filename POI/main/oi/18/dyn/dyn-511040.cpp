#include<iostream>
#include<cstring>
#include<vector>
#include<algorithm>
#include<cassert>

using namespace std;

const int MAXN = 600000;
const int INF = 1 << 28;

template <class T> void setmax(T &a, T b) { if(b > a) a = b; }
template <class T> void setmin(T &a, T b) { if(b < a) a = b; }

int N, M;
vector<int> adj[MAXN];
bool D[MAXN];

int T;

int cnt;

//if > 0 is depth to the next charge
//else is - depth to the next dynamite
int dfs(int cur, int prv) {
	int minCharge = INF;
	int maxDyn = (D[cur] ? 0 : -1);
	for(int i = 0; i < adj[cur].size(); i ++) {
		int nxt = adj[cur][i];
		if(nxt == prv) continue;
		int v = dfs(nxt, cur);
		if(v > 0) setmin(minCharge, v);
		else setmax(maxDyn, -v);
	}

	if(maxDyn == T) {
		minCharge = 0;
		cnt++;
	}

	if(maxDyn + minCharge <= T) maxDyn = -1;

	if(maxDyn == -1) {
		return minCharge + 1;
	} else {
		return -(maxDyn + 1);
	}
}

bool isGood(int t) {
	T = t;
	cnt = 0;
	if(dfs(0, -1) < 0) cnt++;
	return cnt <= M;
}

int bsearch() {
	int min = -1;
	int max = N - 1;

	while(max - min > 1) {
		int mid = (min + max) / 2;
		if(isGood(mid)) max = mid;
		else min = mid;
	}

	return max;
}

int go() {
	return bsearch();
}

int main() {
	ios_base::sync_with_stdio(0);

	cin >> N >> M;
	for(int i = 0; i < N; i++) {
		cin >> D[i];
	}
	for(int i = 0; i < N - 1; i++) {
		int a, b; cin >> a >> b; a--, b--;
		adj[a].push_back(b);
		adj[b].push_back(a);
	}

	cout << go() << '\n';
}
