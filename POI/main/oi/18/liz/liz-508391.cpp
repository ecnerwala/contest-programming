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

const int MAXN = 2000000;
const int MAXM = 2000000;
const int MAXL = 2 * MAXN;

int N, M;
string S;

const pii BAD(-1, -1);

pii val[MAXL];

void precomp() {

	int V;

	int i;
	for(i = 0; i < N && S[i] == 'T'; i++) ;
	V = i;

	if(V == N) {
		for(int i = 1; i <= N; i++) {
			val[2 * i - 1] = BAD;
			val[2 * i] = pii(1, i);
		}
	}

	for(i = 0; i < N && S[N - i - 1] == 'T'; i++) ;
	if(i < V) {
		reverse(S.begin(), S.end());
		V = i;
	}

	int sum = 0;
	for(i = V; i < N; i++) {
		sum += (S[i] == 'T' ? 2 : 1);
		val[sum] = pii(V + 1, i + 1);
		if(S[i] == 'T') {
			val[sum - 1] = pii(V + 2, i + 1);
		}
	}

	for(i = V; i > 0; i--) {
		sum += 2;
		val[sum - 1] = BAD;
		val[sum] = pii(i, N);
	}

}

void io(istream &in, ostream &out) {
	in >> N >> M;
	in >> S;
	precomp();
	for(int i = 0; i < M; i++) {
		int k; in >> k;
		if(val[k] == BAD) out << "NIE" << '\n';
		else out << val[k].A << ' ' << val[k].B << '\n';
	}
}

int main() {
	ifstream fin ("lollipop.in");
	if(fin.good()) {
		ofstream fout ("lollipop.out");
		io(fin, fout);
		fin.close();
		fout.close();
	} else 
		io(cin, cout);
	return 0;
}
