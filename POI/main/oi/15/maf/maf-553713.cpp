#include<iostream>
#include<queue>

using namespace std;

const int MAXN = 2e6;

int N;
int S[MAXN];
int deg[MAXN];

bool vis[MAXN];

bool alive[MAXN];

int minKills() {
	int res = 1;
	for(int i = 0; i < N; i++) deg[i] = 0, alive[i] = true;
	for(int i = 0; i < N; i++) deg[S[i]] ++;

	queue<int> q;
	for(int i = 0; i < N; i++) {
		if(deg[i] == 0) {
			q.push(i);
		}
	}

	int numKills = 0;

	while(!q.empty()) {
		int v = q.front(); q.pop();
		vis[v] = true;
		if(alive[v]) {
			if(alive[S[v]]) {
				alive[S[v]] = false;
				numKills ++;
				q.push(S[v]);
			}
		} else {
			if((--deg[S[v]]) == 0) q.push(S[v]);
		}
	}

	for(int i = 0; i < N; i++) {
		if(alive[i] && deg[i]) {
			int cnt = 1;
			deg[i] --;
			for(int cur = S[i]; cur != i; cur = S[cur]) {
				cnt++;
				deg[cur] --;
			}
			numKills += (cnt + 1) / 2;
		}
	}
	return numKills;
}

int maxKills() {
	int res = 1;
	for(int i = 0; i < N; i++) deg[i] = 0, vis[i] = false;
	for(int i = 0; i < N; i++) deg[S[i]] ++;

	int numKills = N;
	for(int i = 0; i < N; i++) {
		if(deg[i] == 0) {
			numKills --;
		}
		if(!vis[i] && deg[i] == 1) {
			bool good = i != S[i]; 
			vis[i] = true;
			int cur = S[i];
			while(cur != i) {
				if(deg[cur] != 1) {
					good = false;
					break;
				}
				if(vis[cur]) {
					good = false;
					break;
				}
				vis[cur] = true;
				cur = S[cur];
			}
			if(good) {
				numKills --;
			}
		}
	}
	return numKills;
}

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N;
	for(int i = 0; i < N; i++) cin >> S[i], S[i]--;

	cout << minKills() << ' ' << maxKills() << '\n';
	return 0;
}
