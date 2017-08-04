#include<bits/stdc++.h>
using namespace std;

const double PI2 = atan2(1, 0);
const double TAU = 4 * PI2;

bool go(int P, int X, int Y) {
	X -= 50;
	Y -= 50;
	// if (X * X + Y * Y == 50 * 50) return false; // Under the problem conditions, if it's on the boundary, it has to be outside.
	if (X * X + Y * Y > 50 * 50) {
		return false;
	}
	double deg = atan2(X, Y);
	if (deg < 0) {
		deg += TAU;
	}
	deg /= TAU;
	assert(abs(deg * 100 - P) > 1e-9);
	return deg * 100 < P;
}

int main() {
	int T; cin >> T;
	for(int t = 1; t <= T; t++) {
		cout << "Case #" << t << ": ";
		int P, X, Y;
		cin >> P >> X >> Y;
		cout << (go(P, X, Y) ? "black" : "white") << '\n';
	}
	return 0;
}
