#include<bits/stdc++.h>
using namespace std;

const int MAXN = 1.1e5;
int N;

const double PI = acos(-1);

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N;

	double slope0 = 0;
	double val0 = 0;

	map<double, double> slopeChanges;

	for (int i = 0; i < N; i++) {
		double T, s, a; cin >> T >> s >> a;

		val0 += max(0., T - s * min(a, 2*PI - a));

		if (T / s >= PI) {
			// never dies
			if (a < PI) {
				slope0 += s;
				slopeChanges[a] -= 2 * s;
				slopeChanges[a + PI] += 2 * s;
			} else {
				slope0 -= s;
				slopeChanges[a - PI] += 2 * s;
				slopeChanges[a] -= 2 * s;
			}
		} else {
			if (a < PI) {
				if (a - T / s >= 0) {
					slope0 += 0;
					slopeChanges[a - T / s] += s;
					slopeChanges[a] -= 2 * s;
					slopeChanges[a + T / s] += s;
				} else {
					slope0 += s;
					slopeChanges[a] -= 2 * s;
					slopeChanges[a + T / s] += s;
					slopeChanges[PI + PI + a - T / s] += s;
				}
			} else {
				if (a + T / s < 2 * PI) {
					slope0 += 0;
					slopeChanges[a - T / s] += s;
					slopeChanges[a] -= 2 * s;
					slopeChanges[a + T / s] += s;
				} else {
					slope0 -= s;
					slopeChanges[a + T / s - 2 * PI] += s;
					slopeChanges[a - T / s] += s;
					slopeChanges[a] -= 2 * s;
				}
			}
		}
	}

	double ans = val0;
	double curX = 0;
	for (auto it : slopeChanges) {
		val0 += (it.first - curX) * slope0;
		ans = max(ans, val0);
		slope0 += it.second;
		curX = it.first;
	}

	//cerr << curX << '\n';
	if (2 * PI > curX) val0 += (2 * PI - curX) * slope0;
	ans = max(ans, val0);
	cout << fixed << setprecision(10) << ans << '\n';

	return 0;
}
