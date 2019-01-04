#include<bits/stdc++.h>
using namespace std;

unsigned int my_rand() {
	static unsigned int seed = 123456789;
	return seed = (unsigned int)(1103515245 * seed + 12345);
}


template <typename T> struct heap {
	T v;
	heap<T>* c[2] = {nullptr, nullptr};
};

template <typename T> heap<T>* meld(heap<T>* l, heap<T>* r) {
	if (l == nullptr) return r;
	if (r == nullptr) return l;
	if (r->v < l->v) {
		swap(l, r);
	}
	bool d = my_rand() & 1;
	l->c[d] = meld(l->c[d], r);
	return l;
}

template <typename T> heap<T>* pop(heap<T>* h) {
	assert(h != nullptr);
	return meld(h->c[0], h->c[1]);
}

template <typename T> T top(heap<T>* h) {
	return h->v;
}

template <typename T> bool top2eq(heap<T>* h) {
	if (!h) return false;
	return (h->c[0] && h->c[0]->v == h->v) || (h->c[1] && h->c[1]->v == h->v);
}

template <typename T> heap<T>* pop2(heap<T>* h) {
	return pop(pop(h));
}

typedef pair<int, int> pii;
typedef heap<pii> hp;

const int MAXN = 1e5;
int N;
vector<int> adj[MAXN];

int A[MAXN-1][2];

const int MAXM = 1e5;
int M;
hp* endpoints[MAXN];
hp pool[MAXM * 2];

int depth[MAXN];
int res[MAXN];

void dfs(int cur, int prv = -1) {
	depth[cur] = (prv == -1 ? -1 : depth[prv]) + 1;
	for (int nxt : adj[cur]) {
		if (nxt == prv) continue;
		dfs(nxt, cur);
		endpoints[cur] = meld(endpoints[cur], endpoints[nxt]);
	}
	while(top2eq(endpoints[cur])) {
		endpoints[cur] = pop2(endpoints[cur]);
	}
	if (!endpoints[cur]) {
		res[cur] = -1;
	} else {
		res[cur] = endpoints[cur]->v.first;
	}
}

int main() {
	if(fopen("disrupt.in", "r")) {
		freopen("disrupt.in", "r", stdin);
		freopen("disrupt.out", "w", stdout);
	}

	scanf("%d %d", &N, &M);
	for (int i = 0; i < N-1; i++) {
		int u, v; scanf("%d %d", &u, &v); u--, v--;
		A[i][0] = u, A[i][1] = v;
		adj[u].push_back(v);
		adj[v].push_back(u);
	}
	for (int i = 0; i < M; i++) {
		int u, v;
		int r;
		scanf("%d %d %d", &u, &v, &r); u--, v--;
		pool[2 * i].v = pii(r, i);
		pool[2 * i + 1].v = pii(r, i);
		endpoints[u] = meld(endpoints[u], &pool[2 * i]);
		endpoints[v] = meld(endpoints[v], &pool[2 * i+1]);
	}

	dfs(0);

	for (int i = 0; i < N-1; i++) {
		if (depth[A[i][0]] > depth[A[i][1]]) {
			swap(A[i][1], A[i][0]);
		}
		assert(depth[A[i][1]] == depth[A[i][0]] + 1);
		printf("%d\n", res[A[i][1]]);
	}

	return 0;
}
