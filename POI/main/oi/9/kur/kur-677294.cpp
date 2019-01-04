#include<bits/stdc++.h>

using namespace std;

const int MAXN = 2e3;
int N, NP;

const int MAXK = 1e4;
int K;
int P[MAXK][2];

const int MAXM = 500;
int M;
int Q[MAXM][2];
int R[MAXM];

const int MAXS = 3e3;
const int MAXV = MAXN * MAXS;

int B, S;

bool vis[MAXV];
vector<int> adj[MAXV];

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N >> NP;
	cin >> K;
	for(int i = 0; i < K; i++) {
		cin >> P[i][0] >> P[i][1];
		P[i][0] --, P[i][1] --;
	}

	cin >> M;
	for(int i = 0; i < M; i++) {
		cin >> Q[i][0] >> Q[i][1] >> R[i];
		Q[i][0] --, Q[i][1] --;
	}

	cin >> B >> S;
	B--;

	for(int i = 0; i < K; i++) {
		for(int s = 0; s <= S; s++) {
			adj[s * N + P[i][0]].push_back(s * N + P[i][1]);
		}
	}

	for(int i = 0; i < M; i++) {
		for(int s = R[i]; s <= S; s++) {
			adj[s * N + Q[i][0]].push_back((s - R[i]) * N + Q[i][1]);
		}
	}

	stack<int> q;
	q.push(S * N + B);

	while(!q.empty()) {
		int cur = q.top(); q.pop();
		if(vis[cur]) continue;
		vis[cur] = true;

		for(int nxt : adj[cur]) {
			q.push(nxt);
		}
	}
	for(int s = 0; s <= S; s++) {
		for(int i = 0; i < NP; i++) {
			if(vis[s * N + i]) {
				cout << s << '\n';
				return 0;
			}
		}
	}
	assert(false);
	return 0;
}
