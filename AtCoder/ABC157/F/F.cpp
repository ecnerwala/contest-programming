#include<bits/stdc++.h>
using namespace std;

template <typename T> T sq(T a) { return a * a; }

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int N, K; cin >> N >> K;
	vector<pair<pair<int, int>, int>> circles(N);
	for (auto& it : circles) {
		cin >> it.first.first >> it.first.second >> it.second;
	}

	double mi = 0;
	double ma = 1e7;
	for (int z = 0; z < 200; z++) {
		double md = (mi + ma) / 2;
		bool good = false;
		auto checkPt = [&](double x, double y) { 
			int cnt = 0;
			for (auto it : circles) {
				cnt += hypot(x - it.first.first, y - it.first.second) * it.second <= md;
			}
			if (cnt >= K) good = true;
		};

		for (int i = 0; i < N; i++) {
			checkPt(circles[i].first.first, circles[i].first.second);
		}
		for (int i = 0; i < N; i++) {
			for (int j = i+1; j < N; j++) {
				double x1, y1, x2, y2;
				tie(x1, y1) = circles[i].first, tie(x2, y2) = circles[j].first;
				double r1 = md / circles[i].second, r2 = md / circles[j].second;
				double d = hypot(x1 - x2, y1 - y2);
				if (r1 + r2 + d < 2 * max({r1, r2, d})) {
					continue;
				}
				double parallel = (sq(d) + sq(r1) - sq(r2)) / 2 / d;
				double perp = sqrt(max(0., sq(r1) - sq(parallel)));
				perp = max(0., perp - 1e-8);
				double xmid = x1 + (x2 - x1) / d * parallel;
				double ymid = y1 + (y2 - y1) / d * parallel;
				double xa = xmid + (y2 - y1) / d * perp, ya = ymid - (x2 - x1) / d * perp;
				checkPt(xa, ya);
				double xb = xmid - (y2 - y1) / d * perp, yb = ymid + (x2 - x1) / d * perp;
				checkPt(xb, yb);
			}
		}
		if (good) {
			ma = md;
		} else {
			mi = md;
		}
	}
	cout << fixed << setprecision(10) << ma << '\n';

	return 0;
}
