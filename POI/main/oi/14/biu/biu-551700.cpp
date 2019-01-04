#include<iostream>
#include<vector>
#include<list>
#include<cmath>
#include<queue>
#include<algorithm>
#include<cstring>

using namespace std;

const int MAXN = 2e5;

int N, M;
vector<int> adj[MAXN];
list<int> unVis;
vector<int> sz;


int par[MAXN];
int getPar(int a) {
	if(par[a] == -1) return a;
	return par[a] = getPar(par[a]);
}
bool merge(int a, int b) {
	a = getPar(a), b = getPar(b);
	if(a == b) return false;
	par[a] = b;
	return true;
}
int slow() {
	int res = 0;
	for(int i = 0; i < N; i++) {
		sort(adj[i].begin(), adj[i].end());
	}
	memset(par, -1, sizeof(par));
	for(int i = 0; i < N; i++) {
		int x = 0;
		for(int j = 0; j < N; j++) {
			if(x < adj[i].size() && adj[i][x] < j) x++;
			if(adj[i][x] == j) {
				continue;
			} else {
				//cerr << i << ' ' << j <<  '\n';
				merge(i, j);
			}
		}
	}
	for(int i = 0; i < N; i++) {
		if(par[i] == -1) res++;
	}
	return res;
}

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
		//cerr << unVis.front() << '\n';
		unVis.pop_front();
		while(!q.empty()) {
			int v = q.front(); q.pop();
			cnt++;
			int i = 0;
			for(__typeof(unVis.begin()) nx = unVis.begin(); nx != unVis.end(); ) {
				while(i < adj[v].size() && adj[v][i] < (*nx)) i++;
				if(i < adj[v].size() && adj[v][i] == (*nx)) {
					nx++;
					continue;
				} else {
					//cerr << *nx << ' ';
					q.push(*nx);
					nx = unVis.erase(nx);
				}
			}
		}
		//cerr << '\n';
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
	//cerr << slow() << '\n';
	sort(sz.begin(), sz.end());
	for(int i = 0; i < sz.size(); i++) {
		cout << sz[i];
		if(i + 1 == sz.size()) cout << '\n';
		else cout << ' ';
	}
	return 0;
}
