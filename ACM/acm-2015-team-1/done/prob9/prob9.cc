#include<bits/stdc++.h>
using namespace std;

typedef double ld;

typedef long long ll;

const ld INF = 1e9;

typedef pair<ll, ll> pll;
typedef pair<ld, ld> pt;
#define X first
#define Y second

template<class T> inline T cross(pair<T, T> a, pair<T, T> b) { return a.X * b.Y - b.X * a.Y; }
template<class T> inline T dot(pair<T, T> a, pair<T, T> b) { return a.X * b.X + a.Y * b.Y; }
template<class T> inline T norm(pair<T, T> a) { return dot(a, a); }

const int MAXN = 2000;

int N, K;
pll C;
pll A[MAXN];
pll B[MAXN];

bool good( ld r ) {
	r *= r;
	vector<pair<ld, bool>> evts; // 0 is insert, 1 is remove
	for(int i = 0; i < N; i++) {
		ll normb = norm(B[i]);
		ll norma = norm(A[i]);
		if(normb == 0) {
			if(norma <= r) {
				evts.push_back(make_pair(0., false));
			}
			continue;
		}
		ll dotprod = dot(A[i], B[i]);
		ld det = dotprod * dotprod - ((norma - r) * normb);
		if(det < 0) continue;
		ld rtdet = sqrt(det);
		ld st = -dotprod - rtdet;
		ld en = -dotprod + rtdet;
		assert(st <= en);
		st /= normb;
		en /= normb;

		st = max(st, 0.);
		if(en < 0) continue;
		evts.push_back(make_pair(st, false));
		evts.push_back(make_pair(en, true));
	}
	sort(evts.begin(), evts.end());
	int cnt = 0;
	int res = 0;
	for(auto it : evts) {
		if(it.second) cnt--;
		else cnt++;
		res = max(res, cnt);
	}
	return res >= K;
}

ld go() {
	ld mi = 0, ma = INF;
	for(int i = 0; i < 200; i++) {
		ld md = (mi + ma) / 2;
		if(good(md)) {
			ma = md;
		} else {
			mi = md;
		}
	}
	return ma;
}

int main() {
	ios_base::sync_with_stdio(0);
	cin >> C.X >> C.Y;
	cin >> N >> K;
	for(int i = 0; i < N; i++) {
		cin >> A[i].X >> A[i].Y >> B[i].X >> B[i].Y;
		B[i].X -= A[i].X;
		B[i].Y -= A[i].Y;
		A[i].X -= C.X;
		A[i].Y -= C.Y;
	}
	printf("%.5lf\n", go());
	return 0;
}
