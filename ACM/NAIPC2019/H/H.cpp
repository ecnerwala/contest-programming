#include<bits/stdc++.h>
using namespace std;

inline double sq(double v) { return v * v; }

double posAtan2(double y, double x) {
	double res = atan2(y, x);
	if (res < 0) res += 2 * M_PI;
	return res;
}

double go(double x, double y) {
	// Take care of the positive x-axis
	if (abs(y) < 1e-9 && x > -1e-9) return x;

	double res = 2 * M_PI + sqrt(x * x + y * y);
	// 2 options: start going or start turning

	/* Never optimal
	// Case 1: start going: we have to end up on the circle
	if (0 <= y && y <= 2 && x >= -sqrt(max(0., 1 - sq(y-1)))) {
		res = min(res, x - (-sqrt(max(0., 1-sq(y-1)))) + acos(max(-1., min(1.,y-1))) + M_PI);
	}*/

	// Case 2: start turning immediately
	// We never go past facing directly towards it, we also have our circle-center always ccw of the pt
	// We usually want to turn exactly 90 degrees on the circle
	if ((x >= 1 && y >= 0) || (x >= 0 && y >= 0 && x*x+y*y <= 2 && x*x + (y-1)*(y-1) >= 1)) {
		// no initial turning
		// Tangent to the initial circle at (0, 1)
		double tangent = sqrt(x*x + (y-1)*(y-1) - 1);
		double angle = atan2(y-1, x) + asin(1 / sqrt(x * x + (y-1) * (y-1)));
		res = min(res, angle + tangent);
	} else if (x*x+y*y <= 2) {
		// Initial turning, take the arc directly there
		double angle = posAtan2(y, x) + asin(sqrt(x * x + y * y) / 2);
		res = min(res, angle);
	} else {
		// Take the quarter circle
		double tangent = sqrt(x * x + y * y - 1) - 1;
		double angle = posAtan2(y, x) + asin(1 / sqrt(x*x+y*y));
		res = min(res, angle + tangent);
	}

	return res;
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	double x, y; cin >> x >> y;
	double v, w; cin >> v >> w;

	double mult = 1;

	// normalize time to w = 1 radian per second
	mult /= w;
	v /= w;
	w /= w;

	// normalize lengths to v = 1
	x /= v, y /= v;
	v /= v;

	cerr << x << ' ' << y << ' ' << mult << '\n';
	//double res = min(go(x, y), go(x, -y));
	double res = go(x, abs(y));
	res *= mult;
	cout << fixed << setprecision(20) << res << '\n';

	return 0;
}
