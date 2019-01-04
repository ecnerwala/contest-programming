#include<bits/stdc++.h>
using namespace std;

// We start with a cube pointing 45degrees, and rotate it up
// Start: [0, 0.5, 0], [+- 1/2rt(2), 0, 1/2rt(2)]
// Rotate by theta:
// [0, 0.5 cos(theta), -0.5 sin(theta)], [+- 1/2rt(2), 1/2rt(2) sin(theta), 1/2rt(2) cos(theta)]
//
// The area of a face shadow just the dot product of the normal vector of the projected plane to the unit vector of the face
// Normal vector: [0, 1, 0]
// The faces vectors are all have norm 0.5, so we scale them up
// Area = cos(theta) + 2 * (1/rt(2) sin(theta)) = cos(theta) + sqrt(2) * sin(theta)
// This is maximized at

typedef long double ld;
const ld MIN_THETA = 0.l;
const ld MAX_THETA = 2 * atan2(sqrt(2.l), 1.l+sqrt(3.l));

int main() {
	ios_base::sync_with_stdio(0);
	int T; cin >> T;

	cerr << MAX_THETA << '\n';

	for(int case_num = 1; case_num <= T; case_num ++) {
		long double A;
		cin >> A;

		long double mi = MIN_THETA, ma = MAX_THETA;
		for (int i = 0; i < 300; i++) {
			long double md = (mi + ma) / 2;
			long double area = cos(md) + sqrt(2.l) * sin(md);
			if (area > A) {
				ma = md;
			} else {
				mi = md;
			}
		}

		cout << "Case #" << case_num << ":\n";

		long double theta = mi;

		cout << fixed << setprecision(15) << 0.l << ' ' << 0.5l * cos(theta) << ' ' << -0.5l * sin(theta) << '\n';
		cout << fixed << setprecision(15) << 0.5l / sqrt(2.l) << ' ' << 0.5l / sqrt(2.l) * sin(theta) << ' ' << 0.5l / sqrt(2.l) * cos(theta) << '\n';
		cout << fixed << setprecision(15) << -0.5l / sqrt(2.l) << ' ' << 0.5l / sqrt(2.l) * sin(theta) << ' ' << 0.5l / sqrt(2.l) * cos(theta) << '\n';
	}

	return 0;
}
