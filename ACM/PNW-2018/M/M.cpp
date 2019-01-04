#include<bits/stdc++.h>
using namespace std;

using pdd = pair<double, double>;

const int MAXN = 4e4;
int N;
int C;
pair<double, double> P[MAXN];

double cross(pdd a, pdd b) {
	return a.first * b.second - a.second * b.first;
}
pdd operator - (pdd a, pdd b) {
	return pdd(b.first - a.first, b.second - a.second);
}
double cross(pdd a, pdd b, pdd c) {
	return cross(b - a, c - a);
}

int main() {
	cin >> N >> C;
	for (int i = 0; i < N; i++) {
		int c; double h, p; cin >> c >> h >> p;
		P[i].first = (h * C) / c;
		P[i].second = (p * C) / c;
	}
	sort(P, P + N);
	vector<pair<double, double>> hull;
	for (int i = 0; i < N; i++) {
		while (hull.size() >= 2) {
			pdd cur = hull.back(); hull.pop_back();
			if (cross(hull.back(), cur, P[i]) > 0) {
				// leave it popped
			} else {
				hull.push_back(cur);
				break;
			}
		}
		hull.push_back(P[i]);
	}
	double res = 0;
	for (int i = 0; i < int(hull.size()); i++) {
		res = max(res, hull[i].first * hull[i].second);
	}
	for (int i = 0; i+1 < int(hull.size()); i++) {
		pdd a = hull[i];
		pdd b = hull[i+1];
		if (b.first == a.first || b.second == a.second) continue;
		double t = - ((b.first - a.first) * a.second + (b.second - a.second) * a.first) / ((b.first - a.first) * (b.second - a.second)) / 2;
		if (0 < t && t < 1) {
			pdd c = pdd(a.first + (b.first - a.first) * t, a.second + (b.second - a.second) * t);
			res = max(res, c.first * c.second);
		}
	}
	cout << fixed << setprecision(2) << res << '\n';

	return 0;
}
