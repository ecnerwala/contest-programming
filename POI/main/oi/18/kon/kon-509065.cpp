/*
LANG: C++
ID: he.andr1
PROG: kon
*/

#include<iostream>
#include<fstream>
#include<cstdio>
#include<algorithm>
#include<vector>
#include<stack>
#include<queue>
#include<deque>
#include<set>
#include<map>
#include<list>
#include<string>
#include<cstring>
#include<cassert>
#include<numeric>
#include<complex>
#include<cmath>
#include<ctime>

using namespace std;

#define DEBUG 1

#ifdef DEBUG
	#define ERR cerr
#else
	#define ERR if(true); else cerr
#endif

#define ITER(i, v) for(__typeof(v.begin()) i = v.begin(); i != v.end(); i++) 
#define X real()
#define Y imag()
#define A first
#define B second

typedef long long ll;
typedef pair<int, int> pii;
typedef complex<int> pt;

template <class T> T cross(complex<T> a, complex<T> b) { return imag(a * conj(b)); }

template <class T> T min(T a, T b, T c) { return min(a, min(b, c)); }
template <class T> T max(T a, T b, T c) { return max(a, max(b, c)); }

template <class T> void setmin(T &a, T b) { if(b < a) a = b; }
template <class T> void setmax(T &a, T b) { if(b > a) a = b; }

const int MAXN = 10000;

int N;
bool E[MAXN][MAXN];

const char BAD = -1;

vector<char> vis;

inline void printVis() { 
	//for(int i = 0; i < N; i++) cerr << char(vis[i] == BAD ? '-' : (vis[i] + '0')); cerr << '\n'; 
}

bool dfs(int c, bool e) {
	if(vis[c] == e) return true;
	else if(vis[c] == !e) return false;
	vis[c] = e;
	for(int i = 0; i < N; i++) {
		if(c == i) continue;
		if(E[c][i] == !e) {
			if(!dfs(i, !e)) return false;
		}
	}
	return true;
}


ll recurse(int cur = 0) {
	if(cur == N) return 1;
	if(vis[cur] != BAD) return recurse(cur + 1);
	vector<char> v = vis;
	ll ans = 0;
	for(int e = 0; e < 2; e++) {
		if(dfs(cur, e)) ans += recurse(cur + 1);
		vis = v;
	}
	return ans;
} 

void greedy() {
	for(int i = 0; i < N; i++) {
		if(vis[i] != BAD) continue;
		vector<char> v = vis;
		int good = 0;
		for(int e = 0; e < 2; e++) {
			if(dfs(i, e)) good |= (1 << e);
			vis = v;
		}
		if(good == 0) return 0;
		for(int e = 0; e < 2; e++) {
			if(good == (1 << e)) {
				//cerr << i << ' ' << e << '\n';
				dfs(i, e);
			}
		}
	}

	cerr << "greedy done\n";
}

ll go() {
	int e = 0;
	for(int i = 0; i < N; i++) for(int j = 0; j < i; j++) e += E[i][j];
	if(e == 0 || e == N * (N - 1) / 2) return N;

	vis = vector<char>(N);
	fill(vis.begin(), vis.end(), BAD);

	return recurse();
}

void io(istream &in, ostream &out) {
	memset(E, false, sizeof(E));
	scanf("%d", &N);
	for(int i = 0; i < N; i++) {
		int k; scanf("%d", &k);
		for(int j = 0; j < k; j++) {
			int a; scanf("%d", &a); a--;
			E[i][a] = true;
		}
	}
	printf("%lld\n", go());
}

int main() {
		io(cin, cout);
	return 0;
}
