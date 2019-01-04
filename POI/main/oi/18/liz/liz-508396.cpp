/*
LANG: C++
ID: he.andr1
PROG: lollipop
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

const int MAXN = 1000010;
const int MAXM = 1000010;
const int MAXL = 2 * MAXN;

int N, M;
char S[MAXN];

const int BAD = -1;
int val[MAXL][2];

void precomp() {
	memset(val, BAD, sizeof(val));

	int V;

	int i;
	for(i = 0; i < N && S[i] == 'T'; i++) ;
	V = i;

	if(V == N) {
		for(int i = 1; i <= N; i++) {
			val[2 * i - 1][0] = BAD;
			val[2 * i][0] = 1;
			val[2 * i][1] = i;
		}
	}

	bool rev = false;

	for(i = 0; i < N && S[N - i - 1] == 'T'; i++) ;
	if(i < V) {
		reverse(S, S + N);
		V = i;
		rev = true;
	}

	int sum = 0;
	for(i = V; i < N; i++) {
		bool b = S[i] == 'T';
		sum += (b ? 2 : 1);
		val[sum][0] = V + 1;
		val[sum][1] = i + 1;
		if(b) {
			val[sum - 1][0] = V + 2;
			val[sum - 1][1] = i + 1;
		}
	}

	for(i = V; i > 0; i--) {
		sum += 2;
		val[sum - 1][0] = BAD;
		val[sum][0] = i;
		val[sum][1] = N;
	}

	if(rev) {
		for(i = 1; i <= sum; i++) {
			if(val[i][0] == BAD) continue;
			swap(val[i][0], val[i][1]);
			val[i][0] = N + 1 - val[i][0];
			val[i][1] = N + 1 - val[i][1];
		}
	}

}

void io(istream &in, ostream &out) {
	scanf("%d%d", &N, &M);
	scanf("%s", &S);
	precomp();
	//cerr << "precomp done\n";
	for(int i = 0; i < M; i++) {
		int k;
		scanf("%d", &k);
		if(val[k][0] == BAD) printf("NIE\n");
		else printf("%d %d\n", val[k][0], val[k][1]);
	}
}

int main() {
		io(cin, cout);
	return 0;
}
