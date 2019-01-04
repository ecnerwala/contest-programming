#include<cstdio>
#include<iostream>

using namespace std;

typedef long long ll;
typedef pair<int, int> pii;
#define A first
#define B second

const int MAXN = 2e3;
const int MAXL = 2e2;
const int MAXK = 2e5;
const int MAXD = 50;

const int D = 26;
const int P1 = 1e9 + 7;
const int P2 = 1e9 + 9;

int N, L, K;
char C[MAXN][MAXL];

pii curHash[MAXN];

int res[MAXN];

inline int getHash(char *c, int mod) {
	ll val = 0;
	for(int i = 0; i < L; i++) {
		val *= D;
		val += (c[i] - 'a');
		while(val >= mod) val -= mod;
	}
	return val;
}
inline pii getHash(char *c) {
	return pii(getHash(c, P1), getHash(c, P2));
}
inline pii getHash(int v) {
	return getHash(C[v]);
}

inline void proc(int v) {
	int cnt = 0;
	for(int i = 0; i < N; i++) {
		if(curHash[i] == curHash[v]) {
			cnt++;
		}
	}
	for(int i = 0; i < N; i++) {
		if(curHash[i] == curHash[v]) {
			if(cnt > res[i]) res[i] = cnt;
		}
	}
}

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N >> L >> K;
	for(int i = 0; i < N; i++) cin >> C[i];

	for(int i = 0; i < N; i++) {
		curHash[i] = getHash(i);
	}

	for(int i = 0; i < K; i++) {
		int t1, c1, t2, c2;
		cin >> t1 >> c1 >> t2 >> c2;
		t1--, c1--, t2--, c2--;
		swap(C[t1][c1], C[t2][c2]);
		curHash[t1] = getHash(t1);
		if(t1 != t2) curHash[t2] = getHash(t2);

		//cerr << i << '\n';
		//for(int j = 0; j < N; j++) cerr << C[j] << ' ' << curHash[j].A << ',' << curHash[j].B << '\n';

		proc(t1);
		if(curHash[t1] != curHash[t2]) proc(t2);
	}
	
	for(int i = 0; i < N; i++) {
		cout << res[i] << '\n';
	}
	return 0;
}
