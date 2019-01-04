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

const int BAD = -1;

int K;
int N;
vector<pii> A;

vector<int> tri;

bool check(int a, int b, int c) {
	if(a == BAD || c == BAD) return false;
	if(A[a].A + A[b].A > A[c].A) {
		tri.push_back(A[a].B);
		tri.push_back(A[a].A);
		tri.push_back(A[b].B);
		tri.push_back(A[b].A);
		tri.push_back(A[c].B);
		tri.push_back(A[c].A);
		return true;
	}
	return false;
}

vector<int> rem;

int insert(int p, vector<int> & c) {
	vector<int> n;
	n.push_back(p);
	for(int i = 0; n.size() < 3; i++) {
		assert(i < 3);
		if(c[i] != BAD && A[c[i]].B == A[p].B) {
			continue;
		}
		n.push_back(c[i]);
	}
	int ans = (c[0] + c[1] + c[2]) - (n[1] + n[2]);
	c = n;
	return ans;
}

//c is sorted
void undo(int r, vector<int> &c) {
	if(r == BAD) {
		c.erase(c.begin());
		c.push_back(r);
	} else {
		c[0] = r;
		for(int i = 1; i < 3; i++) {
			if(c[i] != BAD && c[i-1] > c[i]) swap(c[i - 1], c[i]);
		}
	}
}

bool go() {
	//cerr << "starting\n";
	sort(A.begin(), A.end());

	//ITER(it, A) cerr << it->A << ' ' << it->B << '\n';
	
	rem.resize(N);

	vector<int> h(3, BAD);

	for(int i = N - 1; i >= 0; i--) {
		rem[i] = insert(i, h);
	}

	//cerr << "precomp\n";
	
	vector<int> l (3, BAD);

	for(int i = 0; i < N; i++) {
		insert(i, l);

		//cerr << l[0] << ' ' << l[1] << ' ' << l[2] << '\n';
		//cerr << h[0] << ' ' << h[1] << ' ' << h[2] << '\n';

		if(A[l[1]].B == A[h[1]].B) {
			if(check(l[1], l[0], h[2])) return true;
			if(check(l[2], l[0], h[1])) return true;
		} else {
			if(check(l[1], l[0], h[1])) return true;
		}

		undo(rem[i], h);
	}

	return false;
}

int main() {
	ios_base::sync_with_stdio(0);

	cin >> K;

	for(int i = 1; i <= K; i++) {
		int n; cin >> n;
		for(; n > 0; n--) {
			int v; cin >> v;
			A.push_back(pii(v, i));
		}
	}

	N = A.size();

	if(go()) {
		//cerr << "printing\n";
		for(int i = 0; i < tri.size(); i++) {
			cout << tri[i] << ((i + 1 == tri.size()) ? '\n' : ' ');
		}
	} else {
		cout << "NIE" << '\n';
	}

	return 0;
}
