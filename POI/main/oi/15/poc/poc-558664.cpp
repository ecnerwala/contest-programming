#include<cstdio>
#include<iostream>
#include<algorithm>

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

//stores hash, time, value
typedef pair<pii, pii> state; 

const int MAXS = 2 * MAXN + 2 * MAXK;
int S;
state evts[MAXS];

pii loc[MAXK];

int startloc[MAXN];
void process() {
	pii curHash = pii(-1, -1);
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
			loc[v] = pii(curPref, curTime);
			while(v > 0 && loc[v - 1] < loc[v]) {
				loc[v - 1] = loc[v];
				v--;
			}
			v++;
			curTime = cur.B.A;
		}
		int l = cur.B.B;
		if(l < 0) {
			curPref--;
			l = -1 - l;
			int mi = -1, ma = v - 1;
			while(ma - mi > 1) {
				int md = (mi + ma) / 2;
				if(loc[md].B >= startloc[l]) ma = md;
				else mi = md;
			}
			int cnd = loc[ma].A;
			if(cnd > res[l]) res[l] = cnd;
		} else {
			curPref++;
			startloc[l] = curTime;
		}
	}
}

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N >> L >> K;
	for(int i = 0; i < N; i++) cin >> C[i];

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
		swap(C[t1][c1], C[t2][c2]);
		curHash[t1] = getHash(t1);
		if(t1 != t2) curHash[t2] = getHash(t2);

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
