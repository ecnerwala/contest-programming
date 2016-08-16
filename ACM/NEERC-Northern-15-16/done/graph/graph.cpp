#include<bits/stdc++.h>
using namespace std;

typedef pair<int, int> pii;
#define A first
#define B second

const int MAXN = 2e5;
const int MAXM = 2e5;

int N, M, K;
pii edges[MAXM];

vector<int> adj[MAXN];
int indeg[MAXN];

int main() {
	if(fopen("graph.in", "r")) {
		freopen("graph.in", "r", stdin);
		freopen("graph.out", "w", stdout);
	}
	ios_base::sync_with_stdio(0);

	cin >> N >> M >> K;
	for(int i = 0; i < M; i++) {
		cin >> edges[i].A >> edges[i].B;
		adj[edges[i].A].push_back(edges[i].B);
		indeg[edges[i].B] ++;
	}
	sort(edges, edges + M);

	priority_queue<int> low;
	priority_queue<int, vector<int>, greater<int>> high;
	priority_queue<int> used;

	for(int i = 1; i <= N; i++) {
		if(indeg[i] == 0) high.push(i);
	}

	vector<int> ord;
	vector<pii> add;

	for(int i = 0; i < N; i++) {

		while(!high.empty() && int(low.size()) <= K) {
			low.push(high.top()); high.pop();
		}
	
		assert(high.empty() || int(low.size()) == K + 1);

		int v;

		if(int(low.size()) <= K && !used.empty() && (low.empty() || used.top() > low.top())) {
			// use the top used one
			v = used.top(); used.pop();
			add.push_back(pii(ord.back(), v));
		} else {
			v = low.top(); low.pop();
		}

		while(!low.empty()) {
			used.push(low.top()); low.pop(); K--;
		}

		ord.push_back(v);

		for(int n : adj[v]) {
			if(!(--indeg[n])) {
				high.push(n);
			}
		}
	}

	for(int i = 0; i < N; i++) {
		if(i) cout << ' ';
		cout << ord[i];
	}
	cout << '\n';

	sort(add.begin(), add.end());

	cout << add.size() << '\n';

	for(pii a : add) { cout << a.A << ' ' << a.B << '\n'; }
}
