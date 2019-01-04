#include <bits/stdc++.h>
using namespace std;

const int MAXD = int(1e9);

const int MAXN = 1.1e5;
const int MAXM = 2.1e5;
const int MAXV = MAXN * 2 + MAXM + 2;
int V;
int dp[MAXV];
int indeg[MAXV];
vector<pair<int, int>> out[MAXV];

void doDP() {
	stack<int> q;
	for (int i = 0; i < V; i++) {
		if (indeg[i] == 0) q.push(i);
	}
	while (!q.empty()) {
		int cur = q.top(); q.pop();
		//cerr << "Processing " << cur << ' ' << dp[cur] << '\n';
		for (auto it : out[cur]) {
			int nxt = it.first;
			int l = it.second;
			dp[nxt] = max(dp[nxt], dp[cur] + l);
			if (--indeg[nxt] == 0) {
				q.push(nxt);
			}
		}
	}
}

void addEdge(int a, int b, int e) {
	//cerr << "Adding " << a << ' ' << b << ' ' << e << '\n';
	assert(0 <= a && a < V);
	assert(0 <= b && b < V);
	out[a].emplace_back(b, e);
	indeg[b]++;
}

int N;
int vstart[30];

void addRange(int l, int r, int dest, int e) {
	r++; // half-open interval
	//cerr << "Adding " << "[" << l << ", " << r << ")" << ' ' << dest << ' ' << e << '\n';
	assert(0 <= l && l <= r && r <= N);
	for (int b = 0; l < r; b ++, l /= 2, r /= 2) {
		if (l & 1) {
			addEdge(vstart[b] + l, dest, e);
			l++;
		}
		if (r & 1) {
			r--;
			addEdge(vstart[b] + r, dest, e);
		}
	}
}

int S;

int M;

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N >> S >> M;

	V = 0;
	const int ZERO = (V++);
	const int INF = (V++);
	addEdge(ZERO, INF, MAXD);

	vstart[0] = V;
	V += N;
	for (int i = 0; i < N; i++) {
		addEdge(ZERO, vstart[0] + i, 1);
		addEdge(vstart[0] + i, INF, 0);
	}
	for (int b = 1; (N >> b) > 0; b++) {
		vstart[b] = V;
		V += (N >> b);
		for (int i = 0; i < (N >> b); i++) {
			addEdge(vstart[b-1] + 2 * i + 0, vstart[b] + i, 0);
			addEdge(vstart[b-1] + 2 * i + 1, vstart[b] + i, 0);
		}
	}

	for (int s = 0; s < S; s++) {
		int p, d; cin >> p >> d; p--;
		assert(1 <= d && d <= MAXD);
		addEdge(ZERO, vstart[0] + p, d);
		addEdge(vstart[0] + p, INF, MAXD - d);
	}

	for (int m = 0; m < M; m++) {
		int l, r, k; cin >> l >> r >> k;
		l--, r--; // inclusive range
		assert(k <= r - l);
		int mid = (V++);
		for (int i = 0; i < k; i++) {
			int x; cin >> x; x--;
			assert(x >= l);
			assert(x <= r);
			addRange(l, x-1, mid, 0);
			addEdge(mid, vstart[0] + x, 1);
			l = x + 1;
		}
		assert(l <= r + 1);
		addRange(l, r, mid, 0);
	}

	doDP();
	//for (int i = 0; i < V; i++) { cerr << dp[i] << ' '; } cerr << '\n';

	if (dp[INF] != MAXD) {
		cout << "NIE\n";
		return 0;
	}
	for (int i = 0; i < N; i++) {
		if (indeg[vstart[0] + i]) {
			cout << "NIE\n";
			return 0;
		}
	}
	cout << "TAK\n";
	for (int i = 0; i < N; i++) {
		cout << dp[vstart[0] + i] << " \n"[i == N-1];
	}

	return 0;
}
