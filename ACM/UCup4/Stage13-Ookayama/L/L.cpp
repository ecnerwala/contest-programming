#include <bits/stdc++.h>
using namespace std;
#include <sys/time.h>

double get_time() {timeval tv; gettimeofday(&tv, NULL); return tv.tv_sec + tv.tv_usec * 1e-6;}
double start_time = get_time();
double elapsed() {return get_time() - start_time;}

template <class T> int sgn(T x) { return (x > 0) - (x < 0); }
template<class T>
struct Point {
	typedef Point P;
	T x, y;
	explicit Point(T x=0, T y=0) : x(x), y(y) {}
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
	friend ostream& operator<<(ostream& os, P p) {
		return os << "(" << p.x << "," << p.y << ")"; }
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

using ll = int64_t;
using P = Point<ll>;

#define all(x) x.begin(), x.end()
#define sz(x) (int)(x).size()

vector<P> convexHull(vector<P> pts) {
	if (sz(pts) <= 1) return pts;
	// sort(all(pts));
	vector<P> h(sz(pts)+1);
	int s = 0, t = 0;
	for (int it = 2; it--; s = --t, reverse(all(pts)))
		for (P p : pts) {
			while (t >= s + 2 && h[t-2].cross(h[t-1], p) <= 0) t--;
			h[t++] = p;
		}
	return {h.begin(), h.begin() + t - (t == 2 && h[0] == h[1])};
}

int main(){
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	constexpr bool TESTING = true;
	int T;
	if (TESTING) {
		T = 100;
	} else {
		cin >> T;
	}
	std::mt19937 mt_test(48);
	vector<vector<pair<P, int> > > test_cases(T);
	for(auto& test_case : test_cases){
		int N;
		if (TESTING) {
			N = std::uniform_int_distribution<int>(1, 10)(mt_test);
		} else {
			cin >> N;
		}
		test_case.resize(N);
		for(auto& [p, c] : test_case){
			if (TESTING) {
				const int X = 10;
				p.x = std::uniform_int_distribution<int>(-X, X)(mt_test);
				p.y = std::uniform_int_distribution<int>(-X, X)(mt_test);
				c = std::uniform_int_distribution<int>(1, N)(mt_test);
			} else {
				cin >> p.x >> p.y >> c;
			}
			c--;
		}
		sort(test_case.begin(), test_case.end());
	}
	std::vector<ll> slow_ans(T);
	if (TESTING) {
		for (int t = 0; t < T; t++) {
			auto& pts = test_cases[t];
			int N = int(pts.size());
			for (int i = 0; i < N; i++) {
				for (int j = 0; j < N; j++) {
					if (pts[j].second == pts[i].second) continue;
					for (int k = 0; k < N; k++) {
						if (pts[k].second == pts[i].second || pts[k].second == pts[j].second) continue;
						for (int l = 0; l < N; l++) {
							if (pts[l].second == pts[i].second || pts[l].second == pts[j].second || pts[l].second == pts[k].second) continue;
							ll cnd = (pts[i].first - pts[j].first).cross(pts[k].first - pts[l].first);
							slow_ans[t] = std::max(slow_ans[t], cnd);
						}
					}
				}
			}
		}
	}
	const double TIME_LIMIT = 3.8;
	vector<ll> ans(T);
	mt19937 mt(48);
	auto cycle = [&](vector<P> pts) -> vector<P> {
		if(pts.size() <= 1) return pts;
		int N = (int)pts.size();
		int i = 0;
		while(angleCmp(P(1, 0), pts[(i)%N] - pts[(i+N-1)%N], pts[(i+1)%N]-pts[(i+0)%N])) i++;
		rotate(pts.begin(), pts.begin()+i, pts.end());
		return pts;
	};
	while(elapsed() < TIME_LIMIT){
		for(int t = 0; t < T; t++){
			auto& pts = test_cases[t];
			int N = (int)test_cases[t].size();
			vector<int> color_mapping(N);
			for(int i = 0; i < N; i++) color_mapping[i] = mt() % 4;
			vector<vector<P> > hulls(4);
			for(int i = 0; i < N; i++){
				hulls[color_mapping[pts[i].second]].push_back(pts[i].first);
			}
			for(int a = 0; a < 4; a++){
				hulls[a] = convexHull(hulls[a]);
			}
			vector<vector<P>> mink(2);
			for(int l : {0, 1}){
				auto& hull0 = hulls[l];
				auto& hull1 = hulls[l+2];
				for(auto& p : hull1) p = p * -1;
				if(hull0.empty() || hull1.empty()) continue;
				hull0 = cycle(hull0);
				hull1 = cycle(hull1);

				int A = hull0.size();
				int B = hull1.size();
				auto get_mink = [&](){
					if(A == 1){
						for(auto& p : hull1) p = p + hull0[0];
						return hull1;
					}
					if(B == 1){
						for(auto& p : hull0) p = p + hull1[0];
						return hull0;
					}
					hull0.push_back(hull0[0]);
					hull1.push_back(hull1[0]);
					int idx0 = 0;
					int idx1 = 0;
					vector<P> res;
					while(idx0 < A || idx1 < B){
						res.push_back(hull0[idx0] + hull1[idx1]);
						if(idx0 < A && idx1 < B && sameDir(hull0[idx0+1] - hull0[idx0], hull1[idx1+1] - hull1[idx1])){
							idx0++;
							idx1++;
						} else if(idx1 == B || (idx0 < A && angleCmp(P(1, 0), hull0[idx0+1] - hull0[idx0], hull1[idx1+1] - hull1[idx1]))){
							idx0++;
						} else {
							idx1++;
						}
					}
					int L = (int)res.size();
					vector<P> filtered_res;
					for(int i = 0; i < L; i++){
						if(res[i] == P(0, 0)) continue;
						if((L <= 2) || (res[i].cross(res[(i+1)%L]) > 0 || res[(i+L-1)%L].cross(res[i]) > 0)) filtered_res.push_back(res[i]);
					}
					res.pop_back();
					return res;
				};
				mink[l] = get_mink();
			}
			int A = (int)mink[0].size();
			int B = (int)mink[1].size();
			if(A <= 2 || B <= 2){
				for(auto p : mink[0]){
					for(auto q : mink[1]){
						ans[t] = max(ans[t], abs(p.cross(q)));
					}
				}
			} else {
				ll cur_best = 0;
				int idx1 = 0;
				{
					auto p = mink[0][0];
					for(int j = 0; j < B; j++){
						if(p.cross(mink[1][idx1]) < p.cross(mink[1][j])) idx1 = j;
					}
				}
				for(auto p : mink[0]){
					while(true){
						cur_best = max(cur_best, abs(p.cross(mink[1][idx1])));
						if(p.cross(mink[1][idx1]) < p.cross(mink[1][(idx1+1) % B])){
							idx1++;
							if(idx1 == B) idx1 = 0;
						} else if(p.cross(mink[1][idx1]) < p.cross(mink[1][(idx1+B-1) % B])){
							if(idx1 == 0) idx1 = B;
							idx1--;
						} else break;
					}
				}
				if (TESTING) {
					ll slow_best = 0;
					for(auto p : mink[0]){
						for(auto q : mink[1]){
							slow_best = max(slow_best, abs(p.cross(q)));
						}
					}
					if (slow_best != cur_best) {
						cerr << mink[0].size() << '\n';
						for (auto p : mink[0]) { cerr << p << '\n'; }
						cerr << mink[1].size() << '\n';
						for (auto p : mink[1]) { cerr << p << '\n'; }
						cerr << "expected " << slow_best << '\n';
						cerr << "got " << cur_best << '\n';
						assert(false);
					}
				}
			}
		}
	}
	for(int t = 0; t < T; t++){
		cout << ans[t] << '\n';
	}
	for (int t = 0; t < T; t++) {
		if (slow_ans[t] != ans[t]) {
			cerr << "have " << slow_ans[t] << ' ' << "got " << ans[t] << '\n';
			for (auto [p, c] : test_cases[t]) {
				cerr << p.x << ' ' << p.y << ' ' << c << '\n';
			}
			assert(false);
		}
	}
}
