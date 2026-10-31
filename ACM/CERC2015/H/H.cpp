#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	vector<pair<double, double>> pts(4);
	for (int i = 0; i < 4; i++) {
		cin >> pts[i].first >> pts[i].second;
	}

	double ans = 0.8 * 5;

	const double weights[4] = {3, 1, 4, 6};
	for (int z = 0; z < 4; z++) {
		// take the x >= 0.5 area
		vector<pair<double, double>> curPts; curPts.reserve(8);

		for (int i = 0; i < 4; i++) {
			auto p1 = pts[i];
			auto p2 = pts[(i+1)%4];

			curPts.emplace_back(max(p1.first, 0.5), p1.second);

			// if these interesect, add that point too
			if (p1 > p2) swap(p1, p2);
			if (p1.first <= 0.5 && p2.first >= 0.5)  {
				curPts.emplace_back(0.5, (p1.second * (p2.first - 0.5) + p2.second * (0.5 - p1.first)) / (p2.first - p1.first));
			}
		}

		double area = 0;
		for (int i = 0; i < int(curPts.size()); i++) {
			auto [x1, y1] = curPts[i];
			auto [x2, y2] = curPts[(i+1)%int(curPts.size())];
			area += x1 * y2 - x2 * y1;
		}
		area /= 2;
		//cerr << area << '\n';

		ans += weights[z] * area / 25;
		// rotate the outside square ccw
		for (int i = 0; i < 4; i++) {
			auto [x,y] = pts[i];
			pts[i] = {-y, x};
		}
	}

	ans *= 125;
	ans /= 124;

	cout << fixed << setprecision(10) << ans << '\n';

	return 0;
}
