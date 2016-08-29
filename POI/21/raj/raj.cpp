#include<bits/stdc++.h>
using namespace std;

const int MAXN = 1e6;
const int MAXM = 2e6;

int N, M;

int A[MAXM], B[MAXM];

vector<int> adj[MAXN];
int indeg[MAXN];

int topo[MAXN];

void toposort() {
	for(int i = 0; i < M; i++) {
		adj[A[i]].push_back(B[i]);
		indeg[B[i]] ++;
	}

	stack<int> q;
	for(int i = 0; i < N; i++) {
		if(indeg[i] == 0) {
			q.push(i);
		}
	}

	int K = 0;
	while(!q.empty()) {
		int v = q.top(); q.pop();
		topo[v] = K++;
		while(adj[v].size()) {
			int n = adj[v].back();
			adj[v].pop_back();
			assert(indeg[n]);
			indeg[n] --;
			if(indeg[n] == 0) {
				q.push(n);
			}
		}
	}

	assert(K == N);

	for(int i = 0; i < M; i++) {
		A[i] = topo[A[i]];
		B[i] = topo[B[i]];

		assert(A[i] < B[i]);
	}
}

void longestpath(int res[MAXN]) {
	for(int i = 0; i < M; i++) {
		adj[A[i]].push_back(B[i]);
	}

	for(int i = 0; i < N; i++) {
		res[i] = 0;
	}

	for(int i = 0; i < N; i++) {
		while(adj[i].size()) {
			int j = adj[i].back();
			adj[i].pop_back();
			res[j] = max(res[j], res[i] + 1);
		}
	}

}

const int L = 20;
const int S = 1 << L;

int seg[2 * S];

// exclusive, 0 indexed
void insert(int a, int b, int v) {
	a++, b--;
	a += S, b += S;
	while(a <= b) {
		if(a % 2 == 1) {
			seg[a] = max(seg[a], v);
			a++;
		}

		if(b % 2 == 0) {
			seg[b] = max(seg[b], v);
			b--;
		}

		a /= 2, b /= 2;
	}
}

void propogate() {
	for(int i = 2; i < 2 * S; i++) {
		seg[i] = max(seg[i], seg[i / 2]);
	}
}

int dp[2][MAXN];

int main() {
	scanf("%d %d", &N, &M);
	for(int i = 0; i < M; i++) {
		scanf("%d %d", &A[i], &B[i]);
		A[i] --, B[i] --;
	}

	toposort();

	longestpath(dp[0]);
	for(int i = 0; i < M; i++) {
		swap(A[i], B[i]);
		A[i] = N - 1 - A[i];
		B[i] = N - 1 - B[i];
	}
	longestpath(dp[1]);
	for(int i = 0; i < M; i++) {
		swap(A[i], B[i]);
		A[i] = N - 1 - A[i];
		B[i] = N - 1 - B[i];
	}
	reverse(dp[1], dp[1] + N);

	memset(seg, 0, sizeof(seg));

	for(int i = 0; i < M; i++) {
		insert(A[i], B[i], dp[0][A[i]] + dp[1][B[i]] + 1);
	}

	for(int i = 0; i < N; i++) {
		insert(i, N, dp[0][i]);
	}

	for(int i = 0; i < N; i++) {
		insert(-1, i, dp[1][i]);
	}

	propogate();

	//for(int i = 0; i < N; i++) cerr << seg[S + i] << ' '; cerr << '\n';
	
	int loc = 0, val = seg[S];
	for(int i = 0; i < N; i++) {
		if(seg[S + i] <= val) {
			val = seg[S + i];
			loc = i;
		}
	}

	for(int i = 0; i < N; i++) {
		if(topo[i] == loc) {
			loc = i;
			break;
		}
	}

	printf("%d %d\n", loc + 1, val);

	return 0;
}
