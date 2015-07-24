#include "message.h"
#include "highest_mountain.h"
#include<bits/stdc++.h>
using namespace std;

#define SZ(x) (int((x).size()))

typedef long long ll;
typedef pair<ll, ll> pt;
#define X first
#define Y second

const int TOTNODES = NumberOfNodes();
const int ID = MyNodeId();

const int MAXN = 5e3;

const int N = NumberOfPeaks();
const int B = (N + TOTNODES - 1) / TOTNODES;
const int NODES = (N + B - 1) / B;
const int S = min(N, B * ID);
const int T = min(N, B * (ID + 1));

const int MAXB = 5e6;

const int M = 1200; // size of one message

inline pt operator - (pt a, pt b) {
	return pt(a.X - b.X, a.Y - b.Y);
}

// compare slopes
inline bool operator <(pt a, pt b) {
	assert(a.X >= 0 && b.X >= 0);
	return a.Y * b.X < b.Y * a.X;
}

// a < b < c
bool below(pt a, pt b, pt c) {
	return (b - a) < (c - a);
}

void PutPt(int target, pt a) {
	PutInt(target, int(a.X));
	PutInt(target, int(a.Y));
}

pt GetPt(int source) {
	pt a;
	a.X = GetInt(source);
	a.Y = GetInt(source);
	return a;
}

void PutVPt(int target, const vector<pt> &a) {
	PutInt(target, SZ(a));
	for(pt p : a) {
		PutPt(target, p);
	}
}

void GetVPt(int source, vector<pt> &a) {
	int s = GetInt(source);
	while(s--) {
		a.push_back(GetPt(source));
	}
}

int trim[200];

int main() {
	assert(NODES <= TOTNODES);
	assert((S == T) == (ID >= NODES));
	if(ID >= NODES) return 0;

	vector<pt> hull;
	pt prv = pt(S, GetHeight(S));
	for(int i = S + 1; i < T; i++) {
		pt cur = pt(i, GetHeight(i));
		while(!hull.empty() && below(hull.back(), prv, cur)) {
			prv = hull.back();
			hull.pop_back();
		}
		hull.push_back(prv);
		prv = cur;
	}
	hull.push_back(prv);

	//for(pt p : hull) cerr << p.first << ',' << p.second << ' '; cerr << '\n';

	int H = SZ(hull);

	vector<pt> sparse_hull;
	for(int i = 0; i < H; i += M) {
		sparse_hull.push_back(hull[i]);
	}

	for(int i = 0; i < ID; i++) {
		PutVPt(i, sparse_hull);
		Send(i);
	}

	int l = 0;
	int r = H;

	for(int i = ID + 1; i < NODES; i++) {
		Receive(i);
		vector<pt> ohull;
		GetVPt(i, ohull);
		int s = 0;
		while(s < SZ(ohull) && r - l >= 1) {
			if(r - l >= 2 && below(hull[r - 2], hull[r - 1], ohull[s])) {
				r--;
			} else if(s + 1 < SZ(ohull) && below(hull[r - 1], ohull[s], ohull[s + 1])) {
				s++;
			} else {
				break;
			}
		}
		PutInt(i, s);
		Send(i);
	}

	for(int i = 0; i < ID; i++) {
		Receive(i);
		trim[i] = GetInt(i);
		l = max(l, trim[i]);
	}
	r = max(l, r);

	for(int i = 0; i < ID; i++) {
		vector<pt> phull;
		for(int p = l * M; p < r && p < trim[i] * M + M; p++) {
			phull.push_back(hull[p]);
		}
		PutVPt(i, phull);
		Send(i);
	}

	for(int i = ID + 1; i < NODES; i++) {
		Receive(i);
		vector<pt> ohull;
		GetVPt(i, ohull);
		int s = 0;
		while(s < SZ(ohull) && r - l >= 1) {
			if(r - l >= 2 && below(hull[r - 2], hull[r - 1], ohull[s])) {
				r--;
			} else if(s + 1 < SZ(ohull) && below(hull[r - 1], ohull[s], ohull[s + 1])) {
				s++;
			} else {
				break;
			}
		}
	}

	int res = r - l;
	if(ID) {
		PutInt(0, res);
		if(res) {
			PutPt(0, hull[l]);
			if(res > 1) PutPt(0, hull[r - 1]);
		}
		Send(0);
	} else {
		res--;
		//prv = hull.back(); // already set
		hull.clear();
		for(int i = 1; i < NODES; i++) {
			Receive(i);
			int sz = GetInt(i);
			if(!sz) continue;
			pt cur = GetPt(i);
			//cerr << i << ' ' << sz << '\n';
			//cerr << cur.X << ',' << cur.Y << '\n';
			while(!hull.empty() && below(hull.back(), prv, cur)) {
				prv = hull.back();
				hull.pop_back();
			}
			hull.push_back(prv);
			prv = cur;

			if(sz > 1) {
				res += SZ(hull) + 1;
				hull.clear();
				res += sz - 2;
				prv = GetPt(i);
			}
		}
		res += SZ(hull) + 1;
		printf("%d\n", res);
	}

	return 0;
}
