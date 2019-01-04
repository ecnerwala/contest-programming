#include<bits/stdc++.h>
using namespace std;

const int MAXN = 2e6;
const int MAXM = 3e6;
const int MAXK = MAXN;

int N, M, K;
int A[MAXM], B[MAXM];

int par[MAXN];
int rnk[MAXN];

int getpar(int a) {
	return (a == par[a]) ? a : (par[a] = getpar(par[a]));
}

bool merge(int a, int b) {
	a = getpar(a), b = getpar(b);
	if(a == b) return false;
	if(rnk[a] > rnk[b]) swap(a, b);
	if(rnk[a] == rnk[b]) rnk[b] ++;
	par[a] = b;
	return true;
}

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N >> M >> K;
	for(int i = 1; i <= M; i++) {
		cin >> A[i] >> B[i];
	}

	for(int i = 1; i <= N; i++) par[i] = i, rnk[i] = 1;

	vector<pair<int, int> > res;
	
	for(int i = 1; i <= M; i++) {
		int u = A[i], v = B[i];
		bool edg = merge(u, v);
		if(!edg && min(u, v) <= K) {
			res.push_back(make_pair(u, v));
		}
	}

	cout << res.size() << '\n';
	for(int i = 0; i < int(res.size()); i++) {
		cout << res[i].first << ' ' << res[i].second << '\n';
	}

	return 0;
}
