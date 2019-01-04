#include<iostream>
#include<vector>

using namespace std;

const int MAXN = 5e5;

int N;
int par[MAXN];
vector<int> children[MAXN];
int L;
int st[MAXN];
int en[MAXN];

void dfs(int cur) {
	st[cur] = L++;
	for(int i = 0; i < children[cur].size(); i++) {
		int nxt = children[cur][i];
		dfs(nxt);
	}
	en[cur] = L;
}

int bit[MAXN];

void insert(int i, int v) {
	while(i <= N) {
		bit[i] += v;
		i += i & (-i);
	}
}

int query(int i) {
	int res = 0;
	while(i > 0) {
		res += bit[i];
		i -= i & (-i);
	}
	return res;
}

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N;
	for(int i = 0; i < N - 1; i++) {
		int a, b; cin >> a >> b; a--, b--;
		par[b] = a;
		children[a].push_back(b);
	}
	dfs(0);
	for(int i = 1; i < N; i++) {
		insert(st[i], 1);
		insert(en[i], -1);
	}
	int M; cin >> M;
	for(int i = 0; i < N - 1 + M; i++) {
		char c; cin >> c;
		if(c == 'A') {
			int a, b; cin >> a >> b; a--, b--;
			insert(st[b], -1);
			insert(en[b], 1);
		} else if(c == 'W'){
			int a; cin >> a; a--;
			cout << query(st[a]) << '\n';
		}
	}
	return 0;
}
