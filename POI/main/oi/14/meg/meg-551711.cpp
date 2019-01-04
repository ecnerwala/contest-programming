#include<iostream>
#include<algorithm>

using namespace std;

typedef pair<int, int> pii;

const int MAXN = 5e5;

int N;
int par[MAXN];
pii E[MAXN];
int L;
int st[MAXN];
int en[MAXN];

void dfs(int cur) {
	st[cur] = L++;
	for(int i = lower_bound(E, E + N - 1, pii(cur, 0)) - E; i < N - 1 && E[i].first == cur; i++) {
		dfs(E[i].second);
	}
	en[cur] = L;
}

int bit[MAXN];

inline void insert(int i, int v) {
	while(i <= N) {
		bit[i] += v;
		i += i & (-i);
	}
}

inline int query(int i) {
	int res = 0;
	while(i > 0) {
		res += bit[i];
		i -= i & (-i);
	}
	return res;
}

const int MAXM = 5e5;

int res[MAXM];

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N;
	for(int i = 0; i < N - 1; i++) {
		int a, b; cin >> a >> b; a--, b--;
		E[i] = pii(a, b);
		par[b] = a;
	}
	sort(E, E + N - 1);
	dfs(0);
	for(int i = 1; i < N; i++) {
		insert(st[i], 1);
		insert(en[i], -1);
	}
	int M; cin >> M;
	int C = 0;
	for(int i = 0; i < N - 1 + M; i++) {
		char c; cin >> c;
		if(c == 'A') {
			int a, b; cin >> a >> b; a--, b--;
			insert(st[b], -1);
			insert(en[b], 1);
		} else if(c == 'W'){
			int a; cin >> a; a--;
			res[C++] = query(st[a]);
		}
	}
	for(int i = 0; i < M; i++) cout << res[i] << '\n';
	return 0;
}
