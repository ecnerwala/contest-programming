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

	friend std::ostream& operator << (std::ostream& o, const P& p) {
		return o << "(" << p.x << ", " << p.y << ")";
	}
	friend std::istream& operator >> (std::istream& i, P& p) {
		return i >> p.x >> p.y;
	}
};

template<class P>
bool segmentIntersectionQ(P s1, P e1, P s2, P e2) {
	if (e1 == s1) {
		if (e2 == s2) return e1 == e2;
		swap(s1,s2); swap(e1,e2);
	}
	P v1 = e1-s1, v2 = e2-s2, d = s2-s1;
	auto a = v1.cross(v2), a1 = d.cross(v1), a2 = d.cross(v2);
	if (a == 0) { // parallel
		auto b1 = s1.dot(v1), c1 = e1.dot(v1),
		     b2 = s2.dot(v1), c2 = e2.dot(v1);
		return !a1 && max(b1,min(b2,c2)) <= min(c1,max(b2,c2));
	}
	if (a < 0) { a = -a; a1 = -a1; a2 = -a2; }
	return (0 <= a1 && a1 <= a && 0 <= a2 && a2 <= a);
}

template <class P>
bool segmentIntersectionQ(pair<P, P> a, pair<P, P> b) {
	return segmentIntersectionQ(a.first, a.second, b.first, b.second);
}

using P = Point<ll>;
using seg_t = pair<P, P>;

const int MAXN = 1.1e5;
int N;
pair<seg_t, int> segs[MAXN];
P pts[MAXN * 2];
pair<P, int> sortedPts[MAXN * 2];

using vi = vector<int>;
#define sz(a) int((a).size())
#define all(a) (a).begin(), (a).end()
#define trav(a, x) for (auto& a : x)

pair<vi, vi> ulHull(const vector<P>& S) {
	vi Q(sz(S)), U, L;
	iota(all(Q), 0);                                                                                                                                                                         
	// already sorted
	//sort(all(Q), [&S](int a, int b){ return S[a] < S[b]; }); 
	trav(it, Q) {
#define ADDP(C, cmp) while (sz(C) > 1 && S[C[sz(C)-2]].cross(\
	S[it], S[C.back()]) cmp 0) C.pop_back(); C.push_back(it);
		ADDP(U, <=); ADDP(L, >=);
#undef ADDP
	}   
	return {U, L}; 
}
vi convexHull(const vector<P>& S) {
	vi u, l; tie(u, l) = ulHull(S);
	if (sz(S) <= 1) return u;
	if (S[u[0]] == S[u[1]]) return {0};
	l.insert(l.end(), u.rbegin()+1, u.rend()-1);
	return l;
}
vector<P> convexHullPts(const vector<P>& S) {
	vi inds = convexHull(S);
	vector<P> ans; ans.reserve(inds.size());
	for (int i : inds) {
		ans.push_back(S[i]);
	}
	return ans;
}

const int S = (1 << 18);
vector<P> hulls[2*S];
int hullInds[2*S];

vector<int> go() {
	cin >> N;
	for (int i = 0; i < N; i++) {
		P a, b;
		cin >> a >> b;
		if (b < a) swap(a,b);
		segs[i] = {{a,b}, i};
	}

	sort(segs, segs + N, [&](const pair<seg_t, int>& a, const pair<seg_t, int>& b) {
		return (a.first.second - a.first.first).angle() < (b.first.second - b.first.first).angle();
	});

	for (int i = 0; i < N; i++) {
		pts[i] = segs[i].first.first;
		pts[i+N] = segs[i].first.second;
	}
	for (int i = 0; i < 2 * N; i++) {
		//cerr << pts[i] << '\n';
	}
	for (int i = 0; i < 2 * N; i++) {
		sortedPts[i] = {pts[i], i};
	}
	sort(sortedPts, sortedPts + 2 * N);

	for (int z = 0; z < 2 * N; z++) {
		P p = sortedPts[z].first;
		int i = sortedPts[z].second;
		for (int a = S+i; a; a /= 2) {
			hulls[a].push_back(p);
		}
	}

	for (int a = 2 * S - 1; a; a --) {
		hulls[a] = convexHullPts(hulls[a]);
		hullInds[a] = 0;
	}

	set<int> uneven;
	for (int i = 0; i < 2 * N; i++) {
		// i+N .. i
		int j = (i+N)%(2*N);
		bool isGood = true;
		auto tryHull = [&](int a) {
			if (hulls[a].size() <= 2) {
				for (P p : hulls[a]) {
					//cerr << "try " << i << ' ' << pts[i] << ' ' << pts[j] << ' ' << p << '\n';
					if (p.cross(pts[i], pts[j]) < 0) {
						isGood = false;
					}
				}
				return;
			}
			while (true) {
				ll vPrv = hulls[a][(hullInds[a] + sz(hulls[a])-1) % sz(hulls[a])].cross(pts[i], pts[j]);
				ll vCur = hulls[a][(hullInds[a] + sz(hulls[a])+0) % sz(hulls[a])].cross(pts[i], pts[j]);
				ll vNxt = hulls[a][(hullInds[a] + sz(hulls[a])+1) % sz(hulls[a])].cross(pts[i], pts[j]);

				if (vCur > vPrv) {
					hullInds[a]++;
					continue;
				}
				assert(vCur <= vPrv);
				if (vNxt <= vCur) {
					hullInds[a]++;
					continue;
				}

				assert(vCur <= vPrv && vCur < vNxt);

				//cerr << "try " << i << ' ' << pts[i] << ' ' << pts[j] << ' ' << hulls[a][hullInds[a] % sz(hulls[a])] << '\n';

				if (vCur < 0) isGood = false;
				break;
			}
		};
		// half-open
		auto segQuery = [&](int a, int b) {
			//cerr << "seg " << i << ' ' << a << ' ' << b << '\n';
			for (int l = S+a, r = S+b; l < r; l /= 2, r /= 2) {
				if (l & 1) {
					tryHull(l);
					l++;
				}
				if (r & 1) {
					r--;
					tryHull(r);
				}
			}
		};
		if (j < i) {
			segQuery(j+1, i);
		} else {
			segQuery(j+1, 2*N);
			segQuery(0, i);
		}
		if (!isGood) {
			//cerr << "Uneven " << i << '\n';
			uneven.insert(i % N);
		}
	}

	set<int> ans = uneven;
	for (int i : uneven) {
		for (int j = 0; j < N; j++) {
			if (i == j) continue;
			if (!segmentIntersectionQ(segs[i].first, segs[j].first)) {
				ans.insert(j);
			}
		}
	}

	for (int z = 0; z < 2*S; z++) {
		hulls[z] = {};
	}

	vector<int> realAns;
	for (int i : ans) {
		realAns.push_back(segs[i].second);
	}
	sort(realAns.begin(), realAns.end());

	return realAns;
}

int main() {
	ios_base::sync_with_stdio(0);
	int T; cin >> T;

	for (int case_num = 1; case_num <= T; case_num ++) {

		cerr << "Case #" << case_num << "\n";

		vector<int> ans = go();

		cout << "Case #" << case_num << ":";
		if (ans.empty()) {
			cout << ' ' << "MAGNIFICENT" << '\n';
		} else {
			sort(ans.begin(), ans.end());
			for (int i : ans) {
				cout << ' ' << i+1;
			}
			cout << '\n';
		}
	}

	return 0;
}
