/*
LANG: C++
ID: he.andr1
PROG: prz
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

const int MAXN = 4000;

int N;
int A[MAXN];

int inversions() {
	int ans = 0;
	for(int i = 0; i < N; i++) {
		for(int j = i + 1; j < N; j++) {
			if(A[i] > A[j]) ans++;
		}
	}
	return ans;
}

//negative is type a, positive is type b
vector<int> moves;

int curHead = 0;

void setHead(int start) {
	if(curHead != start) {
		int v = - (curHead - start + N);
		if(!moves.empty() && moves.back() < 0) {
			moves.back() += v;
		} else moves.push_back(v);
		curHead = start;
	}
}

void moveB() {
	swap(A[(curHead + 2) % N], A[(curHead + 1) % N]);
	swap(A[(curHead + 1) % N], A[(curHead + 0) % N]);
}

void moveB(int num) {
	if(!moves.empty() && moves.back() > 0) {
		moves.back() += num;
	} else {
		moves.push_back(num);
	}
	for(int i = 0; i < num; i++) moveB();
}

void move(int loc, int num) {
	while(num >= 2) {
		setHead((loc - 2 + N) % N);
		moveB(1);
		num -= 2;
		loc -= 2;
	}
	if(num) {
		setHead((loc - 1 + N) % N);
		moveB(2);
	}
}

bool go() {
	bool parity = inversions() % 2;
	if(parity) {
		if(N % 2) {
			return false;
		} else {
			moves.push_back(-(N - 1));
			A[N] = A[0];
			for(int i = 0; i < N; i++) A[i] = A[i + 1];
		}
	}

	for(int i = 0; i < N; i++) {
		for(int j = i; j < N; j++) {
			if(A[j] == i) {
				move(j, j - i);
				break;
			}
		}
	}

	setHead(0);

	return true;
}

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N;
	for(int i = 0; i < N; i++) cin >> A[i], A[i]--;

	if(!go()) {
		cout << "NIE DA SIE" << '\n';
	} else {
		cout << moves.size() << '\n';
		for(int i = 0; i < moves.size(); i++) {	
			cout << (abs(moves[i]) % N) << ((moves[i] < 0) ? 'a' : 'b') << ((i + 1 == moves.size()) ? '\n' : ' ');
		}
	}

	return 0;
}
