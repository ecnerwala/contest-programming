#include<bits/stdc++.h>
using namespace std;

const int MAXN = 2e4;
int N;
vector<int> adj[MAXN];

const int MAXM = MAXN * 2;
int M;
int A[MAXM], B[MAXM], L[MAXM], V[MAXM];
bool vis[MAXM];

int S;
int res[MAXM];
int pref[MAXM];

void dfs(int a) {
	while(!adj[a].empty()) {
		int e = adj[a].back();
		adj[a].pop_back();
		if(vis[e]) continue;
		vis[e] = true;
		assert(a == A[e] || a == B[e]);
		int b = A[e] ^ B[e] ^ a;
		dfs(b);
		res[S++] = e;
	}
}

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N;
	M = N * 2;
	int tot = 0;
	int best = 0;
	for(int i = 0; i < M; i++) {
		cin >> A[i] >> B[i] >> L[i] >> V[i];
		A[i] --, B[i] --;
		adj[A[i]].push_back(i);
		adj[B[i]].push_back(i);
		assert(L[i] % 2 == 0);
		tot += V[i] - L[i];
		if(V[i] - L[i] > V[best] - L[best]) {
			best = i;
		}
	}
	for(int i = 0; i < N; i++) {
		assert(int(adj[i].size()) == 4);
	}
	if(tot < 0) {
		// all nonpositive, right before the end, you mu
		if(V[best] - L[best] <= 0) {
			cout << "NIE\n";
			return 0;
		}
		while(tot < 0) {
			// maintain even degree
			for(int t = 0; t < 2; t++) {
				tot += V[best] - L[best];
				A[M] = A[best];
				B[M] = B[best];
				V[M] = V[best];
				L[M] = L[best];
				adj[A[M]].push_back(M);
				adj[B[M]].push_back(M);
				M++;
			}
		}
	}

	for(int i = 0; i < M; i++) {
		L[i] /= 2;
	}

	dfs(0);
	assert(S == M);
	res[M] = res[0];

	pref[0] = -L[res[0]];
	for(int i = 0; i < M; i++) {
		pref[i + 1] = pref[i] + V[res[i]] - L[res[i]] - L[res[i + 1]];
	}
	assert(pref[M] == pref[0] + tot);
	assert(pref[M] >= pref[0]);
	int first = 0;
	for(int i = 0; i < M; i++) {
		if(pref[i] < pref[first]) {
			first = i;
		}
	}
	int dir = 0;
	for(int i = 0; i <= first; i++) {
		assert(dir == A[res[i]] || dir == B[res[i]]);
		dir = A[res[i]] ^ B[res[i]] ^ dir;
	}
	cout << "TAK\n";
	cout << M << '\n';
	for(int i = 0; i < M; i++) {
		if(res[i] >= 2 * N) res[i] = best;
	}
	cout << res[first] + 1 << ' ' << dir + 1 << '\n';
	for(int i = 1; i < M; i++) {
		cout << res[(first + i) % M] + 1 << '\n';
	}
}
