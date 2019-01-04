#include<bits/stdc++.h>
using namespace std;

const int MAXN = 2e2;
const int MAXM = 6000;

const int MAXB = 1500;

int N, M;
int C[MAXN];
vector<int> adj[MAXN];

int S, T;
int B;

int prv[MAXN][MAXB];

int main() {
	scanf("%d %d %d %d %d", &N, &M, &S, &T, &B);
	S--, T--;
	for(int i = 0; i < N; i++) {
		scanf("%d", &C[i]);
	}
	for(int i = 0; i < M; i++) {
		int u, v; scanf("%d %d", &u, &v);
		u--, v--;
		adj[u].push_back(v);
		adj[v].push_back(u);
	}

	B -= C[S]; 

	memset(prv, -1, sizeof(prv));
	stack<pair<int, int> > q;
	prv[T][B] = T;
	q.push(make_pair(T, B));

	while(prv[S][0] == -1) {
		assert(!q.empty());
		int cur = q.top().first;
		int val = q.top().second;
		q.pop();
		val -= C[cur];
		if(val < 0) continue;
		for(vector<int>::iterator it = adj[cur].begin(); it != adj[cur].end(); it++) {
			int nxt = *it;
			if(prv[nxt][val] != -1) continue;
			prv[nxt][val] = cur;
			q.push(make_pair(nxt, val));
		}
	}
	
	for(int cur = S, val = 0; cur != T; ) {
		cout << cur + 1 << ' ';
		cur = prv[cur][val];
		val += C[cur];
	}
	cout << T + 1 << '\n';
}
