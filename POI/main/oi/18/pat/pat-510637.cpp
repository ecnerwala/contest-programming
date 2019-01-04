/*
LANG: C++
ID: he.andr1
PROG: pat
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

const int MAXK = 100;
const int MAXN = 2000000;

const pii BAD(0, -1);

int K;
int N;
pii A[MAXN];

vector<pii> lo[MAXN];
vector<pii> hi[MAXN];

vector<pii> insert(pii p, vector<pii> c) {
	vector<pii> ans;
	ans.push_back(p);
	for(int i = 0; ans.size() < 3; i++) {
		assert(i < 3);
		if(c[i].B == p.B) continue;
		ans.push_back(c[i]);
	}
	return ans;
}

vector<pii> tri;

bool check(pii a, pii b, pii c) {
	if(a == BAD || c == BAD) return false;
	if(a.A + b.A > c.A) {
		tri.push_back(a);
		tri.push_back(b);
		tri.push_back(c);
		return true;
	}
	return false;
}

bool go() {
	//cerr << "starting\n";
	sort(A, A + N);

	vector<pii> cur;

	cur = vector<pii> (3, BAD);

	for(int i = 0; i < N; i++) {
		cur = insert(A[i], cur);
		lo[i] = cur;
	}

	cur = vector<pii> (3, BAD);

	for(int i = N - 1; i >= 0; i--) {
		cur = insert(A[i], cur);
		hi[i] = cur;
	}
	
	//cerr << "precomp\n";

	for(int i = 0; i < N; i++) {
		vector<pii> l = lo[i], h = hi[i];
		if(l[1].B == h[1].B) {
			if(check(l[1], l[0], h[2])) return true;
			if(check(l[2], l[0], h[1])) return true;
		} else {
			if(check(l[1], l[0], h[1])) return true;
		}
	}

	return false;
}

int main() {
	ios_base::sync_with_stdio(0);

	cin >> K;

	for(int i = 1; i <= K; i++) {
		int n; cin >> n;
		for(; n > 0; n--) {
			cin >> A[N].A;
			A[N].B = i;
			N++;
		}
	}

	if(go()) {
		//cerr << "printing\n";
		for(int i = 0; i < 3; i++) {
			cout << tri[i].B << ' ' << tri[i].A << ((i + 1 == 3) ? '\n' : ' ');
		}
	} else {
		cout << "NIE" << '\n';
	}

	return 0;
}
