#include<bits/stdc++.h>
using namespace std;

const int MAXN = 3e5;
int N;
int H[MAXN][2];
bool hit[MAXN][2];
const int MAXM = 5e5;
int M;
int S[MAXM];
int D[MAXM];

typedef pair<int, int> pii;
#define A first
#define B second

pii par[MAXN];
pii getpar(int a) {
	if(par[a].A == a) return par[a];
	pii res = getpar(par[a].A);
	res.B = min(par[a].B, res.B);
	return par[a] = res;
}
void merge(int a, int b, int t) {
	a = getpar(a).A;
	b = getpar(b).A;
	if(a == b) return;
	if(a > b) swap(a, b);
	assert(a < b);
	par[b] = pii(a, t);
}

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N >> M;
	for(int i = 0; i < N; i++) {
		cin >> H[i][0] >> H[i][1];
		if(H[i][0] > 0) H[i][0] --;
		if(H[i][1] > 0) H[i][1] --;
	}
	for(int i = 0; i < M; i++) {
		cin >> S[i] >> D[i];
		S[i] --, D[i] --;
		hit[S[i]][D[i]] = true;
	}
	for(int i = 0; i < N; i++) {
		par[i] = pii(i, M);
	}
	for(int i = 0; i < N; i++) {
		for(int t = 0; t < 2; t++) {
			if(H[i][t] >= 0 && !hit[i][t]) {
				merge(i, H[i][t], M);
			}
		}
	}
	for(int i = M - 1; i >= 0; i--) {
		if(H[S[i]][D[i]] < 0) continue;
		merge(H[S[i]][D[i]], S[i], i);
	}
	for(int i = 0; i < N; i++) {
		pii p = getpar(i);
		assert(p.A == 0);
		if(p.B == M) p.B = -1;
		cout << p.B << '\n';
	}
}
