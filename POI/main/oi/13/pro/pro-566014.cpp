#include<iostream>
#include<cstring>
#include<vector>
#include<queue>

using namespace std;

const int INF = 36501;

const int MAXN = 1e6 + 20;
const int MAXM = 1e6 + 20;

int N;
int M;
vector<int> adj[MAXN];

int inDeg[MAXN];
int numWays[MAXN];

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N >> M;
	N++;
	for(int i = 0; i < M; i++) {
		int a, b; cin >> a >> b;
		if(a == N) a = 0;
		if(b == N) b = 0;
		adj[b].push_back(a);
		inDeg[a]++;
	}
	queue<int> q;
	for(int i = 0; i < N; i++) {
		if(inDeg[i] == 0) q.push(i);
	}
	numWays[0] = 1;
	while(!q.empty()) {
		int cur = q.front(); q.pop();
		for(vector<int>::iterator it = adj[cur].begin(); it != adj[cur].end(); it++) {
			int nxt = *it;
			numWays[nxt] += numWays[cur];
			if(numWays[nxt] > INF) numWays[nxt] = INF;
			inDeg[nxt]--;
			if(inDeg[nxt] == 0) {
				q.push(nxt);
			}
		}
	}

	for(int i = 0; i < N; i++) {
		if(inDeg[i] != 0) numWays[i] = INF;
	}

	//for(int i = 0; i < N; i++) cerr << numWays[i] << ' '; cerr << '\n';

	int maxVal = 0;
	for(int i = 1; i < N; i++) {
		if(numWays[i] > maxVal) maxVal = numWays[i];
	}
	if(maxVal == INF) cout << "zawsze" << '\n';
	else cout << maxVal << '\n';

	int cntWays = 0;
	for(int i = 1; i < N; i++) {
		if(numWays[i] == maxVal) cntWays ++;
	}
	cout << cntWays << '\n';

	for(int i = 1; i < N; i++) {
		if(numWays[i] == maxVal) {
			cout << i << ((--cntWays) ? ' ' : '\n');
		}
	}
}
