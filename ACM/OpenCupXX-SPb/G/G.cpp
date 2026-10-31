#pragma GCC optimize("Ofast", "unroll-loops")
#pragma GCC target("sse,sse2,sse3,ssse3,popcnt,abm,mmx")
#include<bits/stdc++.h>
using namespace std;

using ll = long long;
template<class T>
struct Point {
	typedef Point P;
	T x, y;
	explicit Point(T x_=0, T y_=0) : x(x_), y(y_) {}
	bool operator<(P p) const { return tie(x,y) < tie(p.x,p.y); }
	bool operator==(P p) const { return tie(x,y)==tie(p.x,p.y); }
	P operator+(P p) const { return P(x+p.x, y+p.y); }
	P operator-(P p) const { return P(x-p.x, y-p.y); }
	P operator*(T d) const { return P(x*d, y*d); }
	P operator/(T d) const { return P(x/d, y/d); }
	T dot(P p) const { return x*p.x + y*p.y; }
	T cross(P p) const { return x*p.y - y*p.x; }
	T cross(P a, P b) const { return (a-*this).cross(b-*this); }
	T dist2() const { return x*x + y*y; }
	double dist() const { return sqrt((double)dist2()); }
	// angle to x-axis in interval [-pi, pi]
	double angle() const { return atan2(y, x); }
	P unit() const { return *this/dist(); } // makes dist()=1
	P perp() const { return P(-y, x); } // rotates +90 degrees
	P normal() const { return perp().unit(); }
	// returns point rotated 'a' radians ccw around the origin
	P rotate(double a) const {
		return P(x*cos(a)-y*sin(a),x*sin(a)+y*cos(a)); }
	friend ostream& operator << (ostream& o, const P& p) {
		return o << "(" << p.x << "," << p.y << ")";
	}
};
template <class P>
bool sameDir(P s, P t) {
	return s.cross(t) == 0 && s.dot(t) > 0;
}
// checks 180 <= s..t < 360?
template <class P>
bool isReflex(P s, P t) {
	auto c = s.cross(t);
	return c ? (c < 0) : (s.dot(t) < 0);
}
// operator < (s,t) for angles in [base,base+2pi)
template <class P>
bool angleCmp(P base, P s, P t) {
	int r = isReflex(base, s) - isReflex(base, t);
	return r ? (r < 0) : (0 < s.cross(t));
}
// is x in [s,t] taken ccw? 1/0/-1 for in/border/out
template <class P>
int angleBetween(P s, P t, P x) {
	if (sameDir(x, s) || sameDir(x, t)) return 0;
	return angleCmp(s, x, t) ? 1 : -1;
}

using pt = Point<ll>;

const int MAXN = 5000;
int N, K;
pt P[MAXN];
int sortInd[MAXN];
int sortLoc[MAXN];

bool movesUp[MAXN];
bool movesDown[MAXN];

struct angleCmptr {
	bool operator () (const pt& a, const pt& b) {
		// return a < b
		return a.cross(b) > 0;
	}
};
pair<pt, pair<int, int>> evtPairs[int(8.1e6)];
int evtPairsSz;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N >> K;
	for (int i = 0; i < N; i++) {
		cin >> P[i].x >> P[i].y;
	}
	for (int i = 0; i < N; i++) {
		P[i+N] = pt() - P[i];
	}
	N *= 2;

	for (int i = 0; i < N; i++) {
		sortInd[i] = i;
	}
	sort(sortInd, sortInd + N, [&](int x, int y) { return P[x] < P[y]; });

	for (int i = 0; i < N; i++) {
		sortLoc[sortInd[i]] = i;
	}
	//cerr << "Starting to generate\n";

	for (int i = 0; i < N; i++) {
		for (int j = i+1; j < N; j++) {
			int a = sortInd[i], b = sortInd[j];
			pt d = P[b] - P[a];
			if (d == pt(0)) {
				continue;
			}
			assert(pt(0) < d);

			/*
			ll g = __gcd(abs(d.x), abs(d.y));
			d.x /= g;
			d.y /= g;
			*/

			evtPairs[evtPairsSz++] = {d, pair<int, int>{a, b}};
		}
	}
	//cerr << "Generated evtPairs\n";
	sort(evtPairs, evtPairs + evtPairsSz, [&](const auto& a, const auto& b) {
			return angleCmptr()(a.first, b.first);
	});
	//cerr << "Sorted\n";
	int L = N/2;
	int R = N/2 + K;
	pt sum(0,0);
	for (int i = L; i < R; i++) {
		sum = sum + P[sortInd[i]];
	}

	pt bestDir(0,-1);
	double bestCross = double(bestDir.cross(sum)) / bestDir.dist();

	auto testDir = [&bestDir, &bestCross](pt dir, pt sum_) {
		//cerr << "testing " << dir << ' ' << sum_ << '\n';
		double cross = double(dir.cross(sum_)) / dir.dist();
		//cerr << "cross " << cross << '\n';
		if (cross > bestCross) {
			bestCross = cross;
			bestDir = dir;
		}
	};

	auto testRange = [&testDir](pt dlo, pt dhi, pt sum_) {
		pt bestAng = pt(sum_.y, -sum_.x);
		if (angleBetween(dlo, dhi, bestAng) == 1) {
			testDir(bestAng, sum_);
		} else {
			testDir(dlo, sum_);
			testDir(dhi, sum_);
		}
	};

	pt prvDir(0, -1);

	for (int ei = 0; ei < evtPairsSz; ) {
		pt dir = evtPairs[ei].first;
		int ej = ei;
		while (ej < evtPairsSz && sameDir(evtPairs[ej].first, evtPairs[ei].first)) {
			ej++;
		}
		//cerr << "evts at " << it.first << '\n';
		testRange(prvDir, dir, sum);
		prvDir = dir;

		for (int k = ei; k < ej; k++) {
			auto evt = evtPairs[k].second;
			int a = evt.first, b = evt.second;
			movesUp[sortLoc[a]] = true;
			movesDown[sortLoc[b]] = true;
		}

		for (int k = ei; k < ej; k++) {
			auto evt = evtPairs[k].second;
			int a = evt.first, b = evt.second;
			if (!movesUp[sortLoc[a]]) continue;
			int lo = sortLoc[a], hi = sortLoc[b];
			while (lo - 1 >= 0 && movesUp[lo-1]) {
				--lo;
			}
			while (hi+1 < N && movesDown[hi+1]) {
				++hi;
			}
			for (int i = lo; i <= hi; i++) {
				movesUp[i] = movesDown[i] = false;
			}
			for (int i = max(lo-1, 0); i+1 <= min(hi+1, N-1); i++) {
				bool isGood = P[sortInd[i]] == P[sortInd[i+1]] || sameDir(dir, P[sortInd[i+1]] - P[sortInd[i]]);
				assert(isGood == (lo <= i && i+1 <= hi));
			}
			//cerr << lo << ' ' << hi << '\n';
			for (int i = lo, j = hi; i < j; i++, j--) {
				if (L <= i && i < R) {
					sum = sum - P[sortInd[i]];
				}
				if (L <= j && j < R) {
					sum = sum - P[sortInd[j]];
				}
				swap(sortInd[i], sortInd[j]);
				sortLoc[sortInd[i]] = i;
				sortLoc[sortInd[j]] = j;
				if (L <= i && i < R) {
					sum = sum + P[sortInd[i]];
				}
				if (L <= j && j < R) {
					sum = sum + P[sortInd[j]];
				}
			}
		}
		ei = ej;
	}
	if (!sameDir(prvDir, pt(0,1))) {
		testRange(prvDir, pt(0,1), sum);
	}

	for (int i = 0; i+1 < N; i++) {
		assert(!(P[sortInd[i]] < P[sortInd[i+1]]));
	}
	cout << fixed << setprecision(15) << bestCross << '\n';
	cout << bestDir.x << ' ' << bestDir.y << '\n';
	vector<pair<double, int>> voters;
	for (int i = 0; i < N/2; i++) {
		voters.emplace_back(abs(bestDir.cross(P[i])), i);
	}
	sort(voters.begin(), voters.end());
	for (int i = 0; i < K; i++) {
		cout << voters[i].second+1 << " \n"[i+1==K];
	}

	return 0;
}
