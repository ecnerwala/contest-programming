#include<bits/stdc++.h>
using namespace std;

typedef long double ld;

const ld PI2 = atan2(1, 0);

// returns true if it's too low
// a is in radians
bool follow(int N, ld a) {

	ld x = 0, y = a;

	for(int i = 1; i <= N; i++) {
		ld v = (cos(x) * cos(y) - cos(2 * y)) / sin(y);
		if(v > 1) return false;
		if(v < sin(y)) return false;
		x = y;
		y = asin(v);
		assert(y > x);
	}

	return true;
}

ld start(int N) {
	ld mi = 0, ma = PI2 / N;
	while(follow(N, ma)) ma *= 2;
	for(int i = 0; i < 200; i++) {
		ld md = (mi + ma) / 2;
		//cerr << mi << ' ' << md << ' ' << ma << '\n';
		if(follow(N, md)) {
			mi = md;
		} else {
			ma = md;
		}
	}
	return mi;
}

ld S(int N) {

	ld x = 0, y = start(N);

	//cerr << x << ' ' << y << ' ';

	ld res = 0;

	for(int i = 1; i <= N; i++) {

		res -= sin(2 * y) / 2;
		res += cos(x) * sin(y);

		ld v = (cos(x) * cos(y) - cos(2 * y)) / sin(y);
		assert(v <= 1);
		assert(v > sin(y));
		x = y;
		y = asin(v);
		assert(y > x);

		//cerr << y << ' ';

	}
	//cerr << '\n';

	res += cos(x) * sin(y);

	return res * 4;
}

int main() {
	cout << fixed << setprecision(10) << S(200) << '\n';
	//cout << start(200) << '\n';
	return 0;
}
