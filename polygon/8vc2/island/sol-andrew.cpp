#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const int MAXN = 3e5;
int N;
int A[MAXN];
int B[MAXN];

int rt;
int fin;

int depth[MAXN];
int par[MAXN][22];
vector<int> ch[MAXN];

int top;
int bot[2];

inline void bad() { cout << -1 << '\n'; exit(0); }

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
	int cnt_diff = 0;
	for(int nxt : ch[cur]) {
		dfs_start(nxt, cur);
		cnt_diff += (A[nxt] != B[nxt]);
	}
	if(prv && B[cur] == 0) {
		swap(B[cur], B[prv]);
	}

	if(B[cur] != A[cur] && cnt_diff >= 2) bad();

	if(B[cur] == A[cur] && cnt_diff) {
		// this is top
		if(!top) top = cur;
		else bad();
	} else if(B[cur] != A[cur] && !cnt_diff){
		// this is a bottom
		if(!bot[0]) {
			bot[0] = cur;
		} else if(!bot[1]) {
			bot[1] = cur;
		} else bad();
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

	rt = 1;
	while(A[rt] != 0) rt++;
	fin = 1;
	while(B[fin] != 0) fin++;

	dfs_start(rt);
	assert(B[rt] == 0);

	if(!top) {
		cout << 0 << ' ' << depth[fin] - depth[rt] << '\n';
		exit(0);
	}

	if(!bot[1]) {
		bot[1] = top;
	}
	assert(bot[0] != bot[1]);
	assert(is_anc(top, bot[0]));
	assert(is_anc(top, bot[1]));

	ll res = depth[top] - depth[rt];

	while(!is_anc(fin, bot[0]) && !is_anc(fin, bot[1])) {
		res++;
		fin = par[fin][0];
	}

	if(is_anc(fin, bot[1])) {
		swap(bot[0], bot[1]);
	}
	assert(is_anc(fin, bot[0]));

	// now we need to just count # times around the cycle
	vector<int> arrA, arrB;
	for(int z = 0; z < 2; z++) {
		for(int i = bot[z]; i != top; i = par[i][0]) {
			arrA.push_back(A[i]);
			arrB.push_back(B[i]);
		}
		if(!z) {
			reverse(arrA.begin(), arrA.end());
			reverse(arrB.begin(), arrB.end());
		}
	}

	if(arrA.size() != arrB.size()) bad();

	int sz = int(arrA.size());
	int off = 0;
	while(off < sz && arrA[off] != arrB[0]) off++;
	if(off == sz) bad();
	for(int i = 0, j = off; i < sz; i++, j++) {
		if(j == sz) j = 0;
		if(arrB[i] != arrA[j]) bad();
	}

	ll cyc_time;
	if(is_anc(fin, top)) {
		res += depth[top] - depth[fin];
		cyc_time = ll(sz + 1) * off;
	} else {
		assert(is_anc(fin, bot[0]));
		assert(is_anc(top, fin));
		int ind = depth[fin] - depth[top];
		// now we just find where fin fits in
		cyc_time = ll(sz + 1) * off + ind;
	}
	assert(cyc_time <= ll(sz + 1) * sz);
	res += min(cyc_time, ll(sz + 1) * sz - cyc_time);

	cout << min(bot[0], bot[1]) << ' ' << max(bot[0], bot[1]) << ' ' << res << '\n';
	return 0;
}
