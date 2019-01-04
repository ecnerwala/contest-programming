#include<bits/stdc++.h>
using namespace std;

const int MAXN = int(1e5) + 20;
const int MAXL = 2 * MAXN;

int N[2];

int L;
int V[MAXL];

const int MAXT = 150;
int T;
int nxt[MAXL][MAXT];

int pos[MAXL];
void make_nxt() {
	memset(nxt, 0, sizeof(nxt));

	int S = 0;
	for(int i = 0; i < L; i++) {
		if(V[i] == 0) {
			// do some resetting
			S = 0;
		}
		for(int j = 0, cur = i; true; j++) {
			swap(cur, pos[j]);
			if(j == S) {
				S++;
				break;
			}
			if(V[cur] == V[i]) {
				break;
			}
		}
		//cerr << i << '\n';
		//for(int j = 0; j < S; j++) { cerr << pos[j] << ' '; } cerr << '\n';
		for(int j = 2; j < S; j++) {
			if(V[pos[j-1]] == V[i+1]) break;
			assert(nxt[pos[j]+1][j] == 0);
			nxt[pos[j]+1][j] = pos[j-1]+1;
		}
		//for(int j = 2; j <= T; j++) { for(int i = 0; i < L; i++) { cerr << nxt[i][j] << ' '; } cerr << '\n'; }
	}
	//for(int j = 2; j <= T; j++) { for(int i = 0; i < L; i++) { cerr << nxt[i][j] << ' '; } cerr << '\n'; }
}

char has[MAXT];

const int MAXNV = MAXL;
int NV;
vector<int> b1[MAXNV];
vector<int> b2[MAXNV];

bool go() {
	scanf("%d %d", &N[0], &N[1]);

	NV = 100;

	memset(has, 0, sizeof(has));

	V[L = 0] = 0;
	for(int i = 0; i < N[0]; i++) {
		int v; scanf("%d", &v);
		if(v == V[L]) {
			N[0]--;
			i--;
		} else {
			V[++L] = v;
			has[v] |= 1;
		}
	}
	V[++L] = 0;
	for(int i = 0; i < N[1]; i++) {
		int v; scanf("%d", &v);
		if(v == V[L]) {
			N[1]--;
			i--;
		} else {
			V[++L] = v;
			has[v] |= 2;
		}
	}
	V[++L] = 0;
	//for(int i = 0; i <= L; i++) {cerr << V[i] << ' ';} cerr << '\n';

	T = 0;
	for(int i = 1; i <= NV; i++) {
		if(has[i] == 3) T++;
		else if(has[i] != 0) return false;
	}

	make_nxt();

	for(int t = 2; t <= T; t++) {
		for(int i = 0; i <= L; i++) {
			if(nxt[i][t]) {
				b1[V[i]].push_back(i);
			}
		}

		for(int i = NV-1; i >= 0; i--) {
			for(__typeof(b1[i].begin()) it = b1[i].begin(); it != b1[i].end(); it++) {
				b2[V[nxt[*it][t]]].push_back(*it);
			}
			b1[i].clear();
		}

		int C = 0;
		for(int i = NV-1; i >= 0; i--) {
			int cur = -1;
			for(__typeof(b2[i].begin()) it = b2[i].begin(); it != b2[i].end(); it++) {
				if(V[*it] != cur) {
					C++;
					cur = V[*it];
				}
				V[*it] = C-1;
			}
			b2[i].clear();
		}
		NV = C;

		//cerr << t << '\n';
		//for(int i = 0; i <= L; i++) {cerr << V[i] << ' ';} cerr << '\n';
	}

	assert(NV <= 2);

	return V[1] == V[N[0]+2];
}

int main() {
	int Q; scanf("%d", &Q);
	while(Q--) {
		bool res = go();
		printf("%d\n", int(res));
	}

	return 0;
}
