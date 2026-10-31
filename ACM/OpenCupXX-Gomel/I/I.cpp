#include<bits/stdc++.h>
using namespace std;

template <typename T> int sz(const T& t) { return int(t.size()); }
using ll = long long;

using pt = pair<ll, ll>;

ll cross(pt a, pt b, pt c) {
	b.first -= a.first;
	c.first -= a.first;

	b.second -= a.second;
	c.second -= a.second;

	return b.first * c.second - b.second * c.first;
}

ll pt_dist(pt a, pt b) {
	ll x = abs(a.first - b.first);
	ll y = abs(a.second - b.second);
	return __gcd(x, y);
}

bool go() {
	int N; cin >> N;
	vector<pt> pts(N);
	for (int i = 0; i < N; i++) {
		cin >> pts[i].first >> pts[i].second;
	}

	sort(pts.begin(), pts.end());
	vector<pt> lower;
	vector<pt> upper;
	for (pt p : pts) {
		while (sz(lower) >= 2 && cross(lower[sz(lower)-2], lower[sz(lower)-1], p) <= 0) { lower.pop_back(); }
		while (sz(upper) >= 2 && cross(upper[sz(upper)-2], upper[sz(upper)-1], p) >= 0) { upper.pop_back(); }
		lower.push_back(p);
		upper.push_back(p);
	}

	vector<pt> hull = std::move(lower);
	hull.pop_back();
	hull.insert(hull.end(), upper.rbegin(), upper.rend()-1);

	ll area = 2;
	for (int i = 0; i < sz(hull); i++) {
		pt p = hull[i];
		pt q = hull[(i+1)%sz(hull)];
		area += cross(pt{0,0}, p, q);
		area += pt_dist(p, q);
	}

	return area == 2 * N;
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int T; cin >> T;
	while (T--) {
		bool ans = go();
		cout << int(ans) << '\n';
	}

	return 0;
}
