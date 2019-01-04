/*
LANG: C++
ID: he.andr1
PROG: garbage
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

const int MAXN = 200000;
const int MAXM = 2000000;

int N, M;

set<int> adj[MAXN];
bool inStack[MAXN];

vector<int> curPath;

vector<vector<int> > ans;

void dfs(int i) {
	if(inStack[i]) {
		curPath.push_back(i);
		return;
	}
	inStack[i] = true;
	while(!adj[i].empty()) {
		int v = *(adj[i].begin());
		adj[i].erase(v);
		adj[v].erase(i);

		dfs(v);
		assert(curPath.size() > 0);

		curPath.push_back(i);

		if(curPath[0] == i) {
			ans.push_back(curPath);
			curPath.clear();
		}
	}
	inStack[i] = false;
}

bool go() {
	for(int i = 1; i <= N; i++) if(adj[i].size() % 2 == 1) return false;

	for(int i = 1; i <= N; i++) {
		if(adj[i].size()) dfs(i);
	}

	return true;
}

void io(istream &in, ostream &out) {
	scanf("%d%d", &N, &M);
	for(int i = 0; i < M; i++) {
		int a, b, s, t;
		scanf("%d%d%d%d", &a, &b, &s, &t);
		if(s != t) {
			adj[a].insert(b);
			adj[b].insert(a);
		}
	}
	if(go()) {
		printf("%d\n", ans.size());
		ITER(it, ans) {
			printf("%d", it->size() - 1);
			ITER(v, (*it)) {
				printf(" %d", *v);
			}
			printf("\n");
		}
	} else {
		printf("NIE\n");
	}
}

int main() {
		io(cin, cout);
	return 0;
}
