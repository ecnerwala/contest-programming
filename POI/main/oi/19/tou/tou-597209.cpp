#include<bits/stdc++.h>
using namespace std;

typedef pair<int, int> pii;
#define A first
#define B second

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

	vector<pair<int, int> > bad;
	vector<pair<int, int> > res;
	
	for(int i = 1; i <= M; i++) {
		int u = A[i], v = B[i];
		if(min(u, v) <= K) {
			bad.push_back(make_pair(u, v));
		} else {
			merge(u, v);
		}
	}

	for(int i = 0; i < int(bad.size()); i++) {
		int u = bad[i].A, v = bad[i].B;
		if(!merge(u, v)) res.push_back(pii(u, v));
	}

	cout << res.size() << '\n';
	for(int i = 0; i < int(res.size()); i++) {
		cout << res[i].first << ' ' << res[i].second << '\n';
	}

	return 0;
}
