#include<iostream>
#include<vector>
#include<list>
#include<cmath>
#include<queue>
#include<algorithm>

using namespace std;

const int MAXN = 2e5;

int N, M;
vector<int> adj[MAXN];
list<int> unVis;
vector<int> sz;

int go() {
	int res = 0;
	for(int i = 0; i < N; i++) {
		unVis.push_back(i);
		sort(adj[i].begin(), adj[i].end());
	}
	while(!unVis.empty()) {
		res++;
		int cnt = 0;
		queue<int> q;
		q.push(unVis.front());
		unVis.pop_front();
		while(!q.empty()) {
			int v = q.front(); q.pop();
			cnt++;
			int i = 0;
			for(__typeof(unVis.begin()) nx = unVis.begin(); nx != unVis.end(); nx++) {
				while(i < adj[v].size() && adj[v][i] < (*nx)) i++;
				if(i < adj[v].size() && adj[v][i] == (*nx)) {
					continue;
				} else {
					q.push(*nx);
					nx = unVis.erase(nx);
				}
			}
		}
		sz.push_back(cnt);
	}
	return res;
}

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N >> M;

	for(int i = 0; i < M; i++) {
		int a, b; cin >> a >> b; a--, b--;
		adj[b].push_back(a);
		adj[a].push_back(b);
	}

	cout << go() << '\n';
	sort(sz.begin(), sz.end());
	for(int i = 0; i < sz.size(); i++) {
		cout << sz[i];
		if(i + 1 == sz.size()) cout << '\n';
		else cout << ' ';
	}
	return 0;
}
