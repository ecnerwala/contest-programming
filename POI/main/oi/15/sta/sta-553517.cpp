#include<iostream>
#include<vector>

using namespace std;

typedef long long ll;

const int MAXN = 2e6;

int N;
vector<int> adj[MAXN];

//size, max val, best loc
pair<int, pair<ll, int> > dfs(int cur, int prev = -1) {
	int size = 1;
	int maxVal = 0;
	int bestLoc = cur;
	for(int i = 0; i < adj[cur].size(); i++) {
		int nxt = adj[cur][i];
		if(nxt == prev) continue;
		pair<int, pair<ll, int> > v = dfs(nxt, cur);
		size += v.first;
		if(v.second.first > maxVal) {
			maxVal = v.second.first;
			bestLoc = v.second.second;
		}
	}
	maxVal += N - 2 * size;
	return make_pair(size, make_pair(maxVal, bestLoc));
}


int main() {
	cin >> N;
	for(int i = 0; i < N - 1; i++) {
		int a, b; cin >> a >> b;
		adj[a].push_back(b);
		adj[b].push_back(a);
	}
	cout << dfs(1).second.second << '\n';
}
