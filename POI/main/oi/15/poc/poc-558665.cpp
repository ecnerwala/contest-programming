#include<cstdio>
#include<iostream>
#include<algorithm>

using namespace std;

typedef long long ll;
typedef pair<int, int> pii;
#define A first
#define B second

typedef ll hash;

const int MAXN = 2e3;
const int MAXL = 2e2;
const int MAXK = 2e5;
const int MAXD = 50;

const int D = 26;
const int P1 = 1e9 + 7;
const int P2 = 1e9 + 9;

const ll P = 3e17 + 11;

int N, L, K;
char C[MAXN][MAXL];

hash curHash[MAXN];

int res[MAXN];

ll exp[MAXL];

inline void precomp() {
	exp[0] = 1;
	for(int i = 1; i < L; i++) {
		exp[i] = exp[i - 1] * D;
		while(exp[i] >= P) exp[i] -= P;
	}
}

inline ll getHash(char *c, ll mod) {
	ll val = 0;
	for(int i = L - 1; i >= 0; i--) {
		val *= D;
		val += (c[i] - 'a');
		while(val >= mod) val -= mod;
	}
	return val;
}

inline hash getHash(char *c) {
	return getHash(c, P);
}
inline hash getHash(int v) {
	return getHash(C[v]);
}

inline void set(int n, int l, char c) {
	curHash[n] += exp[l] * (c - C[n][l]);
	while(curHash[n] >= P) curHash[n] -= P;
	while(curHash[n] < 0) curHash[n] += P;
	C[n][l] = c;
}

//stores hash, time, value
typedef pair<hash, pii> state; 

const int MAXS = 2 * MAXN + 2 * MAXK;
int S;
state evts[MAXS];

int loc[MAXK];
int pref[MAXK];

int starttime[MAXN];
void process() {
	hash curHash = -1;
	int curTime = -1;
	int curPref = 0;

	int v = 0;
	for(int i = 0; i < S; i++) {
		state cur = evts[i];
		if(cur.A != curHash) {
			curTime = -1; 
			curHash = cur.A;
			curPref = 0;
			v = 0;
		}
		if(curTime < cur.B.A) {
			while(v > 0 && pref[v - 1] < curPref) {
				v--;
			}

			pref[v] = curPref;
			loc[v] = curTime;
			v ++;

			curTime = cur.B.A;
		}
		int l = cur.B.B;
		if(l < 0) {
			curPref--;
			l = -1 - l;
			int mi = -1, ma = v - 1;
			int t = starttime[l];
			while(ma - mi > 1) {
				int md = (mi + ma) / 2;
				if(loc[md] >= t) ma = md;
				else mi = md;
			}
			int cnd = pref[ma];
			if(cnd > res[l]) res[l] = cnd;
		} else {
			curPref++;
			starttime[l] = curTime;
		}
	}
}

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N >> L >> K;
	for(int i = 0; i < N; i++) cin >> C[i];

	precomp();
	S = 0;
	for(int i = 0; i < N; i++) {
		curHash[i] = getHash(i);
		evts[S++] = state(curHash[i], pii(0, i));
	}

	for(int i = 1; i <= K; i++) {
		int t1, c1, t2, c2;
		cin >> t1 >> c1 >> t2 >> c2;
		t1--, c1--, t2--, c2--;

		evts[S++] = state(curHash[t1], pii(i, -t1 - 1));
		if(t1 != t2) {
			evts[S++] = state(curHash[t2], pii(i, -t2 - 1));
		}

		char a = C[t1][c1], b = C[t2][c2];
		set(t2, c2, a);
		set(t1, c1, b);

		//cerr << i << '\n';
		//for(int j = 0; j < N; j++) cerr << C[j] << ' ' << curHash[j].A << ',' << curHash[j].B << '\n';

		evts[S++] = state(curHash[t1], pii(i, t1));
		if(t1 != t2) {
			evts[S++] = state(curHash[t2], pii(i, t2));
		}
	}

	for(int i = 0; i < N; i++) {
		evts[S++] = state(curHash[i], pii(K + 1, -i - 1));
	}

	sort(evts, evts + S);

	for(int i = 0; i < N; i++) {
		res[i] = 1;
	}

	process();

	for(int i = 0; i < N; i++) {
		cout << res[i] << '\n';
	}
	return 0;
}
