#include<bits/stdc++.h>
using namespace std;

const int MAXN = 1.1e5;
const int MAXM = 1.1e5;
int N, M;
int A[MAXM], B[MAXM];
vector<int> adj[MAXN];
int deg[MAXN];

int par[MAXM];
int getPar(int a) {
	return par[a] == 0 ? a : (par[a] = getPar(par[a]));
}
bool merge(int a, int b) {
	a = getPar(a), b = getPar(b);
	if (a == b) return false;
	par[b] = a;
	return true;
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N >> M;
	for (int i = 1; i <= M; i++) {
		int a, b; cin >> a >> b;
		A[i] = a, B[i] = b;

		adj[a].push_back(b);
		adj[b].push_back(a);
		deg[a]++;
		deg[b]++;
	}

	if (N == 1) {
		cout << "No" << '\n';
		exit(0);
	}

	for (int i = 1; i <= N; i++) {
		if (deg[i] % 2 == 1) {
			cout << "No" << '\n';
			exit(0);
		}
		assert(deg[i] >= 2);
	}
	for (int i = 1; i <= N; i++) {
		if (deg[i] >= 6) {
			cout << "Yes" << '\n';
			exit(0);
		}
		assert(deg[i] == 2 || deg[i] == 4);
	}

	vector<int> fours;
	int n4 = 0;
	for (int i = 1; i <= N; i++) {
		if (deg[i] == 4) {
			fours.push_back(i);
			n4++;
		} else {
			assert(deg[i] == 2);
		}
	}
	if (n4 >= 3) {
		cout << "Yes" << '\n';
		exit(0);
	}
	if (n4 <= 1) {
		cout << "No" << '\n';
		exit(0);
	}

	assert(n4 == 2);

	int s = fours[0], t = fours[1];
	int numE = 0;
	for (int nei : adj[s]) {
		int prv = s;
		int cur = nei;
		while (cur != s && cur != t) {
			assert(adj[cur].size() == 2);
			int nxt = adj[cur][0] ^ adj[cur][1] ^ prv;
			prv = cur;
			cur = nxt;
		}
		if (cur == t) {
			numE ++;
		}
	}
	if (numE == 4) {
		cout << "No\n";
	} else {
		cout << "Yes\n";
	}

	return 0;
}
