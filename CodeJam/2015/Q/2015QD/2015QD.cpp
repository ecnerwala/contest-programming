#include<bits/stdc++.h>
using namespace std;

bool can_tile(int X, int R, int C) {
	if(R * C % X != 0) return false; // obviously
	if(X > R && X > C) return false; // we can use the 1xX
	if(X >= min(R, C) * 2 + 1) return false // use the bent piece

	if(X >= 7) return false; // we can make a ring

	if(X == 1) return true;
	if(X == 2) return true;

	if(X == 3) {
		return true;
	} else if(X == 4) {

	}

	return true;
}

int main() {
	int T; cin >> T;
	for(int t = 1; t <= T; t++) {
		int X, R, C; cin >> X >> R >> C;
		bool res = can_tile(X, R, C);
		cout << "Case #" << t << ": " << (res ? "GABRIEL" : "RICHARD") << '\n';
	}
	return 0;
}
