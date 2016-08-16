#include<bits/stdc++.h>
using namespace std;

const int MAXN = 3e5;
int N;
int A[MAXN];
int B[MAXN];

int rt;

int depth[MAXN];
int par[MAXN][22];
vector<int> ch[MAXN];

void dfs_start(int cur, int prv = 0) {
	par[cur][0] = prv;
	depth[cur] = depth[prv] + 1;
	for(int i = 0; par[cur][i]; i++) {
		par[cur][i + 1] = par[par[cur][i]][i];
	}
	for(int i = 0; i < int(ch[cur].size()); i++) {
		if(ch[cur][i] == prv) {
			ch[cur].erase(ch[cur].begin() + i);
			break;
		}
	}
	for(int nxt : ch[cur]) {
		dfs_start(nxt, cur);
		if(B[nxt] == 0) {
			swap(B[cur], B[nxt]);
		}
	}
}

int get_anc(int a, int d) {
	assert(d >= 0);
	for(int i = 0; d; i++, d /= 2) {
		if(d % 2) a = par[a][i];
	}
	return a;
}

// is a the ancestor of b?
bool is_anc(int a, int b) {
	return (depth[b] >= depth[a]) && (get_anc(b, depth[b] - depth[a]) == a);
}

int ds[MAXN];

int find(int a) { return ds[a] ? (ds[a] = find(ds[a])) : a; }
bool merge(int a, int b) {
	a = find(a), b = find(b);
	if(a == b) return false;
	ds[b] = a;
	return true;
}

int loc[MAXN][2];

int mindep[MAXN];
int st[MAXN][2];

bool is_cyc(const vector<int> &a, const vector<int> &b) {
	if(a.size() != b.size()) return false;
	int off = 0;
	while(off < int(b.size()) && b[off] != a[0]) off++;
	if(off == int(b.size())) return false;
	for(int i = 0, j = off; i < int(a.size()); i++, j++) {
		if(j == int(b.size())) j = 0;
		if(a[i] != b[j]) return false;
	}
	return true;
}

int go() {
	rt = 1;
	while(A[rt] != 0) rt++;
	dfs_start(rt);
	assert(B[rt] == 0);
	for(int i = 1; i <= N; i++) {
		loc[A[i]][0] = i;
		loc[B[i]][1] = i;
	}

	int cnt = N;
	for(int v = 1; v < N; v++) {
		int a = loc[v][0], b = loc[v][1];
		assert(a != rt);
		assert(par[a][0]);
		assert(b != rt);
		assert(par[b][0]);

		cnt -= merge(a, b);
		if(!is_anc(par[b][0], a)) cnt -= merge(par[b][0], b);
		if(!is_anc(par[a][0], b)) cnt -= merge(par[a][0], a);
	}

	for(int i = 1; i <= N; i++) {
		mindep[i] = N + 1;
		st[i][0] = st[i][1] = 0;
	}
	for(int i = 1; i <= N; i++) {
		int p = find(i);
		mindep[p] = min(mindep[p], depth[i]);
		if(is_anc(st[p][0], i)) st[p][0] = i;
		else if(is_anc(i, st[p][0])) ;
		else if(is_anc(st[p][1], i)) st[p][1] = i;
		else if(is_anc(i, st[p][1])) ;
		else return -1;
	}
	for(int i = 1; i <= N; i++) {
		int p = find(i);
		assert(st[p][0] != 0);
		// st[p][1] may be 0

		// i should be an ancestor of exactly one of the two
		if(is_anc(i, st[p][0]) == is_anc(i, st[p][1])) return -1;
	}

	for(int p = 1; p <= N; p++) {
		if(ds[p]) continue;
		vector<int> arrA, arrB;
		for(int z = 0; z < 2; z++) {
			for(int i = st[p][z]; depth[i] >= mindep[p]; i = par[i][0]) {
				if(find(i) != p) return -1;
				arrA.push_back(A[i]);
				arrB.push_back(B[i]);
			}
			reverse(arrA.begin(), arrA.end());
			reverse(arrB.begin(), arrB.end());
		}
		if(!is_cyc(arrA, arrB)) return -1;
		assert(arrA.size() == arrB.size());
		if(arrA.size() == 1) {
			cnt--;
		}
	}
	return cnt;
}

int main() {
	scanf("%d", &N);
	for(int i = 1; i <= N; i++) {
		scanf("%d", &A[i]);
	}
	for(int i = 1; i <= N; i++) {
		scanf("%d",&B[i]);
	}
	for(int i = 0; i < N - 1; i++) {
		int u, v; scanf("%d %d", &u, &v);
		ch[u].push_back(v);
		ch[v].push_back(u);
	}
	int res = go();
	printf("%d\n", res);
}
