#include<bits/stdc++.h>
using namespace std;

const int INF = 1e9;

int S, T;

const int MAXN = 400;
int N;
struct switcher {
	bool C0;
	int R0;
	int T[2];

	int get(int t) const {
		t %= T[0] + T[1];
		return (R0 <= t && t < R0 + T[!C0]) ? !C0 : C0;
	}

	int next(int t) const {
		int f = t;
		t %= (T[0] + T[1]);
		f -= t;

		if (t < R0) f += R0;
		else if (t < R0 + T[!C0]) f += R0 + T[!C0];
		else f += R0 + T[!C0] + T[C0];

		return f;
	}
} C[MAXN];

vector<pair<int, int>> adj[MAXN];
int dist[MAXN];
int prv[MAXN];

const int MAXM = 20000;
int M;

int next_match(int t, const switcher &a, const switcher &b) {
	if (a.get(t) == b.get(t)) {
		return t;
	}
	int na = a.next(t);
	int nb = b.next(t);
	assert(a.get(na) == !a.get(t));
	assert(b.get(nb) == !b.get(t));
	if (na != nb) {
		return min(na, nb);
	}
	assert(na == nb);
	na = a.next(na);
	nb = b.next(nb);
	if (na != nb) {
		return min(na, nb);
	}
	assert(na == nb);
	na = a.next(na);
	nb = b.next(nb);
	if (na != nb) {
		return min(na, nb);
	}
	// Always cycle together
	return INF;
}

int main() {
	scanf(" %d %d", &S, &T);
	scanf(" %d %d", &N, &M);
	for (int i = 1; i <= N; i++) {
		char c;
		scanf(" %c %d %d %d", &c, &C[i].R0, &C[i].T[0], &C[i].T[1]);
		C[i].C0 = (c == 'P');
		assert("BP"[C[i].C0] == c);
	}
	for (int i = 1; i <= M; i++) {
		int u, v, d;
		scanf(" %d %d %d", &u, &v, &d);
		adj[u].emplace_back(v, d);
		adj[v].emplace_back(u, d);
	}

	for (int i = 1; i <= N; i++) {
		dist[i] = INF;
		prv[i] = -1;
	}

	priority_queue<pair<int, int>> pq;
	dist[S] = 0;
	pq.emplace(-dist[S], S);
	while (!pq.empty()) {
		int d = -pq.top().first;
		int cur = pq.top().second;
		pq.pop();
		assert(d >= dist[cur]);
		if (d != dist[cur]) continue;
		for (auto it : adj[cur]) {
			int nxt = it.first;
			int nd = next_match(d, C[cur], C[nxt]);
			if (nd == INF) continue;
			assert(nd >= d);
			nd += it.second;
			if (nd < dist[nxt]) {
				dist[nxt] = nd;
				prv[nxt] = cur;
				pq.emplace(-dist[nxt], nxt);
			}
		}
	}

	if (dist[T] == INF) {
		printf("0\n");
		return 0;
	}
	printf("%d\n", dist[T]);
	vector<int> path;
	for (int cur = T; cur != S; cur = prv[cur]) {
		path.push_back(cur);
	}
	reverse(path.begin(), path.end());
	printf("%d", S);
	for (int i : path) {
		printf(" %d", i);
	}
	printf("\n");

	return 0;
}
